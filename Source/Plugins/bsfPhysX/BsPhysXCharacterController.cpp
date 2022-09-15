//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsPhysXCharacterController.h"
#include "Utility/BsTime.h"
#include "BsPhysX.h"
#include "Components/BsCCollider.h"
#include "characterkinematic/PxControllerManager.h"

using namespace physx;

namespace bs
{
	PxExtendedVec3 toPxExtVector(const Vector3& input)
	{
		return PxExtendedVec3(input.x, input.y, input.z);
	}

	Vector3 fromPxExtVector(const PxExtendedVec3& input)
	{
		return Vector3((float)input.x, (float)input.y, (float)input.z);
	}

	PxCapsuleClimbingMode::Enum toPxEnum(CharacterClimbingMode value)
	{
		return value == CharacterClimbingMode::Normal
			? PxCapsuleClimbingMode::eEASY
			: PxCapsuleClimbingMode::eCONSTRAINED;
	}

	PxControllerNonWalkableMode::Enum toPxEnum(CharacterNonWalkableMode value)
	{
		return value == CharacterNonWalkableMode::Prevent
			? PxControllerNonWalkableMode::ePREVENT_CLIMBING
			: PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	}

	CharacterClimbingMode fromPxEnum(PxCapsuleClimbingMode::Enum value)
	{
		return value == PxCapsuleClimbingMode::eEASY
			? CharacterClimbingMode::Normal
			: CharacterClimbingMode::Constrained;
	}

	CharacterNonWalkableMode fromPxEnum(PxControllerNonWalkableMode::Enum value)
	{
		return value == PxControllerNonWalkableMode::ePREVENT_CLIMBING
			? CharacterNonWalkableMode::Prevent
			: CharacterNonWalkableMode::PreventAndSlide;
	}

	PxCapsuleControllerDesc toPxDesc(const CHAR_CONTROLLER_DESC& desc)
	{
		PxCapsuleControllerDesc output;
		output.climbingMode = toPxEnum(desc.climbingMode);
		output.nonWalkableMode = toPxEnum(desc.nonWalkableMode);
		output.contactOffset = desc.contactOffset;
		output.stepOffset = desc.stepOffset;
		output.slopeLimit = desc.slopeLimit.ValueRadians();
		output.height = desc.height;
		output.radius = desc.radius;
		output.upDirection = toPxVector(desc.up);
		output.position = toPxExtVector(desc.position);

		return output;
	}

	PhysXCharacterController::PhysXCharacterController(PxControllerManager* manager, const CHAR_CONTROLLER_DESC& desc)
		:CharacterController(desc)
	{
		PxCapsuleControllerDesc pxDesc = toPxDesc(desc);
		pxDesc.reportCallback = this;
		pxDesc.material = gPhysX().GetDefaultMaterial();
		pxDesc.height = pxDesc.height <= 0 ? 0.01f : pxDesc.height;

		mController = static_cast<PxCapsuleController*>(manager->createController(pxDesc));
		mController->SetUserData(this);
	}

	PhysXCharacterController::~PhysXCharacterController()
	{
		mController->SetUserData(nullptr);
		mController->release();
	}

	CharacterCollisionFlags PhysXCharacterController::Move(const Vector3& displacement)
	{
		PxControllerFilters filters;
		filters.mFilterCallback = this;
		filters.mFilterFlags = PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
		filters.mCCTFilterCallback = this;

		float curTime = gTime().GetTime();
		float delta = curTime - mLastMoveCall;
		mLastMoveCall = curTime;

		PxControllerCollisionFlags collisionFlag = mController->move(toPxVector(displacement), mMinMoveDistance, delta, filters);

		CharacterCollisionFlags output;
		if (collisionFlag.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
			output.Set(CharacterCollisionFlag::Down);

		if (collisionFlag.isSet(PxControllerCollisionFlag::eCOLLISION_UP))
			output.Set(CharacterCollisionFlag::Up);

		if (collisionFlag.isSet(PxControllerCollisionFlag::eCOLLISION_SIDES))
			output.Set(CharacterCollisionFlag::Sides);

		return output;
	}

	Vector3 PhysXCharacterController::GetPosition() const
	{
		return fromPxExtVector(mController->GetPosition());
	}

	void PhysXCharacterController::SetPosition(const Vector3& position)
	{
		mController->SetPosition(toPxExtVector(position));
	}

	Vector3 PhysXCharacterController::GetFootPosition() const
	{
		return fromPxExtVector(mController->GetFootPosition());
	}

	void PhysXCharacterController::SetFootPosition(const Vector3& position)
	{
		mController->SetFootPosition(toPxExtVector(position));
	}

	float PhysXCharacterController::GetRadius() const
	{
		return mController->GetRadius();
	}

	void PhysXCharacterController::SetRadius(float radius)
	{
		mController->SetRadius(radius);
	}

	float PhysXCharacterController::GetHeight() const
	{
		return mController->GetHeight();
	}

	void PhysXCharacterController::SetHeight(float height)
	{
		mController->SetHeight(height);
	}

	Vector3 PhysXCharacterController::GetUp() const
	{
		return fromPxVector(mController->GetUpDirection());
	}

	void PhysXCharacterController::SetUp(const Vector3& up)
	{
		mController->SetUpDirection(toPxVector(up));
	}

	CharacterClimbingMode PhysXCharacterController::GetClimbingMode() const
	{
		return fromPxEnum(mController->GetClimbingMode());
	}

	void PhysXCharacterController::SetClimbingMode(CharacterClimbingMode mode)
	{
		mController->SetClimbingMode(toPxEnum(mode));
	}

	CharacterNonWalkableMode PhysXCharacterController::getNonWalkableMode() const
	{
		return fromPxEnum(mController->GetNonWalkableMode());
	}

	void PhysXCharacterController::SetNonWalkableMode(CharacterNonWalkableMode mode)
	{
		mController->SetNonWalkableMode(toPxEnum(mode));
	}

	float PhysXCharacterController::GetMinMoveDistance() const
	{
		return mMinMoveDistance;
	}

	void PhysXCharacterController::SetMinMoveDistance(float value)
	{
		mMinMoveDistance = value;
	}

	float PhysXCharacterController::GetContactOffset() const
	{
		return mController->GetContactOffset();
	}

	void PhysXCharacterController::SetContactOffset(float value)
	{
		mController->SetContactOffset(value);
	}

	float PhysXCharacterController::GetStepOffset() const
	{
		return mController->GetStepOffset();
	}

	void PhysXCharacterController::SetStepOffset(float value)
	{
		mController->SetStepOffset(value);
	}

	Radian PhysXCharacterController::GetSlopeLimit() const
	{
		return Radian(mController->GetSlopeLimit());
	}

	void PhysXCharacterController::SetSlopeLimit(Radian value)
	{
		mController->SetSlopeLimit(value.valueRadians());
	}

	void PhysXCharacterController::onShapeHit(const PxControllerShapeHit& hit)
	{
		if (onColliderHit.empty())
			return;

		ControllerColliderCollision collision;
		collision.position = fromPxExtVector(hit.worldPos);
		collision.normal = fromPxVector(hit.worldNormal);
		collision.motionDir = fromPxVector(hit.dir);
		collision.motionAmount = hit.length;
		collision.triangleIndex = hit.triangleIndex;
		collision.colliderRaw = (Collider*)hit.shape->userData;

		onColliderHit(collision);
	}

	void PhysXCharacterController::onControllerHit(const PxControllersHit& hit)
	{
		if (CharacterController::onControllerHit.empty())
			return;

		ControllerControllerCollision collision;
		collision.position = fromPxExtVector(hit.worldPos);
		collision.normal = fromPxVector(hit.worldNormal);
		collision.motionDir = fromPxVector(hit.dir);
		collision.motionAmount = hit.length;
		collision.controllerRaw = (CharacterController*)hit.controller->GetUserData();

		CharacterController::onControllerHit(collision);
	}

	PxQueryHitType::Enum PhysXCharacterController::preFilter(const PxFilterData& filterData, const PxShape* shape,
		const PxRigidActor* actor, PxHitFlags& queryFlags)
	{
		PxFilterData colliderFilterData = shape->GetSimulationFilterData();
		UINT64 colliderLayer = *(UINT64*)&colliderFilterData.word0;

		bool canCollide = gPhysics().isCollisionEnabled(colliderLayer, getLayer());

		if(canCollide)
			return PxSceneQueryHitType::eBLOCK;

		return PxSceneQueryHitType::eNONE;
	}

	PxQueryHitType::Enum PhysXCharacterController::postFilter(const PxFilterData& filterData,
		const PxQueryHit& hit)
	{
		return PxSceneQueryHitType::eBLOCK;
	}

	bool PhysXCharacterController::filter(const PxController& a, const PxController& b)
	{
		CharacterController* controllerA = (CharacterController*)a.getUserData();
		CharacterController* controllerB = (CharacterController*)b.getUserData();

		bool canCollide = gPhysics().isCollisionEnabled(controllerA->GetLayer(), controllerB->GetLayer());
		return canCollide;
	}
}
