//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCReflectionProbe.h"
#include "Private/RTTI/BsCReflectionProbeRTTI.h"
#include "Scene/BsSceneManager.h"

namespace bs
{
	CReflectionProbe::CReflectionProbe()
	{
		setFlag(ComponentFlag::AlwaysRun, true);
		setName("ReflectionProbe");
	}

	CReflectionProbe::CReflectionProbe(const HSceneObject& parent)
		: Component(parent)
	{
		setFlag(ComponentFlag::AlwaysRun, true);
		setName("ReflectionProbe");
	}

	CReflectionProbe::~CReflectionProbe()
	{
		mInternal->destroy();
	}

	Sphere CReflectionProbe::GetBounds() const
	{
		mInternal->UpdateStateInternal(*SO());

		return mInternal->getBounds();
	}

	void CReflectionProbe::OnInitialized()
	{
		// If mInternal already exists this means this object was deserialized,
		// so all we need to do is initialize it.
		if (mInternal != nullptr)
			mInternal->initialize();
		else
			mInternal = ReflectionProbe::CreateBox(Vector3::ONE);

		gSceneManager().BindActorInternal(mInternal, sceneObject());

		// If filtered texture doesn't exist, ensure it is generated
		SPtr<Texture> filteredTexture = mInternal->getFilteredTexture();
		if(filteredTexture == nullptr)
		{
			if (mInternal->getCustomTexture())
				mInternal->filter();
			else
				mInternal->capture();
		}
	}

	void CReflectionProbe::OnDestroyed()
	{
		gSceneManager().UnbindActorInternal(mInternal);
	}

	RTTITypeBase* CReflectionProbe::GetRttiStatic()
	{
		return CReflectionProbeRTTI::Instance();
	}

	RTTITypeBase* CReflectionProbe::GetRtti() const
	{
		return CReflectionProbe::GetRttiStatic();
	}
}
