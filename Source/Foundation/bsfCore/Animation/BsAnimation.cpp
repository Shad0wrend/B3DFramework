//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Animation/BsAnimation.h"
#include "Animation/BsAnimationManager.h"
#include "Animation/BsAnimationClip.h"
#include "Animation/BsAnimationUtility.h"
#include "Scene/BsSceneObject.h"
#include "Animation/BsMorphShapes.h"

namespace bs
{
	AnimationClipInfo::AnimationClipInfo(const HAnimationClip& clip)
		: Clip(clip)
	{ }

	AnimationProxy::AnimationProxy(UINT64 id)
		: Id(id)
	{ }

	AnimationProxy::~AnimationProxy()
	{
		Clear();
	}

	void AnimationProxy::Clear()
	{
		if (Layers == nullptr)
			return;

		for(UINT32 i = 0; i < NumLayers; i++)
		{
			AnimationStateLayer& layer = Layers[i];
			for(UINT32 j = 0; j < layer.NumStates; j++)
			{
				AnimationState& state = layer.States[j];

				if(state.Curves != nullptr)
				{
					{
						UINT32 numCurves = (UINT32)state.Curves->Position.size();
						for (UINT32 k = 0; k < numCurves; k++)
							state.PositionCaches[k].~TCurveCache();
					}

					{
						UINT32 numCurves = (UINT32)state.Curves->Rotation.size();
						for (UINT32 k = 0; k < numCurves; k++)
							state.RotationCaches[k].~TCurveCache();
					}

					{
						UINT32 numCurves = (UINT32)state.Curves->Scale.size();
						for (UINT32 k = 0; k < numCurves; k++)
							state.ScaleCaches[k].~TCurveCache();
					}

					{
						UINT32 numCurves = (UINT32)state.Curves->Generic.size();
						for (UINT32 k = 0; k < numCurves; k++)
							state.GenericCaches[k].~TCurveCache();
					}
				}

				if(Skeleton != nullptr)
				{
					UINT32 numBones = Skeleton->GetNumBones();
					for (UINT32 k = 0; k < numBones; k++)
						state.BoneToCurveMapping[k].~AnimationCurveMapping();
				}

				if(state.SoToCurveMapping != nullptr)
				{
					for(UINT32 k = 0; k < NumSceneObjects; k++)
						state.SoToCurveMapping[k].~AnimationCurveMapping();
				}

				state.~AnimationState();
			}

			layer.~AnimationStateLayer();
		}

		for(UINT32 i = 0; i < NumMorphShapes; i++)
		{
			MorphShapeInfos[i].Shape.~SPtr<MorphShape>();
		}

		// All of the memory is part of the same buffer, so we only need to free the first element
		bs_free(Layers);
		Layers = nullptr;
		GenericCurveOutputs = nullptr;
		SceneObjectInfos = nullptr;
		SceneObjectTransforms = nullptr;

		NumLayers = 0;
		NumGenericCurves = 0;
	}

	void AnimationProxy::Rebuild(const SPtr<bs::Skeleton>& skeleton, const bs::SkeletonMask& mask,
		Vector<AnimationClipInfo>& clipInfos, const Vector<AnimatedSceneObject>& sceneObjects,
		const SPtr<MorphShapes>& morphShapes)
	{
		this->Skeleton = skeleton;
		this->SkeletonMask = mask;

		// Note: I could avoid having a separate allocation for LocalSkeletonPoses and use the same buffer as the rest
		// of AnimationProxy
		if (skeleton != nullptr)
			SkeletonPose = LocalSkeletonPose(skeleton->GetNumBones());

		NumSceneObjects = (UINT32)sceneObjects.size();
		if (NumSceneObjects > 0)
			SceneObjectPose = LocalSkeletonPose(NumSceneObjects, true);
		else
			SceneObjectPose = LocalSkeletonPose();

		Rebuild(clipInfos, sceneObjects, morphShapes);
	}

	void AnimationProxy::Rebuild(Vector<AnimationClipInfo>& clipInfos, const Vector<AnimatedSceneObject>& sceneObjects,
		const SPtr<MorphShapes>& morphShapes)
	{
		Clear();

		bs_frame_mark();
		{
			FrameVector<bool> clipLoadState(clipInfos.size());
			FrameVector<AnimationStateLayer> tempLayers;
			UINT32 clipIdx = 0;
			for (auto& clipInfo : clipInfos)
			{
				UINT32 layer = clipInfo.State.Layer;
				if (layer == (UINT32)-1)
					layer = 0;
				else
					layer += 1;

				auto iterFind = std::find_if(tempLayers.begin(), tempLayers.end(),
					[&](auto& x)
				{
					return x.Index == layer;
				});

				bool isLoaded = clipInfo.Clip.IsLoaded();
				clipLoadState[clipIdx] = isLoaded;

				if (iterFind == tempLayers.end())
				{
					tempLayers.push_back(AnimationStateLayer());
					AnimationStateLayer& newLayer = tempLayers.back();

					newLayer.Index = layer;
					newLayer.Additive = isLoaded && clipInfo.Clip->IsAdditive();
				}

				clipIdx++;
			}

			std::sort(tempLayers.begin(), tempLayers.end(),
				[&](auto& x, auto& y)
			{
				return x.Index < y.Index;
			});

			NumLayers = (UINT32)tempLayers.size();
			UINT32 numClips = (UINT32)clipInfos.size();
			UINT32 numBones;

			if (Skeleton != nullptr)
				numBones = Skeleton->GetNumBones();
			else
				numBones = 0;

			UINT32 numPosCurves = 0;
			UINT32 numRotCurves = 0;
			UINT32 numScaleCurves = 0;

			clipIdx = 0;
			for (auto& clipInfo : clipInfos)
			{
				bool isLoaded = clipLoadState[clipIdx++];
				if (!isLoaded)
					continue;

				SPtr<AnimationCurves> curves = clipInfo.Clip->GetCurves();
				numPosCurves += (UINT32)curves->Position.size();
				numRotCurves += (UINT32)curves->Rotation.size();
				numScaleCurves += (UINT32)curves->Scale.size();
			}

			NumGenericCurves = 0;
			if(clipInfos.size() > 0 && clipLoadState[0])
			{
				SPtr<AnimationCurves> curves = clipInfos[0].Clip->GetCurves();
				NumGenericCurves = (UINT32)curves->Generic.size();
			}

			UINT32* mappedBoneIndices = (UINT32*)bs_frame_alloc(sizeof(UINT32) * NumSceneObjects);
			for (UINT32 i = 0; i < NumSceneObjects; i++)
				mappedBoneIndices[i] = -1;

			UINT32 numBoneMappedSOs = 0;
			if (Skeleton != nullptr)
			{
				for (UINT32 i = 0; i < NumSceneObjects; i++)
				{
					if (sceneObjects[i].So.IsDestroyed(true))
						continue;

					// Empty string always means root bone
					if (sceneObjects[i].CurveName.empty())
					{
						UINT32 rootBoneIdx = Skeleton->GetRootBoneIndex();
						if (rootBoneIdx != (UINT32)-1)
						{
							mappedBoneIndices[i] = rootBoneIdx;
							numBoneMappedSOs++;
						}
					}
					else
					{
						for (UINT32 j = 0; j < numBones; j++)
						{
							if (Skeleton->GetBoneInfo(j).Name == sceneObjects[i].CurveName)
							{
								mappedBoneIndices[i] = j;

								numBoneMappedSOs++;
								break;
							}
						}
					}
				}
			}

			if (morphShapes != nullptr)
			{
				NumMorphChannels = morphShapes->GetNumChannels();
				NumMorphVertices = morphShapes->GetNumVertices();

				NumMorphShapes = 0;
				for (UINT32 i = 0; i < NumMorphChannels; i++)
					NumMorphShapes += morphShapes->GetChannel(i)->GetNumShapes();
			}
			else
			{
				NumMorphChannels = 0;
				NumMorphShapes = 0;
				NumMorphVertices = 0;
			}

			UINT32 numBoneMappings = numBones * numClips;
			UINT32 layersSize = sizeof(AnimationStateLayer) * NumLayers;
			UINT32 clipsSize = sizeof(AnimationState) * numClips;
			UINT32 boneMappingSize = numBoneMappings * sizeof(AnimationCurveMapping);
			UINT32 posCacheSize = numPosCurves * sizeof(TCurveCache<Vector3>);
			UINT32 rotCacheSize = numRotCurves * sizeof(TCurveCache<Quaternion>);
			UINT32 scaleCacheSize = numScaleCurves * sizeof(TCurveCache<Vector3>);
			UINT32 genCacheSize = NumGenericCurves * sizeof(TCurveCache<float>);
			UINT32 genericCurveOutputSize = NumGenericCurves * sizeof(float);
			UINT32 sceneObjectIdsSize = NumSceneObjects * sizeof(AnimatedSceneObjectInfo);
			UINT32 sceneObjectTransformsSize = numBoneMappedSOs * sizeof(Matrix4);
			UINT32 morphChannelSize = NumMorphChannels * sizeof(MorphChannelInfo);
			UINT32 morphShapeSize = NumMorphShapes * sizeof(MorphShapeInfo);

			UINT8* data = (UINT8*)bs_alloc(layersSize + clipsSize + boneMappingSize + posCacheSize + rotCacheSize +
				scaleCacheSize + genCacheSize + genericCurveOutputSize + sceneObjectIdsSize + sceneObjectTransformsSize +
				morphChannelSize + morphShapeSize);

			Layers = (AnimationStateLayer*)data;
			memcpy(Layers, tempLayers.data(), layersSize);
			data += layersSize;

			AnimationState* states = (AnimationState*)data;
			for(UINT32 i = 0; i < numClips; i++)
				new (&states[i]) AnimationState();

			data += clipsSize;

			AnimationCurveMapping* boneMappings = (AnimationCurveMapping*)data;
			for (UINT32 i = 0; i < numBoneMappings; i++)
				new (&boneMappings[i]) AnimationCurveMapping();

			data += boneMappingSize;

			TCurveCache<Vector3>* posCache = (TCurveCache<Vector3>*)data;
			for (UINT32 i = 0; i < numPosCurves; i++)
				new (&posCache[i]) TCurveCache<Vector3>();

			data += posCacheSize;

			TCurveCache<Quaternion>* rotCache = (TCurveCache<Quaternion>*)data;
			for (UINT32 i = 0; i < numRotCurves; i++)
				new (&rotCache[i]) TCurveCache<Quaternion>();

			data += rotCacheSize;

			TCurveCache<Vector3>* scaleCache = (TCurveCache<Vector3>*)data;
			for (UINT32 i = 0; i < numScaleCurves; i++)
				new (&scaleCache[i]) TCurveCache<Vector3>();

			data += scaleCacheSize;

			TCurveCache<float>* genCache = (TCurveCache<float>*)data;
			for (UINT32 i = 0; i < NumGenericCurves; i++)
				new (&genCache[i]) TCurveCache<float>();

			data += genCacheSize;

			GenericCurveOutputs = (float*)data;
			data += genericCurveOutputSize;

			SceneObjectInfos = (AnimatedSceneObjectInfo*)data;
			data += sceneObjectIdsSize;

			SceneObjectTransforms = (Matrix4*)data;
			for (UINT32 i = 0; i < numBoneMappedSOs; i++)
				SceneObjectTransforms[i] = Matrix4::IDENTITY;

			data += sceneObjectTransformsSize;

			MorphChannelInfos = (MorphChannelInfo*)data;
			data += morphChannelSize;

			MorphShapeInfos = (MorphShapeInfo*)data;
			data += morphShapeSize;

			// Generate data required for morph shape animation
			if (morphShapes != nullptr)
			{
				UINT32 currentShapeIdx = 0;
				for (UINT32 i = 0; i < NumMorphChannels; i++)
				{
					SPtr<MorphChannel> morphChannel = morphShapes->GetChannel(i);
					UINT32 numShapes = morphChannel->GetNumShapes();

					MorphChannelInfo& channelInfo = MorphChannelInfos[i];
					channelInfo.Weight = 0.0f;
					channelInfo.ShapeStart = currentShapeIdx;
					channelInfo.ShapeCount = numShapes;
					channelInfo.FrameCurveIdx = (UINT32)-1;
					channelInfo.WeightCurveIdx = (UINT32)-1;

					for (UINT32 j = 0; j < numShapes; j++)
					{
						MorphShapeInfo& shapeInfo = MorphShapeInfos[currentShapeIdx];
						new (&shapeInfo.Shape) SPtr<MorphShape>();

						SPtr<MorphShape> shape = morphChannel->GetShape(j);
						shapeInfo.Shape = shape;
						shapeInfo.FrameWeight = shape->GetWeight();
						shapeInfo.FinalWeight = 0.0f;

						currentShapeIdx++;
					}
				}

				// Find any curves affecting morph shape animation
				if (!clipInfos.empty())
				{
					bool isClipValid = clipLoadState[0];
					if (isClipValid)
					{
						AnimationClipInfo& clipInfo = clipInfos[0];

						for (UINT32 i = 0; i < NumMorphChannels; i++)
						{
							SPtr<MorphChannel> morphChannel = morphShapes->GetChannel(i);
							MorphChannelInfo& channelInfo = MorphChannelInfos[i];

							clipInfo.Clip->GetMorphMapping(morphChannel->GetName(), channelInfo.FrameCurveIdx,
								channelInfo.WeightCurveIdx);
						}
					}
				}

				MorphChannelWeightsDirty = true;
			}

			UINT32 curLayerIdx = 0;
			UINT32 curStateIdx = 0;

			// Note: Hidden dependency. First clip info must be in layers[0].states[0] (needed for generic curves which only
			// use the primary clip).
			for(UINT32 i = 0; i < NumLayers; i++)
			{
				AnimationStateLayer& layer = Layers[i];

				layer.States = &states[curStateIdx];
				layer.NumStates = 0;

				UINT32 localStateIdx = 0;
				for(UINT32 j = 0; j < (UINT32)clipInfos.size(); j++)
				{
					AnimationClipInfo& clipInfo = clipInfos[j];

					UINT32 clipLayer = clipInfo.State.Layer;
					if (clipLayer == (UINT32)-1)
						clipLayer = 0;
					else
						clipLayer += 1;

					if (clipLayer != layer.Index)
						continue;

					AnimationState& state = states[curStateIdx];
					state.Loop = clipInfo.State.WrapMode == AnimWrapMode::Loop;

					// Calculate weight if fading is active
					float weight = clipInfo.State.Weight;

					//// Assumes time is clamped to [0, fadeLength] and fadeLength != 0
					if(clipInfo.FadeDirection < 0.0f)
					{
						float t = clipInfo.FadeTime / clipInfo.FadeLength;
						weight *= (1.0f - t);
					}
					else if(clipInfo.FadeDirection > 0.0f)
					{
						float t = clipInfo.FadeTime / clipInfo.FadeLength;
						weight *= t;
					}

					state.Weight = weight;

					// Set up individual curves and their caches
					bool isClipValid = clipLoadState[j];
					if (isClipValid)
					{
						state.Curves = clipInfo.Clip->GetCurves();
						state.Length = clipInfo.Clip->GetLength();
						state.Disabled = clipInfo.PlaybackType == AnimPlaybackType::None;
					}
					else
					{
						static SPtr<AnimationCurves> zeroCurves = bs_shared_ptr_new<AnimationCurves>();
						state.Curves = zeroCurves;
						state.Length = 0.0f;
						state.Disabled = true;
					}

					// Wrap time if looping
					if (state.Loop && state.Length > 0.0f)
						state.Time = Math::Repeat(clipInfo.State.Time, state.Length);
					else
						state.Time = clipInfo.State.Time;

					state.PositionCaches = posCache;
					posCache += state.Curves->Position.size();

					state.RotationCaches = rotCache;
					rotCache += state.Curves->Rotation.size();

					state.ScaleCaches = scaleCache;
					scaleCache += state.Curves->Scale.size();

					state.GenericCaches = genCache;
					genCache += state.Curves->Generic.size();

					clipInfo.LayerIdx = curLayerIdx;
					clipInfo.StateIdx = localStateIdx;

					if(isClipValid)
						clipInfo.CurveVersion = clipInfo.Clip->GetVersion();

					// Set up bone mapping
					if (Skeleton != nullptr)
					{
						state.BoneToCurveMapping = &boneMappings[curStateIdx * numBones];

						if (isClipValid)
						{
							clipInfo.Clip->GetBoneMapping(*Skeleton, state.BoneToCurveMapping);
						}
						else
						{
							AnimationCurveMapping emptyMapping = { (UINT32)-1, (UINT32)-1, (UINT32)-1 };

							for (UINT32 i = 0; i < numBones; i++)
								state.BoneToCurveMapping[i] = emptyMapping;
						}
					}
					else
						state.BoneToCurveMapping = nullptr;

					layer.NumStates++;
					curStateIdx++;
					localStateIdx++;
				}

				curLayerIdx++;

				// Must be larger than zero otherwise the layer.states pointer will point to data held by some other layer
				assert(layer.NumStates > 0);
			}

			Matrix4 invRootTransform(BsIdentity);
			for (UINT32 i = 0; i < NumSceneObjects; i++)
			{
				if(sceneObjects[i].CurveName.empty())
				{
					HSceneObject so = sceneObjects[i].So;
					if (!so.IsDestroyed(true))
						invRootTransform = so->GetWorldMatrix().InverseAffine();

					break;
				}
			}

			UINT32 boneIdx = 0;
			for(UINT32 i = 0; i < NumSceneObjects; i++)
			{
				HSceneObject so = sceneObjects[i].So;
				AnimatedSceneObjectInfo& soInfo = SceneObjectInfos[i];
				soInfo.Id = so.GetInstanceId();
				soInfo.BoneIdx = mappedBoneIndices[i];

				bool isSOValid = !so.IsDestroyed(true);
				if (isSOValid)
					soInfo.Hash = so->GetTransformHash();
				else
					soInfo.Hash = 0;

				soInfo.LayerIdx = (UINT32)-1;
				soInfo.StateIdx = (UINT32)-1;

				// If no bone mapping, find curves directly
				if(soInfo.BoneIdx == -1)
				{
					soInfo.CurveIndices = { (UINT32)-1, (UINT32)-1, (UINT32)-1 };

					if (isSOValid)
					{
						for (UINT32 j = 0; j < (UINT32)clipInfos.size(); j++)
						{
							AnimationClipInfo& clipInfo = clipInfos[j];

							soInfo.LayerIdx = clipInfo.LayerIdx;
							soInfo.StateIdx = clipInfo.StateIdx;

							bool isClipValid = clipLoadState[j];
							if (isClipValid)
							{
								// Note: If there are multiple clips with the relevant curve name, we only use the first

								clipInfo.Clip->GetCurveMapping(sceneObjects[i].CurveName, soInfo.CurveIndices);
								break;
							}
						}
					}
				}
				else
				{
					// No need to check if SO is valid, if it has a bone connection it must be
					SceneObjectTransforms[boneIdx] = so->GetWorldMatrix() * invRootTransform;
					boneIdx++;
				}
			}

			bs_frame_free(mappedBoneIndices);
		}
		bs_frame_clear();
	}

	void AnimationProxy::UpdateClipInfos(const Vector<AnimationClipInfo>& clipInfos)
	{
		for(auto& clipInfo : clipInfos)
		{
			AnimationState& state = Layers[clipInfo.LayerIdx].States[clipInfo.StateIdx];

			state.Loop = clipInfo.State.WrapMode == AnimWrapMode::Loop;
			state.Weight = clipInfo.State.Weight;

			// Wrap time if looping
			if (state.Loop && state.Length > 0.0f)
				state.Time = Math::Repeat(clipInfo.State.Time, state.Length);
			else
				state.Time = clipInfo.State.Time;

			bool isLoaded = clipInfo.Clip.IsLoaded();
			state.Disabled = !isLoaded || clipInfo.PlaybackType == AnimPlaybackType::None;
		}
	}

	void AnimationProxy::UpdateMorphChannelWeights(const Vector<float>& weights)
	{
		UINT32 numWeights = (UINT32)weights.size();
		for(UINT32 i = 0; i < NumMorphChannels; i++)
		{
			if (i < numWeights)
				MorphChannelInfos[i].Weight = weights[i];
			else
				MorphChannelInfos[i].Weight = 0.0f;
		}

		MorphChannelWeightsDirty = true;
	}

	void AnimationProxy::UpdateTransforms(const Vector<AnimatedSceneObject>& sceneObjects)
	{
		Matrix4 invRootTransform(BsIdentity);
		for (UINT32 i = 0; i < NumSceneObjects; i++)
		{
			if (sceneObjects[i].CurveName.empty())
			{
				HSceneObject so = sceneObjects[i].So;
				if (!so.IsDestroyed(true))
					invRootTransform = so->GetWorldMatrix().InverseAffine();

				break;
			}
		}

		UINT32 boneIdx = 0;
		for (UINT32 i = 0; i < NumSceneObjects; i++)
		{
			HSceneObject so = sceneObjects[i].So;
			if (so.IsDestroyed(true))
			{
				SceneObjectInfos[i].Hash = 0;
				continue;
			}

			SceneObjectInfos[i].Hash = so->GetTransformHash();

			if (SceneObjectInfos[i].BoneIdx == -1)
				continue;

			SceneObjectTransforms[boneIdx] = sceneObjects[i].So->GetWorldMatrix() * invRootTransform;
			boneIdx++;
		}
	}

	void AnimationProxy::UpdateTime(const Vector<AnimationClipInfo>& clipInfos)
	{
		for (auto& clipInfo : clipInfos)
		{
			AnimationState& state = Layers[clipInfo.LayerIdx].States[clipInfo.StateIdx];

			// Wrap time if looping
			if (state.Loop && state.Length > 0.0f)
				state.Time = Math::Repeat(clipInfo.State.Time, state.Length);
			else
				state.Time = clipInfo.State.Time;

			bool isLoaded = clipInfo.Clip.IsLoaded();
			state.Disabled = !isLoaded || clipInfo.PlaybackType == AnimPlaybackType::None;
		}
	}

	Animation::Animation()
	{
		mId = AnimationManager::Instance().RegisterAnimation(this);
		mAnimProxy = bs_shared_ptr_new<AnimationProxy>(mId);
	}

	Animation::~Animation()
	{
		AnimationManager::Instance().UnregisterAnimation(mId);
	}

	void Animation::SetSkeleton(const SPtr<Skeleton>& skeleton)
	{
		mSkeleton = skeleton;
		mDirty |= AnimDirtyStateFlag::All;
	}

	void Animation::SetMorphShapes(const SPtr<MorphShapes>& morphShapes)
	{
		mMorphShapes = morphShapes;

		UINT32 numChannels;
		if (mMorphShapes != nullptr)
			numChannels = mMorphShapes->GetNumChannels();
		else
			numChannels = 0;

		mMorphChannelWeights.assign(numChannels, 0.0f);
		if (numChannels > 0)
			mMorphChannelWeights[0] = 1.0f;

		mDirty |= AnimDirtyStateFlag::Layout;
		mDirty |= AnimDirtyStateFlag::MorphWeights;
	}

	void Animation::SetMorphChannelWeight(UINT32 idx, float weight)
	{
		UINT32 numShapes = (UINT32)mMorphChannelWeights.size();
		if (idx >= numShapes)
			return;

		mMorphChannelWeights[idx] = weight;
		mDirty |= AnimDirtyStateFlag::MorphWeights;
	}

	void Animation::SetMask(const SkeletonMask& mask)
	{
		mSkeletonMask = mask;
		mDirty |= AnimDirtyStateFlag::All;
	}

	void Animation::SetWrapMode(AnimWrapMode wrapMode)
	{
		mDefaultWrapMode = wrapMode;

		for (auto& clipInfo : mClipInfos)
			clipInfo.State.WrapMode = wrapMode;

		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::SetSpeed(float speed)
	{
		mDefaultSpeed = speed;

		for (auto& clipInfo : mClipInfos)
		{
			// Special case: Ignore non-moving ones
			if(!clipInfo.State.Stopped)
				clipInfo.State.Speed = speed;
		}

		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::SetBounds(const AABox& bounds)
	{
		mBounds = bounds;

		mDirty |= AnimDirtyStateFlag::Culling;
	}

	void Animation::SetCulling(bool cull)
	{
		mCull = cull;

		mDirty |= AnimDirtyStateFlag::Culling;
	}

	void Animation::Play(const HAnimationClip& clip)
	{
		AnimationClipInfo* clipInfo = AddClip(clip, (UINT32)-1);
		if(clipInfo != nullptr)
		{
			clipInfo->State.Time = 0.0f;
			clipInfo->State.Speed = mDefaultSpeed;
			clipInfo->State.Weight = 1.0f;
			clipInfo->State.WrapMode = mDefaultWrapMode;
			clipInfo->PlaybackType = AnimPlaybackType::Normal;
		}

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::BlendAdditive(const HAnimationClip& clip, float weight, float fadeLength, UINT32 layer)
	{
		if(clip != nullptr && !clip->IsAdditive())
		{
			BS_LOG(Warning, Renderer,
				"blendAdditive() called with a clip that doesn't contain additive animation. Ignoring.");

			// Stop any clips on this layer, even if invalid
			HAnimationClip nullClip;
			AddClip(nullClip, layer);

			mSampleStep = AnimSampleStep::None;
			return;
		}

		AnimationClipInfo* clipInfo = AddClip(clip, layer);
		if (clipInfo != nullptr)
		{
			clipInfo->State.Time = 0.0f;
			clipInfo->State.Speed = mDefaultSpeed;
			clipInfo->State.Weight = weight;
			clipInfo->State.WrapMode = mDefaultWrapMode;

			if(fadeLength > 0.0f)
			{
				clipInfo->FadeDirection = 1.0f;
				clipInfo->FadeTime = 0.0f;
				clipInfo->FadeLength = fadeLength;
			}

			clipInfo->PlaybackType = AnimPlaybackType::Normal;

			mSampleStep = AnimSampleStep::None;
			mDirty |= AnimDirtyStateFlag::Value;
		}
	}

	void Animation::Blend1D(const Blend1DInfo& info, float t)
	{
		if (info.Clips.size() == 0)
			return;

		// Find valid range
		float startPos = 0.0f;
		float endPos = 0.0f;

		for (UINT32 i = 0; i < (UINT32)info.Clips.size(); i++)
		{
			startPos = std::min(startPos, info.Clips[i].Position);
			endPos = std::min(endPos, info.Clips[i].Position);
		}

		float length = endPos - startPos;
		if(Math::ApproxEquals(length, 0.0f) || info.Clips.size() < 2)
		{
			Play(info.Clips[0].Clip);
			return;
		}

		// Clamp or loop time
		bool loop = mDefaultWrapMode == AnimWrapMode::Loop;
		if (t < startPos)
		{
			if (loop)
				t = t - std::floor(t / length) * length;
			else // Clamping
				t = startPos;
		}

		if (t > endPos)
		{
			if (loop)
				t = t - std::floor(t / length) * length;
			else // Clamping
				t = endPos;
		}

		// Find keys to blend between
		UINT32 leftKey = 0;
		UINT32 rightKey = 0;

		INT32 start = 0;
		INT32 searchLength = (INT32)info.Clips.size();

		while (searchLength > 0)
		{
			INT32 half = searchLength >> 1;
			INT32 mid = start + half;

			if (t < info.Clips[mid].Position)
			{
				searchLength = half;
			}
			else
			{
				start = mid + 1;
				searchLength -= (half + 1);
			}
		}

		leftKey = std::max(0, start - 1);
		rightKey = std::min(start, (INT32)info.Clips.size() - 1);

		float interpLength = info.Clips[rightKey].Position - info.Clips[leftKey].Position;
		t = (t - info.Clips[leftKey].Position) / interpLength;

		// Add clips and set weights
		for(UINT32 i = 0; i < (UINT32)info.Clips.size(); i++)
		{
			AnimationClipInfo* clipInfo = AddClip(info.Clips[i].Clip, (UINT32)-1, i == 0);
			if (clipInfo != nullptr)
			{
				clipInfo->State.Time = 0.0f;
				clipInfo->State.Stopped = true;
				clipInfo->State.Speed = 0.0f;
				clipInfo->State.WrapMode = AnimWrapMode::Clamp;

				if (i == leftKey)
					clipInfo->State.Weight = 1.0f - t;
				else if (i == rightKey)
					clipInfo->State.Weight = t;
				else
					clipInfo->State.Weight = 0.0f;

				clipInfo->PlaybackType = AnimPlaybackType::Normal;
			}
		}

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::Blend2D(const Blend2DInfo& info, const Vector2& t)
	{
		AnimationClipInfo* topLeftClipInfo = AddClip(info.TopLeftClip, (UINT32)-1, true);
		if (topLeftClipInfo != nullptr)
		{
			topLeftClipInfo->State.Time = 0.0f;
			topLeftClipInfo->State.Stopped = true;
			topLeftClipInfo->State.Speed = 0.0f;
			topLeftClipInfo->State.Weight = (1.0f - t.X) * (1.0f - t.Y);
			topLeftClipInfo->State.WrapMode = AnimWrapMode::Clamp;

			topLeftClipInfo->PlaybackType = AnimPlaybackType::Normal;
		}

		AnimationClipInfo* topRightClipInfo = AddClip(info.TopRightClip, (UINT32)-1, false);
		if (topRightClipInfo != nullptr)
		{
			topRightClipInfo->State.Time = 0.0f;
			topRightClipInfo->State.Stopped = true;
			topRightClipInfo->State.Speed = 0.0f;
			topRightClipInfo->State.Weight = t.X * (1.0f - t.Y);
			topRightClipInfo->State.WrapMode = AnimWrapMode::Clamp;

			topRightClipInfo->PlaybackType = AnimPlaybackType::Normal;
		}

		AnimationClipInfo* botLeftClipInfo = AddClip(info.BotLeftClip, (UINT32)-1, false);
		if (botLeftClipInfo != nullptr)
		{
			botLeftClipInfo->State.Time = 0.0f;
			botLeftClipInfo->State.Stopped = true;
			botLeftClipInfo->State.Speed = 0.0f;
			botLeftClipInfo->State.Weight = (1.0f - t.X) * t.Y;
			botLeftClipInfo->State.WrapMode = AnimWrapMode::Clamp;

			botLeftClipInfo->PlaybackType = AnimPlaybackType::Normal;
		}

		AnimationClipInfo* botRightClipInfo = AddClip(info.BotRightClip, (UINT32)-1, false);
		if (botRightClipInfo != nullptr)
		{
			botRightClipInfo->State.Time = 0.0f;
			botRightClipInfo->State.Stopped = true;
			botRightClipInfo->State.Speed = 0.0f;
			botRightClipInfo->State.Weight = t.X * t.Y;
			botRightClipInfo->State.WrapMode = AnimWrapMode::Clamp;

			botRightClipInfo->PlaybackType = AnimPlaybackType::Normal;
		}

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::CrossFade(const HAnimationClip& clip, float fadeLength)
	{
		bool isFading = fadeLength > 0.0f;
		if(!isFading)
		{
			Play(clip);
			return;
		}

		AnimationClipInfo* clipInfo = AddClip(clip, (UINT32)-1, false);
		if (clipInfo != nullptr)
		{
			clipInfo->State.Time = 0.0f;
			clipInfo->State.Speed = mDefaultSpeed;
			clipInfo->State.Weight = 1.0f;
			clipInfo->State.WrapMode = mDefaultWrapMode;
			clipInfo->PlaybackType = AnimPlaybackType::Normal;

			// Set up fade lengths
			clipInfo->FadeDirection = 1.0f;
			clipInfo->FadeTime = 0.0f;
			clipInfo->FadeLength = fadeLength;

			for (auto& entry : mClipInfos)
			{
				if (entry.State.Layer == (UINT32)-1 && entry.Clip != clip)
				{
					// If other clips are already cross-fading, we need to persist their current weight before starting
					// a new crossfade. We do that by adjusting the fade times.
					if(clipInfo->FadeDirection != 0 && clipInfo->FadeTime < clipInfo->FadeLength)
					{
						float t = clipInfo->FadeTime / clipInfo->FadeLength;
						if (clipInfo->FadeDirection < 0.0f)
							t = (1.0f - t);

						clipInfo->State.Weight *= t;
					}

					clipInfo->FadeDirection = -1.0f;
					clipInfo->FadeTime = 0.0f;
					clipInfo->FadeLength = fadeLength;
				}
			}
		}

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::Sample(const HAnimationClip& clip, float time)
	{
		AnimationClipInfo* clipInfo = AddClip(clip, (UINT32)-1);
		if (clipInfo != nullptr)
		{
			clipInfo->State.Time = time;
			clipInfo->State.Speed = 0.0f;
			clipInfo->State.Weight = 1.0f;
			clipInfo->State.WrapMode = mDefaultWrapMode;
			clipInfo->PlaybackType = AnimPlaybackType::Sampled;
		}

		mSampleStep = AnimSampleStep::Frame;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	void Animation::Stop(UINT32 layer)
	{
		bs_frame_mark();
		{
			FrameVector<AnimationClipInfo> newClips;
			for (auto& clipInfo : mClipInfos)
			{
				if (clipInfo.State.Layer != layer)
					newClips.push_back(clipInfo);
				else
					mDirty |= AnimDirtyStateFlag::Layout;
			}

			mClipInfos.resize(newClips.size());
			for(UINT32 i = 0; i < (UINT32)newClips.size(); i++)
				mClipInfos[i] = newClips[i];
		}
		bs_frame_clear();
	}

	void Animation::StopAll()
	{
		mClipInfos.clear();

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Layout;
	}

	AnimationClipInfo* Animation::AddClip(const HAnimationClip& clip, UINT32 layer, bool stopExisting)
	{
		AnimationClipInfo* output = nullptr;
		bool hasExisting = false;

		// Search for existing
		for (auto& clipInfo : mClipInfos)
		{
			if (clipInfo.State.Layer == layer)
			{
				if (clipInfo.Clip == clip)
					output = &clipInfo;
				else if (stopExisting)
					hasExisting = true;
			}
		}

		// Doesn't exist or found extra animations, rebuild
		if (output == nullptr || hasExisting)
		{
			bs_frame_mark();
			{
				FrameVector<AnimationClipInfo> newClips;
				for (auto& clipInfo : mClipInfos)
				{
					if (!stopExisting || clipInfo.State.Layer != layer || clipInfo.Clip == clip)
						newClips.push_back(clipInfo);
				}

				if (output == nullptr && clip != nullptr)
					newClips.push_back(AnimationClipInfo());

				mClipInfos.resize(newClips.size());
				for(UINT32 i = 0; i < (UINT32)newClips.size(); i++)
					mClipInfos[i] = newClips[i];

				mDirty |= AnimDirtyStateFlag::Layout;
			}
			bs_frame_clear();
		}

		// If new clip was added, get its address
		if (output == nullptr && clip != nullptr)
		{
			AnimationClipInfo& newInfo = mClipInfos.back();
			newInfo.Clip = clip;
			newInfo.State.Layer = layer;

			output = &newInfo;
		}

		return output;
	}

	bool Animation::GetAnimatesRootInternal() const
	{
		if (mSkeleton == nullptr)
			return false;

		UINT32 rootBoneIdx = mSkeleton->GetRootBoneIndex();
		if (rootBoneIdx == (UINT32)-1)
			return false;

		String rootBoneName = mSkeleton->GetBoneInfo(rootBoneIdx).Name;
		for (auto& entry : mClipInfos)
		{
			if (entry.Clip.IsLoaded())
			{
				HAnimationClip clip = entry.Clip;
				if(!clip->HasRootMotion())
				{
					AnimationCurveMapping mapping;
					clip->GetCurveMapping(rootBoneName, mapping);

					if (mapping.Position != (UINT32)-1)
						return true;

					if (mapping.Rotation != (UINT32)-1)
						return true;

					if (mapping.Scale != (UINT32)-1)
						return true;
				}
			}
		}

		return false;
	}

	void Animation::GetListenerResources(Vector<HResource>& resources)
	{
		for (auto& entry : mClipInfos)
		{
			if(entry.Clip != nullptr)
				resources.push_back(entry.Clip);
		}
	}

	void Animation::NotifyResourceLoaded(const HResource& resource)
	{
		mDirty |= AnimDirtyStateFlag::Layout;
	}

	void Animation::NotifyResourceChanged(const HResource& resource)
	{
		mDirty |= AnimDirtyStateFlag::Layout;
	}

	bool Animation::IsPlaying() const
	{
		for(auto& clipInfo : mClipInfos)
		{
			if (clipInfo.Clip.IsLoaded())
				return true;
		}

		return false;
	}

	bool Animation::GetState(const HAnimationClip& clip, AnimationClipState& state)
	{
		if (clip == nullptr)
			return false;

		for (auto& clipInfo : mClipInfos)
		{
			if (clipInfo.Clip == clip)
			{
				state = clipInfo.State;

				if (state.Layer == (UINT32)-1)
					state.Layer = 0;
				else
					state.Layer += 1;

				// Internally we store unclamped time, so clamp/loop it
				float clipLength = 0.0f;
				if (clip.IsLoaded())
					clipLength = clip->GetLength();

				bool loop = clipInfo.State.WrapMode == AnimWrapMode::Loop;
				AnimationUtility::WrapTime(clipInfo.State.Time, 0.0f, clipLength, loop);

				return true;
			}
		}

		return false;
	}

	void Animation::SetState(const HAnimationClip& clip, AnimationClipState state)
	{
		if (state.Layer == 0)
			state.Layer = (UINT32)-1;
		else
			state.Layer -= 1;

		AnimationClipInfo* clipInfo = AddClip(clip, state.Layer, false);

		if (clipInfo == nullptr)
			return;

		clipInfo->State = state;
		clipInfo->PlaybackType = AnimPlaybackType::Normal;

		mSampleStep = AnimSampleStep::None;
		mDirty |= AnimDirtyStateFlag::Value;
	}

	UINT32 Animation::GetNumClips() const
	{
		return (UINT32)mClipInfos.size();
	}

	HAnimationClip Animation::GetClip(UINT32 idx) const
	{
		if (idx >= (UINT32)mClipInfos.size())
			return HAnimationClip();

		return mClipInfos[idx].Clip;
	}

	void Animation::TriggerEvents(float delta)
	{
		for (auto& clipInfo : mClipInfos)
		{
			if (!clipInfo.Clip.IsLoaded())
				continue;

			const Vector<AnimationEvent>& events = clipInfo.Clip->GetEvents();
			bool loop = clipInfo.State.WrapMode == AnimWrapMode::Loop;

			float start = std::max(clipInfo.State.Time - delta, 0.0f);
			float end = clipInfo.State.Time;
			float clipLength = clipInfo.Clip->GetLength();

			float wrappedStart = start;
			float wrappedEnd = end;
			AnimationUtility::WrapTime(wrappedStart, 0.0f, clipLength, loop);
			AnimationUtility::WrapTime(wrappedEnd, 0.0f, clipLength, loop);

			if(!loop)
			{
				for (auto& event : events)
				{
					if (event.Time >= wrappedStart && (event.Time < wrappedEnd ||
						(event.Time == clipLength && start < clipLength && end >= clipLength)))
						OnEventTriggered(clipInfo.Clip, event.Name);
				}
			}
			else
			{
				if (wrappedStart < wrappedEnd)
				{
					for (auto& event : events)
					{
						if (event.Time >= wrappedStart && event.Time < wrappedEnd)
							OnEventTriggered(clipInfo.Clip, event.Name);
					}
				}
				else if (wrappedEnd < wrappedStart) // End is looped, but start is not
				{
					for (auto& event : events)
					{
						if ((event.Time >= wrappedStart && event.Time <= clipLength) || (event.Time >= 0 && event.Time < wrappedEnd))
							OnEventTriggered(clipInfo.Clip, event.Name);
					}
				}
			}
		}
	}

	void Animation::MapCurveToSceneObject(const String& curve, const HSceneObject& so)
	{
		AnimatedSceneObject animSo = { so, curve };
		mSceneObjects[so.GetInstanceId()] = animSo;

		mDirty |= AnimDirtyStateFlag::All;
	}

	void Animation::UnmapSceneObject(const HSceneObject& so)
	{
		mSceneObjects.erase(so.GetInstanceId());

		mDirty |= AnimDirtyStateFlag::All;
	}

	bool Animation::GetGenericCurveValue(UINT32 curveIdx, float& value)
	{
		if (!mGenericCurveValuesValid || curveIdx >= (UINT32)mGenericCurveOutputs.size())
			return false;

		value = mGenericCurveOutputs[curveIdx];
		return true;
	}

	SPtr<Animation> Animation::Create()
	{
		Animation* anim = new (bs_alloc<Animation>()) Animation();

		SPtr<Animation> animPtr = bs_core_ptr(anim);
		animPtr->SetThisPtrInternal(animPtr);
		animPtr->Initialize();

		return animPtr;
	}

	void Animation::UpdateAnimProxy(float timeDelta)
	{
		// Check if any of the clip curves are dirty and advance time, perform fading
		for (auto& clipInfo : mClipInfos)
		{
			float scaledTimeDelta = timeDelta * clipInfo.State.Speed;
			clipInfo.State.Time += scaledTimeDelta;

			HAnimationClip clip = clipInfo.Clip;
			if (clip.IsLoaded())
			{
				if (clipInfo.CurveVersion != clip->GetVersion())
					mDirty |= AnimDirtyStateFlag::Layout;
			}

			float fadeTime = clipInfo.FadeTime + scaledTimeDelta;
			clipInfo.FadeTime = Math::Clamp(fadeTime, 0.0f, clipInfo.FadeLength);
		}

		if(mSampleStep == AnimSampleStep::None)
			mAnimProxy->SampleStep = AnimSampleStep::None;
		else if(mSampleStep == AnimSampleStep::Frame)
		{
			if(mAnimProxy->SampleStep == AnimSampleStep::None)
				mAnimProxy->SampleStep = AnimSampleStep::Frame;
			else
				mAnimProxy->SampleStep = AnimSampleStep::Done;
		}

		if (mDirty.IsSet(AnimDirtyStateFlag::Culling))
		{
			mAnimProxy->MCullEnabled = mCull;
			mAnimProxy->MBounds = mBounds;

			mDirty.Unset(AnimDirtyStateFlag::Culling);
		}

		auto getAnimatedSOList = [&]()
		{
			Vector<AnimatedSceneObject> animatedSO(mSceneObjects.size());
			UINT32 idx = 0;
			for (auto& entry : mSceneObjects)
				animatedSO[idx++] = entry.second;

			return animatedSO;
		};

		bool didFullRebuild = false;
		if((UINT32)mDirty == 0) // Clean
		{
			mAnimProxy->UpdateTime(mClipInfos);
		}
		else
		{
			if (mDirty.IsSet(AnimDirtyStateFlag::All))
			{
				Vector<AnimatedSceneObject> animatedSOs = getAnimatedSOList();

				mAnimProxy->Rebuild(mSkeleton, mSkeletonMask, mClipInfos, animatedSOs, mMorphShapes);
				didFullRebuild = true;
			}
			else if (mDirty.IsSet(AnimDirtyStateFlag::Layout))
			{
				Vector<AnimatedSceneObject> animatedSOs = getAnimatedSOList();

				mAnimProxy->Rebuild(mClipInfos, animatedSOs, mMorphShapes);
				didFullRebuild = true;
			}
			else if(mDirty.IsSet(AnimDirtyStateFlag::Value))
				mAnimProxy->UpdateClipInfos(mClipInfos);

			if (mDirty.IsSet(AnimDirtyStateFlag::MorphWeights) || didFullRebuild)
				mAnimProxy->UpdateMorphChannelWeights(mMorphChannelWeights);
		}

		// Check if there are dirty transforms
		if (!didFullRebuild)
		{
			for (UINT32 i = 0; i < mAnimProxy->NumSceneObjects; i++)
			{
				AnimatedSceneObjectInfo& soInfo = mAnimProxy->SceneObjectInfos[i];

				auto iterFind = mSceneObjects.find(soInfo.Id);
				if (iterFind == mSceneObjects.end())
				{
					assert(false); // Should never happen
					continue;
				}

				UINT32 hash;

				HSceneObject so = iterFind->second.So;
				if (so.IsDestroyed(true))
					hash = 0;
				else
					hash = so->GetTransformHash();

				if (hash != mAnimProxy->SceneObjectInfos[i].Hash)
				{
					Vector<AnimatedSceneObject> animatedSOs = getAnimatedSOList();
					mAnimProxy->UpdateTransforms(animatedSOs);
					break;
				}
			}
		}

		mDirty = AnimDirtyState();
	}

	void Animation::UpdateFromProxy()
	{
		// When sampling a single frame we don't want to keep updating the scene objects so they can be moved through other
		// means (e.g. for the purposes of recording new keyframes if running from the editor).
		const bool disableSOUpdates = mAnimProxy->SampleStep == AnimSampleStep::Done;
		if(disableSOUpdates)
			return;

		// If the object was culled, then we have no valid data to read back
		if(mAnimProxy->WasCulled)
			return;

		HSceneObject rootSO;

		// Write TRS animation results to relevant SceneObjects
		for(UINT32 i = 0; i < mAnimProxy->NumSceneObjects; i++)
		{
			AnimatedSceneObjectInfo& soInfo = mAnimProxy->SceneObjectInfos[i];

			auto iterFind = mSceneObjects.find(soInfo.Id);
			if (iterFind == mSceneObjects.end())
				continue;

			HSceneObject so = iterFind->second.So;
			if (iterFind->second.CurveName.empty())
				rootSO = so;

			if (so.IsDestroyed(true))
				continue;

			if(soInfo.BoneIdx != -1)
			{
				if (mAnimProxy->SkeletonPose.HasOverride[soInfo.BoneIdx])
					continue;

				Vector3 position = mAnimProxy->SkeletonPose.Positions[soInfo.BoneIdx];
				Quaternion rotation = mAnimProxy->SkeletonPose.Rotations[soInfo.BoneIdx];
				Vector3 scale = mAnimProxy->SkeletonPose.Scales[soInfo.BoneIdx];

				const SPtr<Skeleton>& skeleton = mAnimProxy->Skeleton;

				UINT32 parentBoneIdx = skeleton->GetBoneInfo(soInfo.BoneIdx).Parent;
				if (parentBoneIdx == (UINT32)-1)
				{
					so->SetPosition(position);
					so->SetRotation(rotation);
					so->SetScale(scale);
				}
				else
				{
					while(parentBoneIdx != (UINT32)-1)
					{
						// Update rotation
						const Quaternion& parentOrientation = mAnimProxy->SkeletonPose.Rotations[parentBoneIdx];
						rotation = parentOrientation * rotation;

						// Update scale
						const Vector3& parentScale = mAnimProxy->SkeletonPose.Scales[parentBoneIdx];
						scale = parentScale * scale;

						// Update position
						position = parentOrientation.Rotate(parentScale * position);
						position += mAnimProxy->SkeletonPose.Positions[parentBoneIdx];

						parentBoneIdx = skeleton->GetBoneInfo(parentBoneIdx).Parent;
					}

					// Search for root if not already found
					if(rootSO == nullptr)
					{
						for(auto& entry : mSceneObjects)
						{
							if (entry.second.CurveName.empty())
								rootSO = entry.second.So;
						}
					}

					while(rootSO && rootSO.IsDestroyed(true))
						rootSO = rootSO->GetParent();

					Vector3 parentPos = Vector3::ZERO;
					Quaternion parentRot = Quaternion::IDENTITY;
					Vector3 parentScale = Vector3::ONE;

					if(!rootSO.IsDestroyed(true))
					{
						const Transform& tfrm = rootSO->GetTransform();
						parentPos = tfrm.GetPosition();
						parentRot = tfrm.GetRotation();
						parentScale = tfrm.GetScale();
					}

					// Transform from space relative to root's parent to world space
					rotation = parentRot * rotation;

					scale = parentScale * scale;

					position = parentRot.Rotate(parentScale * position);
					position += parentPos;

					so->SetWorldPosition(position);
					so->SetWorldRotation(rotation);
					so->SetWorldScale(scale);
				}
			}
			else
			{
				if (!mAnimProxy->SceneObjectPose.HasOverride[i * 3 + 0])
					so->SetPosition(mAnimProxy->SceneObjectPose.Positions[i]);

				if (!mAnimProxy->SceneObjectPose.HasOverride[i * 3 + 1])
					so->SetRotation(mAnimProxy->SceneObjectPose.Rotations[i]);

				if (!mAnimProxy->SceneObjectPose.HasOverride[i * 3 + 2])
					so->SetScale(mAnimProxy->SceneObjectPose.Scales[i]);
			}
		}

		// Must ensure that clip in the proxy and current primary clip are the same
		mGenericCurveValuesValid = false;
		if(mAnimProxy->NumLayers > 0 && mAnimProxy->Layers[0].NumStates > 0)
		{
			const AnimationState& state = mAnimProxy->Layers[0].States[0];

			if(!state.Disabled && mClipInfos.size() > 0)
			{
				const AnimationClipInfo& clipInfo = mClipInfos[0];

				if (clipInfo.StateIdx == 0 && clipInfo.LayerIdx == 0)
				{
					if (clipInfo.Clip.IsLoaded() && clipInfo.CurveVersion == clipInfo.Clip->GetVersion())
					{
						UINT32 numGenericCurves = (UINT32)clipInfo.Clip->GetCurves()->Generic.size();
						mGenericCurveValuesValid = numGenericCurves == mAnimProxy->NumGenericCurves;
					}
				}
			}
		}

		if(mGenericCurveValuesValid)
		{
			mGenericCurveOutputs.resize(mAnimProxy->NumGenericCurves);

			memcpy(mGenericCurveOutputs.data(), mAnimProxy->GenericCurveOutputs, mAnimProxy->NumGenericCurves * sizeof(float));
		}
	}
}
