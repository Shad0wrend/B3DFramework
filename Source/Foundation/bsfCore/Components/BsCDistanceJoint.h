//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsDistanceJoint.h"
#include "Components/BsCJoint.h"

namespace b3d
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	DistanceJoint
	 *
	 * @note	Wraps DistanceJoint as a Component.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Physics), ExportName(DistanceJoint)) CDistanceJoint : public CJoint
	{
	public:
		CDistanceJoint(const HSceneObject& parent);

		/** @copydoc DistanceJoint::GetDistance */
		B3D_SCRIPT_EXPORT(ExportName(Distance), Property(Getter))
		float GetDistance() const;

		/** @copydoc DistanceJoint::GetMinDistance */
		B3D_SCRIPT_EXPORT(ExportName(MinDistance), Property(Getter))
		float GetMinDistance() const;

		/** @copydoc DistanceJoint::SetMinDistance */
		B3D_SCRIPT_EXPORT(ExportName(MinDistance), Property(Setter))
		void SetMinDistance(float value);

		/** @copydoc DistanceJoint::GetMaxDistance */
		B3D_SCRIPT_EXPORT(ExportName(MaxDistance), Property(Getter))
		float GetMaxDistance() const;

		/** @copydoc DistanceJoint::SetMaxDistance */
		B3D_SCRIPT_EXPORT(ExportName(MaxDistance), Property(Setter))
		void SetMaxDistance(float value);

		/** @copydoc DistanceJoint::GetTolerance */
		B3D_SCRIPT_EXPORT(ExportName(Tolerance), Property(Getter))
		float GetTolerance() const;

		/** @copydoc DistanceJoint::SetTolerance */
		B3D_SCRIPT_EXPORT(ExportName(Tolerance), Property(Setter))
		void SetTolerance(float value);

		/** @copydoc DistanceJoint::GetSpring */
		B3D_SCRIPT_EXPORT(ExportName(Spring), Property(Getter))
		Spring GetSpring() const;

		/** @copydoc DistanceJoint::SetSpring */
		B3D_SCRIPT_EXPORT(ExportName(Spring), Property(Setter))
		void SetSpring(const Spring& value);

		/** @copydoc DistanceJoint::SetFlag */
		B3D_SCRIPT_EXPORT(ExportName(SetFlag))
		void SetFlag(DistanceJointFlag flag, bool enabled);

		/** @copydoc DistanceJoint::HasFlag */
		B3D_SCRIPT_EXPORT(ExportName(HasFlag))
		bool HasFlag(DistanceJointFlag flag) const;

		/** @name Internal
		 *  @{
		 */

		/**	Returns the distance joint that this component wraps. */
		DistanceJoint* GetInternalInternal() const { return static_cast<DistanceJoint*>(mInternal.get()); }

		/** @} */

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		SPtr<Joint> CreateInternal() override;

		DistanceJointCreateInformation mDesc;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CDistanceJointRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const;

	protected:
		CDistanceJoint(); // Serialization only
	};

	/** @} */
} // namespace b3d
