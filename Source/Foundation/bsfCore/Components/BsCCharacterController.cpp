//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCCharacterController.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsSceneManager.h"
#include "Physics/BsCollider.h"
#include "Private/RTTI/BsCCharacterControllerRTTI.h"
#include "BsCCollider.h"

using namespace std::placeholders;

namespace bs
{
	CCharacterController::CCharacterController()
	{
		SetName("CharacterController");

		mNotifyFlags = TCF_Transform;
	}

	CCharacterController::CCharacterController(const HSceneObject& parent)
		: Component(parent)
	{
		SetName("CharacterController");

		mNotifyFlags = TCF_Transform;
	}

	CharacterCollisionFlags CCharacterController::Move(const Vector3& displacement)
	{
		CharacterCollisionFlags output;

		if (mInternal == nullptr)
			return output;

		output = mInternal->Move(displacement);
		UpdatePositionFromController();

		return output;
	}

	Vector3 CCharacterController::GetFootPosition() const
	{
		if (mInternal == nullptr)
			return Vector3::ZERO;

		return mInternal->GetFootPosition();
	}

	void CCharacterController::SetFootPosition(const Vector3& position)
	{
		if (mInternal == nullptr)
			return;

		mInternal->SetFootPosition(position);
		UpdatePositionFromController();
	}

	void CCharacterController::SetRadius(float radius)
	{
		mDesc.radius = radius;

		if (mInternal != nullptr)
			UpdateDimensions();
	}

	void CCharacterController::SetHeight(float height)
	{
		mDesc.height = height;

		if (mInternal != nullptr)
			UpdateDimensions();
	}

	void CCharacterController::SetUp(const Vector3& up)
	{
		mDesc.up = up;

		if (mInternal != nullptr)
			mInternal->SetUp(up);
	}

	void CCharacterController::SetClimbingMode(CharacterClimbingMode mode)
	{
		mDesc.climbingMode = mode;

		if (mInternal != nullptr)
			mInternal->SetClimbingMode(mode);
	}

	void CCharacterController::SetNonWalkableMode(CharacterNonWalkableMode mode)
	{
		mDesc.nonWalkableMode = mode;

		if (mInternal != nullptr)
			mInternal->SetNonWalkableMode(mode);
	}

	void CCharacterController::SetMinMoveDistance(float value)
	{
		mDesc.minMoveDistance = value;

		if (mInternal != nullptr)
			mInternal->SetMinMoveDistance(value);
	}

	void CCharacterController::SetContactOffset(float value)
	{
		mDesc.contactOffset = value;

		if (mInternal != nullptr)
			mInternal->SetContactOffset(value);
	}

	void CCharacterController::SetStepOffset(float value)
	{
		mDesc.stepOffset = value;

		if (mInternal != nullptr)
			mInternal->SetStepOffset(value);
	}

	void CCharacterController::SetSlopeLimit(Radian value)
	{
		mDesc.slopeLimit = value;

		if (mInternal != nullptr)
			mInternal->SetSlopeLimit(value);
	}

	void CCharacterController::SetLayer(UINT64 layer)
	{
		mLayer = layer;

		if (mInternal != nullptr)
			mInternal->SetLayer(layer);
	}

	void CCharacterController::OnInitialized()
	{

	}

	void CCharacterController::OnDestroyed()
	{
		DestroyInternal();
	}

	void CCharacterController::OnDisabled()
	{
		DestroyInternal();
	}

	void CCharacterController::OnEnabled()
	{
		const SPtr<SceneInstance>& scene = SO()->GetScene();

		mDesc.position = SO()->GetTransform().GetPosition();
		mInternal = CharacterController::Create(*scene->GetPhysicsScene(), mDesc);
		mInternal->SetOwnerInternal(PhysicsOwnerType::Component, this);

		mInternal->onColliderHit.Connect(std::bind(&CCharacterController::TriggerOnColliderHit, this, _1));
		mInternal->onControllerHit.Connect(std::bind(&CCharacterController::TriggerOnControllerHit, this, _1));

		mInternal->SetLayer(mLayer);
		UpdateDimensions();
	}

	void CCharacterController::OnTransformChanged(TransformChangedFlags flags)
	{
		if (!SO()->GetActive() || mInternal == nullptr)
			return;

		mInternal->SetPosition(SO()->GetTransform().GetPosition());
	}

	void CCharacterController::UpdatePositionFromController()
	{
		mNotifyFlags = (TransformChangedFlags)0;
		SO()->SetWorldPosition(mInternal->GetPosition());
		mNotifyFlags = TCF_Transform;
	}

	void CCharacterController::UpdateDimensions()
	{
		Vector3 scale = SO()->GetTransform().GetScale();
		float height = mDesc.height * Math::Abs(scale.y);
		float radius = mDesc.radius * Math::Abs(std::max(scale.x, scale.z));

		mInternal->SetHeight(height);
		mInternal->SetRadius(radius);
	}

	void CCharacterController::DestroyInternal()
	{
		// This should release the last reference and destroy the internal controller
		if(mInternal)
		{
			mInternal->SetOwnerInternal(PhysicsOwnerType::None, nullptr);
			mInternal = nullptr;
		}
	}

	void CCharacterController::TriggerOnColliderHit(const ControllerColliderCollision& value)
	{
		// Const-cast and modify is okay because we're the only object receiving this event
		auto& hit = const_cast<ControllerColliderCollision&>(value);

		if(hit.colliderRaw)
		{
			const auto collider = (CCollider*)hit.colliderRaw->GetOwnerInternal(PhysicsOwnerType::Component);
			hit.collider = static_object_cast<CCollider>(collider->GetHandle());
		}

		onColliderHit(hit);
	}

	void CCharacterController::TriggerOnControllerHit(const ControllerControllerCollision& value)
	{
		// Const-cast and modify is okay because we're the only object receiving this event
		ControllerControllerCollision& hit = const_cast<ControllerControllerCollision&>(value);

		if(hit.controllerRaw)
		{
			const auto controller = (CCharacterController*)hit.controllerRaw->GetOwnerInternal(PhysicsOwnerType::Component);
			hit.controller = static_object_cast<CCharacterController>(controller->GetHandle());
		}

		onControllerHit(hit);
	}

	RTTITypeBase* CCharacterController::GetRttiStatic()
	{
		return CCharacterControllerRTTI::Instance();
	}

	RTTITypeBase* CCharacterController::GetRtti() const
	{
		return CCharacterController::GetRttiStatic();
	}
}
