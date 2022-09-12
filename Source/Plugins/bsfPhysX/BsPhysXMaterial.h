//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPhysXPrerequisites.h"
#include "Physics/BsPhysicsMaterial.h"
#include "PxMaterial.h"

namespace bs
{
	/** @addtogroup PhysX
	 *  @{
	 */

	/** PhysX implementation of a PhysicsMaterial. */
	class PhysXMaterial : public PhysicsMaterial
	{
	public:
		PhysXMaterial(physx::PxPhysics* physx, float staFric, float dynFriction, float restitution);
		~PhysXMaterial();

		/** @copydoc PhysicsMaterial::setStaticFriction */
		void SetStaticFriction(float value) override;

		/** @copydoc PhysicsMaterial::getStaticFriction */
		float GetStaticFriction() const override;

		/** @copydoc PhysicsMaterial::setDynamicFriction */
		void SetDynamicFriction(float value) override;

		/** @copydoc PhysicsMaterial::getDynamicFriction */
		float GetDynamicFriction() const override;

		/** @copydoc PhysicsMaterial::setRestitutionCoefficient */
		void SetRestitutionCoefficient(float value) override;

		/** @copydoc PhysicsMaterial::getRestitutionCoefficient */
		float GetRestitutionCoefficient() const override;

		/** Returns the internal PhysX material. */
		physx::PxMaterial* _getInternal() const { return mInternal; }

	private:
		physx::PxMaterial* mInternal;
	};

	/** @} */
}
