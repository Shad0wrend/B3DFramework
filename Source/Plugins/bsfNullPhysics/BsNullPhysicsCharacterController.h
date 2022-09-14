//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsNullPhysicsPrerequisites.h"
#include "Physics/BsCharacterController.h"

namespace bs
{
	/** @addtogroup NullPhysics
	 *  @{
	 */

	/** Null implementation if a CharacterController. */
	class NullPhysicsCharacterController : public CharacterController
	{
	public:
		NullPhysicsCharacterController(const CHAR_CONTROLLER_DESC& desc);
		~NullPhysicsCharacterController() = default;

		/** @copydoc CharacterController::move */
		CharacterCollisionFlags Move(const Vector3& displacement) override;

		/** @copydoc CharacterController::getPosition */
		Vector3 GetPosition() const override { return mDesc.position; }

		/** @copydoc CharacterController::setPosition */
		void SetPosition(const Vector3& position) override { mDesc.position = position; }

		/** @copydoc CharacterController::getFootPosition */
		Vector3 GetFootPosition() const override { return mDesc.position; }

		/** @copydoc CharacterController::setFootPosition */
		void SetFootPosition(const Vector3& position) override { mDesc.position = position; }

		/** @copydoc CharacterController::getRadius */
		float GetRadius() const override { return mDesc.radius; }

		/** @copydoc CharacterController::setRadius */
		void SetRadius(float radius) override { mDesc.radius = radius; }

		/** @copydoc CharacterController::getHeight */
		float GetHeight() const override { return mDesc.height; }

		/** @copydoc CharacterController::setHeight */
		void SetHeight(float height) override { mDesc.height = height; }

		/** @copydoc CharacterController::getUp */
		Vector3 GetUp() const override { return mDesc.up; }

		/** @copydoc CharacterController::setUp */
		void SetUp(const Vector3& up) override { mDesc.up = up; }

		/** @copydoc CharacterController::getClimbingMode */
		CharacterClimbingMode GetClimbingMode() const override { return mDesc.climbingMode; }

		/** @copydoc CharacterController::setClimbingMode */
		void SetClimbingMode(CharacterClimbingMode mode) override { mDesc.climbingMode = mode; }

		/** @copydoc CharacterController::getNonWalkableMode */
		CharacterNonWalkableMode GetNonWalkableMode() const override { return mDesc.nonWalkableMode; }

		/** @copydoc CharacterController::setNonWalkableMode */
		void SetNonWalkableMode(CharacterNonWalkableMode mode) override { mDesc.nonWalkableMode = mode; }

		/** @copydoc CharacterController::getMinMoveDistance */
		float GetMinMoveDistance() const override { return mDesc.minMoveDistance; }

		/** @copydoc CharacterController::setMinMoveDistance */
		void SetMinMoveDistance(float value) override { mDesc.minMoveDistance = value; }

		/** @copydoc CharacterController::getContactOffset */
		float GetContactOffset() const override { return mDesc.contactOffset; }

		/** @copydoc CharacterController::setContactOffset */
		void SetContactOffset(float value) override { mDesc.contactOffset = value; }

		/** @copydoc CharacterController::getStepOffset */
		float GetStepOffset() const override { return mDesc.stepOffset; }

		/** @copydoc CharacterController::setStepOffset */
		void SetStepOffset(float value) override { mDesc.stepOffset = value; }

		/** @copydoc CharacterController::getSlopeLimit */
		Radian GetSlopeLimit() const override { return mDesc.slopeLimit; }

		/** @copydoc CharacterController::setSlopeLimit */
		void SetSlopeLimit(Radian value) override { mDesc.slopeLimit = value; }

	private:
		CHAR_CONTROLLER_DESC mDesc;
	};

	/** @} */
}
