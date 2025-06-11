//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsJoint.h"

namespace bs
{
	class PhysicsScene;
	/** @addtogroup Physics
	 *  @{
	 */

	struct FixedJointCreateInformation;

	/** Physics joint that will maintain a fixed distance and orientation between its two attached bodies. */
	class B3D_CORE_EXPORT FixedJoint : public Joint
	{
	public:
		FixedJoint(const FixedJointCreateInformation& desc) {}

		virtual ~FixedJoint() = default;

		/**
		 * Creates a new fixed joint.
		 *
		 * @param[in]	scene		Scene to which to add the joint.
		 * @param[in]	desc		Settings describing the joint.
		 */
		static SPtr<FixedJoint> Create(PhysicsScene& scene, const FixedJointCreateInformation& desc);
	};

	/** Structure used for initializing a new FixedJoint. */
	struct FixedJointCreateInformation : JOINT_DESC
	{};

	/** @} */
} // namespace bs
