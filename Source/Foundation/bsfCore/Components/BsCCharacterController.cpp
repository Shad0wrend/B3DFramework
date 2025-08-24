//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCCharacterController.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsSceneInstance.h"
#include "Private/RTTI/BsCCharacterControllerRTTI.h"
#include "BsCollider.h"
#include "Physics/BsPhysics.h"

using namespace std::placeholders;

using namespace b3d;

CCharacterController::CCharacterController(const HSceneObject& parent)
	: Component(parent)
{
	SetName("CharacterController");

	mNotifyFlags = TCF_Transform;
}

CCharacterController::CCharacterController()
	: CCharacterController(nullptr)
{ }

CharacterCollisionFlags CCharacterController::Move(const Vector3& displacement)
{
	CharacterCollisionFlags output;

	if(mImplementation == nullptr)
		return output;

	output = mImplementation->Move(displacement);
	UpdateSceneObjectPositionFromController();

	return output;
}

Vector3 CCharacterController::GetFootPosition() const
{
	if(mImplementation == nullptr)
		return Vector3::kZero;

	return mImplementation->GetFootPosition();
}

void CCharacterController::SetFootPosition(const Vector3& position)
{
	if(mImplementation == nullptr)
		return;

	mImplementation->SetFootPosition(position);
	UpdateSceneObjectPositionFromController();
}

void CCharacterController::SetRadius(float radius)
{
	mInformation.Radius = radius;

	if(mImplementation != nullptr)
		UpdateDimensions();
}

void CCharacterController::SetHeight(float height)
{
	mInformation.Height = height;

	if(mImplementation != nullptr)
		UpdateDimensions();
}

void CCharacterController::SetUp(const Vector3& up)
{
	mInformation.Up = up;

	if(mImplementation != nullptr)
		mImplementation->SetUp(up);
}

void CCharacterController::SetClimbingMode(CharacterClimbingMode mode)
{
	mInformation.ClimbingMode = mode;

	if(mImplementation != nullptr)
		mImplementation->SetClimbingMode(mode);
}

void CCharacterController::SetNonWalkableMode(CharacterNonWalkableMode mode)
{
	mInformation.NonWalkableMode = mode;

	if(mImplementation != nullptr)
		mImplementation->SetNonWalkableMode(mode);
}

void CCharacterController::SetMinMoveDistance(float value)
{
	mInformation.MinMoveDistance = value;

	if(mImplementation != nullptr)
		mImplementation->SetMinMoveDistance(value);
}

void CCharacterController::SetContactOffset(float value)
{
	mInformation.ContactOffset = value;

	if(mImplementation != nullptr)
		mImplementation->SetContactOffset(value);
}

void CCharacterController::SetStepOffset(float value)
{
	mInformation.StepOffset = value;

	if(mImplementation != nullptr)
		mImplementation->SetStepOffset(value);
}

void CCharacterController::SetSlopeLimit(Radian value)
{
	mInformation.SlopeLimit = value;

	if(mImplementation != nullptr)
		mImplementation->SetSlopeLimit(value);
}

void CCharacterController::OnDestroyed()
{
	mImplementation = nullptr;
}

void CCharacterController::OnDisabled()
{
	mImplementation = nullptr;
}

void CCharacterController::OnEnabled()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();

	mInformation.Position = SO()->GetTransform().GetPosition();
	mImplementation = scene->GetPhysicsScene()->CreateCharacterController(*this, mInformation);

	UpdateDimensions();
}

void CCharacterController::OnTransformChanged(TransformChangedFlags flags)
{
	if(mImplementation == nullptr)
		return;

	mImplementation->SetPosition(SO()->GetTransform().GetPosition());
}

void CCharacterController::UpdateSceneObjectPositionFromController()
{
	mNotifyFlags = (TransformChangedFlags)0;
	SO()->SetWorldPosition(mImplementation->GetPosition());
	mNotifyFlags = TCF_Transform;
}

void CCharacterController::UpdateDimensions()
{
	const Vector3 scale = SO()->GetTransform().GetScale();
	const float height = mInformation.Height * Math::Abs(scale.Y);
	const float radius = mInformation.Radius * Math::Abs(std::max(scale.X, scale.Z));

	mImplementation->SetHeight(height);
	mImplementation->SetRadius(radius);
}

void CCharacterController::TriggerOnColliderHit(const ControllerColliderCollision& value)
{
	// Const-cast and modify is okay because we're the only object receiving this event
	auto& hit = const_cast<ControllerColliderCollision&>(value);

	if(hit.ColliderShape)
	{
		Collider* const collider = hit.ColliderShape->GetParentCollider();
		hit.Collider = B3DStaticGameObjectCast<Collider>(collider->GetHandle());
	}

	OnColliderHit(hit);
}

void CCharacterController::TriggerOnControllerHit(const ControllerControllerCollision& value)
{
	// Const-cast and modify is okay because we're the only object receiving this event
	ControllerControllerCollision& hit = const_cast<ControllerControllerCollision&>(value);

	if(hit.ControllerRaw)
	{
		const CCharacterController* const controller = hit.ControllerRaw;
		hit.Controller = B3DStaticGameObjectCast<CCharacterController>(controller->GetHandle());
	}

	OnControllerHit(hit);
}

RTTIType* CCharacterController::GetRttiStatic()
{
	return CCharacterControllerRTTI::Instance();
}

RTTIType* CCharacterController::GetRtti() const
{
	return CCharacterController::GetRttiStatic();
}
