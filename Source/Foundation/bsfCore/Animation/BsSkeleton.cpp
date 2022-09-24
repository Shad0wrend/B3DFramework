//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Animation/BsSkeleton.h"
#include "Animation/BsAnimationClip.h"
#include "Animation/BsSkeletonMask.h"
#include "Private/RTTI/BsSkeletonRTTI.h"

namespace bs
{
	LocalSkeletonPose::LocalSkeletonPose(UINT32 numBones, bool individualOverride)
		: NumBones(numBones)
	{
		const UINT32 overridesPerBone = individualOverride ? 3 : 1;

		UINT32 elementSize = sizeof(Vector3) * 2 + sizeof(Quaternion) + sizeof(bool) * overridesPerBone;
		UINT8* buffer = (UINT8*)bs_alloc(elementSize * numBones);

		Positions = (Vector3*)buffer;
		buffer += sizeof(Vector3) * numBones;

		Rotations = (Quaternion*)buffer;
		buffer += sizeof(Quaternion) * numBones;

		Scales = (Vector3*)buffer;
		buffer += sizeof(Vector3) * numBones;

		HasOverride = (bool*)buffer;
	}

	LocalSkeletonPose::LocalSkeletonPose(UINT32 numPos, UINT32 numRot, UINT32 numScale)
	{
		UINT32 bufferSize = sizeof(Vector3) * numPos + sizeof(Quaternion) * numRot + sizeof(Vector3) * numScale;
		UINT8* buffer = (UINT8*)bs_alloc(bufferSize);

		Positions = (Vector3*)buffer;
		buffer += sizeof(Vector3) * numPos;

		Rotations = (Quaternion*)buffer;
		buffer += sizeof(Quaternion) * numRot;

		Scales = (Vector3*)buffer;
	}

	LocalSkeletonPose::LocalSkeletonPose(LocalSkeletonPose&& other)
		: Positions{std::exchange(other.Positions, nullptr)}
		, Rotations{std::exchange(other.Rotations, nullptr)}
		, Scales{std::exchange(other.Scales, nullptr)}
		, HasOverride{std::exchange(other.HasOverride, nullptr)}
		, NumBones(std::exchange(other.NumBones, 0))
	{ }

	LocalSkeletonPose::~LocalSkeletonPose()
	{
		if (Positions != nullptr)
			bs_free(Positions);
	}

	LocalSkeletonPose& LocalSkeletonPose::operator=(LocalSkeletonPose&& other)
	{
		if (this != &other)
		{
			if (Positions != nullptr)
				bs_free(Positions);

			Positions = std::exchange(other.Positions, nullptr);
			Rotations = std::exchange(other.Rotations, nullptr);
			Scales = std::exchange(other.Scales, nullptr);
			HasOverride = std::exchange(other.HasOverride, nullptr);
			NumBones = std::exchange(other.NumBones, 0);
		}

		return *this;
	}

	Skeleton::Skeleton(BONE_DESC* bones, UINT32 numBones)
		: mNumBones(numBones), mBoneTransforms(bs_newN<Transform>(numBones)), mInvBindPoses(bs_newN<Matrix4>(numBones))
		, mBoneInfo(bs_newN<SkeletonBoneInfo>(numBones))
	{
		for(UINT32 i = 0; i < numBones; i++)
		{
			mBoneTransforms[i] = bones[i].LocalTfrm;
			mInvBindPoses[i] = bones[i].InvBindPose;
			mBoneInfo[i].Name = bones[i].Name;
			mBoneInfo[i].Parent = bones[i].Parent;
		}
	}

	Skeleton::~Skeleton()
	{
		if(mBoneTransforms != nullptr)
			bs_deleteN(mBoneTransforms, mNumBones);

		if(mInvBindPoses != nullptr)
			bs_deleteN(mInvBindPoses, mNumBones);

		if (mBoneInfo != nullptr)
			bs_deleteN(mBoneInfo, mNumBones);
	}

	SPtr<Skeleton> Skeleton::Create(BONE_DESC* bones, UINT32 numBones)
	{
		Skeleton* rawPtr = new (bs_alloc<Skeleton>()) Skeleton(bones, numBones);

		return bs_shared_ptr<Skeleton>(rawPtr);
	}

	void Skeleton::GetPose(Matrix4* pose, LocalSkeletonPose& localPose, const SkeletonMask& mask,
		const AnimationClip& clip, float time, bool loop)
	{
		bs_frame_mark();
		{
			FrameVector<AnimationCurveMapping> boneToCurveMapping(mNumBones);

			AnimationState state;
			state.Curves = clip.GetCurves();
			state.Length = clip.GetLength();
			state.BoneToCurveMapping = boneToCurveMapping.data();
			state.Loop = loop;
			state.Weight = 1.0f;
			state.Time = time;

			FrameVector<TCurveCache<Vector3>> positionCache(state.Curves->Position.size());
			FrameVector<TCurveCache<Quaternion>> rotationCache(state.Curves->Rotation.size());
			FrameVector<TCurveCache<Vector3>> scaleCache(state.Curves->Scale.size());

			state.PositionCaches = positionCache.data();
			state.RotationCaches = rotationCache.data();
			state.ScaleCaches = scaleCache.data();
			state.GenericCaches = nullptr;
			state.Disabled = false;

			AnimationStateLayer layer;
			layer.Index = 0;
			layer.Additive = false;
			layer.States = &state;
			layer.NumStates = 1;

			clip.GetBoneMapping(*this, state.BoneToCurveMapping);

			GetPose(pose, localPose, mask, &layer, 1);
		}
		bs_frame_clear();
	}

	void Skeleton::GetPose(Matrix4* pose, LocalSkeletonPose& localPose, const SkeletonMask& mask,
		const AnimationStateLayer* layers, UINT32 numLayers)
	{
		// Note: If more performance is required this method could be optimized with vector instructions

		assert(localPose.NumBones == mNumBones);

		for(UINT32 i = 0; i < mNumBones; i++)
		{
			localPose.Positions[i] = Vector3::ZERO;
			localPose.Rotations[i] = Quaternion::ZERO;
			localPose.Scales[i] = Vector3::ONE;
		}

		bool* hasAnimCurve = bs_stack_alloc<bool>(mNumBones);
		bs_zero_out(hasAnimCurve, mNumBones);

		// Note: For a possible performance improvement consider keeping an array of only active (non-disabled) bones and
		// just iterate over them without mask checks. Possibly also a list of active curve mappings to avoid those checks
		// as well.
		for(UINT32 i = 0; i < numLayers; i++)
		{
			const AnimationStateLayer& layer = layers[i];

			float invLayerWeight;
			if (layer.Additive)
			{
				float weightSum = 0.0f;
				for (UINT32 j = 0; j < layer.NumStates; j++)
					weightSum += layer.States[j].Weight;

				invLayerWeight = 1.0f / weightSum;
			}
			else
				invLayerWeight = 1.0f;

			for (UINT32 j = 0; j < layer.NumStates; j++)
			{
				const AnimationState& state = layer.States[j];
				if (state.Disabled)
					continue;

				float normWeight = state.Weight * invLayerWeight;

				// Early exit for clips that don't contribute (which there could be plenty especially for sequential blends)
				if (Math::ApproxEquals(normWeight, 0.0f))
					continue;

				for (UINT32 k = 0; k < mNumBones; k++)
				{
					if (!mask.IsEnabled(k))
						continue;

					const AnimationCurveMapping& mapping = state.BoneToCurveMapping[k];
					UINT32 curveIdx = mapping.Position;
					if (curveIdx != (UINT32)-1)
					{
						const TAnimationCurve<Vector3>& curve = state.Curves->Position[curveIdx].Curve;
						localPose.Positions[k] += curve.Evaluate(state.Time, state.PositionCaches[curveIdx], false) * normWeight;

						localPose.HasOverride[k] = false;
						hasAnimCurve[k] = true;
					}

					curveIdx = mapping.Scale;
					if (curveIdx != (UINT32)-1)
					{
						const TAnimationCurve<Vector3>& curve = state.Curves->Scale[curveIdx].Curve;
						localPose.Scales[k] *= curve.Evaluate(state.Time, state.ScaleCaches[curveIdx], false) * normWeight;

						localPose.HasOverride[k] = false;
						hasAnimCurve[k] = true;
					}

					if (layer.Additive)
					{
						curveIdx = mapping.Rotation;
						if (curveIdx != (UINT32)-1)
						{
							bool isAssigned = localPose.Rotations[k].W != 0.0f;
							if (!isAssigned)
								localPose.Rotations[k] = Quaternion::IDENTITY;

							const TAnimationCurve<Quaternion>& curve = state.Curves->Rotation[curveIdx].Curve;

							Quaternion value = curve.Evaluate(state.Time, state.RotationCaches[curveIdx], false);
							value = Quaternion::Lerp(normWeight, Quaternion::IDENTITY, value);

							localPose.Rotations[k] *= value;
							localPose.HasOverride[k] = false;
							hasAnimCurve[k] = true;
						}
					}
					else
					{
						curveIdx = mapping.Rotation;
						if (curveIdx != (UINT32)-1)
						{
							const TAnimationCurve<Quaternion>& curve = state.Curves->Rotation[curveIdx].Curve;
							Quaternion value = curve.Evaluate(state.Time, state.RotationCaches[curveIdx], false) * normWeight;

							if (value.Dot(localPose.Rotations[k]) < 0.0f)
								value = -value;

							localPose.Rotations[k] += value;
							localPose.HasOverride[k] = false;
							hasAnimCurve[k] = true;
						}
					}
				}
			}
		}

		// Apply default local tranform to non-animated bones (so that any potential child bones are transformed properly)
		for(UINT32 i = 0; i < mNumBones; i++)
		{
			if(hasAnimCurve[i])
				continue;

			localPose.Positions[i] = mBoneTransforms[i].GetPosition();
			localPose.Rotations[i] = mBoneTransforms[i].GetRotation();
			localPose.Scales[i] = mBoneTransforms[i].GetScale();
		}

		// Calculate local pose matrices
		UINT32 isGlobalBytes = sizeof(bool) * mNumBones;
		bool* isGlobal = (bool*)bs_stack_alloc(isGlobalBytes);
		memset(isGlobal, 0, isGlobalBytes);

		for(UINT32 i = 0; i < mNumBones; i++)
		{
			bool isAssigned = localPose.Rotations[i].W != 0.0f;
			if (!isAssigned)
				localPose.Rotations[i] = Quaternion::IDENTITY;
			else
				localPose.Rotations[i].Normalize();

			if (localPose.HasOverride[i])
			{
				isGlobal[i] = true;
				continue;
			}

			pose[i] = Matrix4::TRS(localPose.Positions[i], localPose.Rotations[i], localPose.Scales[i]);
		}

		// Calculate global poses
		// Note: For a possible performance improvement consider sorting bones in such order so that parents (and overrides)
		// always come before children, we no isGlobal check is needed.
		std::function<void(UINT32)> calcGlobal = [&](UINT32 boneIdx)
		{
			UINT32 parentBoneIdx = mBoneInfo[boneIdx].Parent;
			if (parentBoneIdx == (UINT32)-1)
			{
				isGlobal[boneIdx] = true;
				return;
			}

			if (!isGlobal[parentBoneIdx])
				calcGlobal(parentBoneIdx);

			pose[boneIdx] = pose[parentBoneIdx] * pose[boneIdx];
			isGlobal[boneIdx] = true;
		};

		for (UINT32 i = 0; i < mNumBones; i++)
		{
			if (!isGlobal[i])
				calcGlobal(i);
		}

		for (UINT32 i = 0; i < mNumBones; i++)
			pose[i] = pose[i] * mInvBindPoses[i];

		bs_stack_free(isGlobal);
		bs_stack_free(hasAnimCurve);
	}

	Transform Skeleton::CalcBoneTransform(UINT32 idx) const
	{
		if(idx >= mNumBones)
			return Transform::IDENTITY;

		Transform output = mBoneTransforms[idx];

		UINT32 parentIdx = mBoneInfo[idx].Parent;
		while(parentIdx != (UINT32)-1)
		{
			output.MakeWorld(mBoneTransforms[parentIdx]);

			parentIdx = mBoneInfo[parentIdx].Parent;
		}

		return output;
	}

	UINT32 Skeleton::GetRootBoneIndex() const
	{
		for (UINT32 i = 0; i < mNumBones; i++)
		{
			if (mBoneInfo[i].Parent == (UINT32)-1)
				return i;
		}

		return (UINT32)-1;
	}

	SPtr<Skeleton> Skeleton::CreateEmpty()
	{
		Skeleton* rawPtr = new (bs_alloc<Skeleton>()) Skeleton();

		SPtr<Skeleton> newSkeleton = bs_shared_ptr<Skeleton>(rawPtr);
		return newSkeleton;
	}

	RTTITypeBase* Skeleton::GetRttiStatic()
	{
		return SkeletonRTTI::Instance();
	}

	RTTITypeBase* Skeleton::GetRtti() const
	{
		return GetRttiStatic();
	}
}
