//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsHingeJoint.h"
#include "Components/BsCJoint.h"

namespace bs
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	HingeJoint
	 *
	 * @note	Wraps HingeJoint as a Component.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Physics), ExportName(HingeJoint)) CHingeJoint : public CJoint
	{
	public:
		CHingeJoint(const HSceneObject& parent);

		/** @copydoc HingeJoint::GetAngle */
		B3D_SCRIPT_EXPORT(ExportName(Angle), Property(Getter))
		Radian GetAngle() const;

		/** @copydoc HingeJoint::GetSpeed */
		B3D_SCRIPT_EXPORT(ExportName(Speed), Property(Getter))
		float GetSpeed() const;

		/** @copydoc HingeJoint::GetLimit */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Getter))
		LimitAngularRange GetLimit() const;

		/** @copydoc HingeJoint::SetLimit */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Setter))
		void SetLimit(const LimitAngularRange& limit);

		/** @copydoc HingeJoint::GetDrive */
		B3D_SCRIPT_EXPORT(ExportName(Drive), Property(Getter))
		HingeJointDrive GetDrive() const;

		/** @copydoc HingeJoint::SetDrive */
		B3D_SCRIPT_EXPORT(ExportName(Drive), Property(Setter))
		void SetDrive(const HingeJointDrive& drive);

		/** @copydoc HingeJoint::SetFlag */
		B3D_SCRIPT_EXPORT(ExportName(SetFlag))
		void SetFlag(HingeJointFlag flag, bool enabled);

		/** @copydoc HingeJoint::HasFlag */
		B3D_SCRIPT_EXPORT(ExportName(HasFlag))
		bool HasFlag(HingeJointFlag flag) const;

		/** @name Internal
		 *  @{
		 */

		/**	Returns the hinge joint that this component wraps. */
		HingeJoint* GetInternalInternal() const { return static_cast<HingeJoint*>(mInternal.get()); }

		/** @} */

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		SPtr<Joint> CreateInternal() override;

		HINGE_JOINT_DESC mDesc;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CHingeJointRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const;

	protected:
		CHingeJoint(); // Serialization only
	};

	/** @} */
} // namespace bs
