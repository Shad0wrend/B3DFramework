//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Renderer/BsReflectionProbe.h"
#include "Scene/BsComponent.h"

namespace bs
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	ReflectionProbe
	 *
	 * @note	Wraps ReflectionProbe as a Component.
	 */
	class BS_CORE_EXPORT BS_SCRIPT_EXPORT(m:Rendering,n:ReflectionProbe) CReflectionProbe : public Component
	{
	public:
		CReflectionProbe(const HSceneObject& parent);
		virtual ~CReflectionProbe();

		/** @copydoc ReflectionProbe::getType */
		BS_SCRIPT_EXPORT(n:Type,pr:getter)
		ReflectionProbeType GetType() const { return mInternal->GetTypeId(); }

		/** @copydoc ReflectionProbe::setType */
		BS_SCRIPT_EXPORT(n:Type,pr:setter)
		void SetType(ReflectionProbeType type) { mInternal->setType(type); }

		/** @copydoc ReflectionProbe::getRadius */
		BS_SCRIPT_EXPORT(n:Radius,pr:getter)
		float GetRadius() const { return mInternal->getRadius(); }

		/** @copydoc ReflectionProbe::setRadius */
		BS_SCRIPT_EXPORT(n:Radius,pr:setter)
		void SetRadius(float radius) { mInternal->setRadius(radius); }

		/** @copydoc ReflectionProbe::getExtents */
		BS_SCRIPT_EXPORT(n:Extents,pr:getter)
		Vector3 GetExtents() const { return mInternal->getExtents(); }

		/** @copydoc ReflectionProbe::setExtents */
		BS_SCRIPT_EXPORT(n:Extents,pr:setter)
		void SetExtents(const Vector3& extents) { mInternal->setExtents(extents); }

		/** Retrieves transition distance set by setTransitionDistance(). */
		float GetTransitionDistance() const { return mInternal->getTransitionDistance(); }

		/** @copydoc ReflectionProbe::setTransitionDistance */
		void SetTransitionDistance(float distance) { mInternal->setTransitionDistance(distance); }

		/** @copydoc ReflectionProbe::getCustomTexture */
		BS_SCRIPT_EXPORT(n:CustomTexture,pr:getter)
		HTexture GetCustomTexture() const { return mInternal->getCustomTexture(); }

		/** @copydoc ReflectionProbe::setCustomTexture */
		BS_SCRIPT_EXPORT(n:CustomTexture,pr:setter)
		void SetCustomTexture(const HTexture& texture) { mInternal->setCustomTexture(texture); }

		/** @copydoc ReflectionProbe::getBounds */
		Sphere GetBounds() const;

		/** @copydoc ReflectionProbe::capture */
		BS_SCRIPT_EXPORT(n:Capture)
		void Capture() { mInternal->capture(); }

		/** @name Internal
		 *  @{
		 */

		/**	Returns the reflection probe that this component wraps. */
		SPtr<ReflectionProbe> GetReflectionProbeInternal() const { return mInternal; }

		/** @} */

	protected:
		mutable SPtr<ReflectionProbe> mInternal;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/** @copydoc Component::onInitialized */
		void OnInitialized() ;

		/** @copydoc Component::onDestroyed */
		void OnDestroyed() ;

		/** @copydoc Component::update */
		void Update() override { }

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CReflectionProbeRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;

	protected:
		CReflectionProbe(); // Serialization only
	};

	/** @} */
}
