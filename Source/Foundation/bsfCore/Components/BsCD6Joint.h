//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsD6Joint.h"
#include "Components/BsCJoint.h"

namespace bs
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	D6Joint
	 *
	 * @note	Wraps D6Joint as a Component.
	 */
	class BS_CORE_EXPORT BS_SCRIPT_EXPORT(DocumentationGroup(Physics),ExportName(D6Joint)) CD6Joint : public CJoint
	{
	public:
		CD6Joint(const HSceneObject& parent);

		/** @copydoc D6Joint::getMotion */
		BS_SCRIPT_EXPORT(ExportName(GetMotion))
		D6JointMotion GetMotion(D6JointAxis axis) const;

		/** @copydoc D6Joint::setMotion */
		BS_SCRIPT_EXPORT(ExportName(SetMotion))
		void SetMotion(D6JointAxis axis, D6JointMotion motion);

		/** @copydoc D6Joint::getTwist */
		BS_SCRIPT_EXPORT(ExportName(Twist),pr:getter)
		Radian GetTwist() const;

		/** @copydoc D6Joint::getSwingY */
		BS_SCRIPT_EXPORT(ExportName(SwingY),pr:getter)
		Radian GetSwingY() const;

		/** @copydoc D6Joint::getSwingZ */
		BS_SCRIPT_EXPORT(ExportName(SwingZ),pr:getter)
		Radian GetSwingZ() const;

		/** @copydoc D6Joint::getLimitLinear */
		BS_SCRIPT_EXPORT(ExportName(LimitLinear),pr:getter)
		LimitLinear GetLimitLinear() const;

		/** @copydoc D6Joint::setLimitLinear */
		BS_SCRIPT_EXPORT(ExportName(LimitLinear),pr:setter)
		void SetLimitLinear(const LimitLinear& limit);

		/** @copydoc D6Joint::getLimitTwist */
		BS_SCRIPT_EXPORT(ExportName(LimitTwist),pr:getter)
		LimitAngularRange GetLimitTwist() const;

		/** @copydoc D6Joint::setLimitTwist */
		BS_SCRIPT_EXPORT(ExportName(LimitTwist),pr:setter)
		void SetLimitTwist(const LimitAngularRange& limit);

		/** @copydoc D6Joint::getLimitSwing */
		BS_SCRIPT_EXPORT(ExportName(LimitSwing),pr:getter)
		LimitConeRange GetLimitSwing() const;

		/** @copydoc D6Joint::setLimitSwing */
		BS_SCRIPT_EXPORT(ExportName(LimitSwing),pr:setter)
		void SetLimitSwing(const LimitConeRange& limit);

		/** @copydoc D6Joint::getDrive */
		BS_SCRIPT_EXPORT(ExportName(GetDrive))
		D6JointDrive GetDrive(D6JointDriveType type) const;

		/** @copydoc D6Joint::setDrive */
		BS_SCRIPT_EXPORT(ExportName(SetDrive))
		void SetDrive(D6JointDriveType type, const D6JointDrive& drive);

		/** @copydoc D6Joint::getDrivePosition */
		BS_SCRIPT_EXPORT(ExportName(DrivePosition),pr:getter)
		Vector3 GetDrivePosition() const;

		/** @copydoc D6Joint::getDriveRotation */
		BS_SCRIPT_EXPORT(ExportName(DriveRotation),pr:getter)
		Quaternion GetDriveRotation() const;

		/** @copydoc D6Joint::setDriveTransform */
		BS_SCRIPT_EXPORT(ExportName(SetDriveTransform))
		void SetDriveTransform(const Vector3& position, const Quaternion& rotation);

		/** @copydoc D6Joint::getDriveLinearVelocity */
		BS_SCRIPT_EXPORT(ExportName(DriveLinearVelocity),pr:getter)
		Vector3 GetDriveLinearVelocity() const;

		/** @copydoc D6Joint::getDriveAngularVelocity */
		BS_SCRIPT_EXPORT(ExportName(DriveAngularVelocity),pr:getter)
		Vector3 GetDriveAngularVelocity() const;

		/** @copydoc D6Joint::setDriveVelocity */
		BS_SCRIPT_EXPORT(ExportName(SetDriveVelocity))
		void SetDriveVelocity(const Vector3& linear, const Vector3& angular);

		/** @name Internal
		 *  @{
		 */

		/**	Returns the D6 joint that this component wraps. */
		D6Joint* GetInternalInternal() const { return static_cast<D6Joint*>(mInternal.get()); }

		/** @} */

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/** @copydoc CJoint::createInternal */
		SPtr<Joint> CreateInternal() ;

		D6_JOINT_DESC mDesc;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CD6JointRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const ;

	protected:
		CD6Joint(); // Serialization only
	 };

	 /** @} */
}
