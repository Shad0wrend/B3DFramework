//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsHingeJoint.h"
#include "Components/BsCJoint.h"

namespace b3d
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/** Hinge joint removes all but a single rotation degree of freedom from its two attached bodies (for example a door hinge). */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Physics), ExportName(HingeJoint)) CHingeJoint : public CJoint
	{
	public:
		CHingeJoint(const HSceneObject& parent);

		/** Returns the current angle between the two attached bodes. */
		B3D_SCRIPT_EXPORT(ExportName(Angle), Property(Getter))
		Radian GetAngle() const;

		/** Returns the current angular speed of the joint. */
		B3D_SCRIPT_EXPORT(ExportName(Speed), Property(Getter))
		float GetSpeed() const;

		/**
		 * Determines the limit of the joint. Limit constrains the motion to the specified angle range. You must enable the
		 * limit flag on the joint in order for this to be recognized.
		 *
		 * @see LimitAngularRange
		 */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Setter))
		void SetLimit(const LimitAngularRange& limit);

		/** @copydoc SetLimit */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Getter))
		LimitAngularRange GetLimit() const;

		/**
		 * Determines the drive properties of the joint. It drives the joint's angular velocity towards a particular value.
		 * You must enable the drive flag on the joint in order for the drive to be active.
		 *
		 * @see HingeJoint::Drive
		 */
		B3D_SCRIPT_EXPORT(ExportName(Drive), Property(Setter))
		void SetDrive(const HingeJointDrive& drive);

		/** @copydoc SetDrive */
		B3D_SCRIPT_EXPORT(ExportName(Drive), Property(Getter))
		HingeJointDrive GetDrive() const;

		/** Enables or disables a flag that controls joint behaviour. */
		B3D_SCRIPT_EXPORT(ExportName(SetFlag))
		void SetFlag(HingeJointFlag flag, bool enabled);

		/** Checks is the specified option enabled. */
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

		HingeJointCreateInformation mInformation;

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

	/** Low-level interface for a joint used by the HingeJoint component. Should be implemented by the physics plugin to provide joint functionality. */
	class B3D_CORE_EXPORT IHingeJointImplementation : public IJointImplementation
	{
	public:
		/** @copydoc HingeJoint::GetAngle */
		virtual Radian GetAngle() const = 0;

		/** @copydoc HingeJoint::GetSpeed */
		virtual float GetSpeed() const = 0;

		/** @copydoc HingeJoint::SetLimit */
		virtual void SetLimit(const LimitAngularRange& limit) = 0;

		/** @copydoc HingeJoint::GetLimit */
		virtual LimitAngularRange GetLimit() const = 0;

		/** @copydoc HingeJoint::SetDrive */
		virtual void SetDrive(const HingeJointDrive& drive) = 0;

		/** @copydoc HingeJoint::GetDrive */
		virtual HingeJointDrive GetDrive() const = 0;

		/** @copydoc HingeJoint::SetFlag */
		virtual void SetFlag(HingeJointFlag flag, bool enabled) = 0;

		/** @copydoc HingeJoint::HasFlag */
		virtual bool HasFlag(HingeJointFlag flag) const = 0;
	};

	/** @} */
} // namespace b3d
