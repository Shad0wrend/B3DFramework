//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsSliderJoint.h"
#include "Components/BsCJoint.h"

namespace bs
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	SliderJoint
	 *
	 * @note	Wraps SliderJoint as a Component.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Physics), ExportName(SliderJoint)) CSliderJoint : public CJoint
	{
	public:
		CSliderJoint(const HSceneObject& parent);

		/** @copydoc SliderJoint::GetPosition */
		B3D_SCRIPT_EXPORT(ExportName(Position), Property(Getter))
		float GetPosition() const;

		/** @copydoc SliderJoint::GetSpeed */
		B3D_SCRIPT_EXPORT(ExportName(Speed), Property(Getter))
		float GetSpeed() const;

		/** @copydoc SliderJoint::GetLimit */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Getter))
		LimitLinearRange GetLimit() const;

		/** @copydoc SliderJoint::SetLimit */
		B3D_SCRIPT_EXPORT(ExportName(Limit), Property(Setter))
		void SetLimit(const LimitLinearRange& limit);

		/** @copydoc SliderJoint::SetFlag */
		B3D_SCRIPT_EXPORT(ExportName(SetFlag))
		void SetFlag(SliderJointFlag flag, bool enabled);

		/** @copydoc SliderJoint::HasFlag */
		B3D_SCRIPT_EXPORT(ExportName(HasFlag))
		bool HasFlag(SliderJointFlag flag) const;

		/** @name Internal
		 *  @{
		 */

		/**	Returns the slider joint that this component wraps. */
		SliderJoint* GetInternalInternal() const { return static_cast<SliderJoint*>(mInternal.get()); }

		/** @} */

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		SPtr<Joint> CreateInternal() override;
		void GetLocalTransform(JointBody body, Vector3& position, Quaternion& rotation) override;

		SLIDER_JOINT_DESC mDesc;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CSliderJointRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const;

	protected:
		CSliderJoint(); // Serialization only
	};

	/** @} */
} // namespace bs
