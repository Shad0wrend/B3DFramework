//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCSkybox.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCSkybox.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"

namespace bs
{
	ScriptCSkybox::ScriptCSkybox(MonoObject* managedInstance, const GameObjectHandle<CSkybox>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCSkybox::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getTexture", (void*)&ScriptCSkybox::InternalGetTexture);
		metaData.scriptClass->AddInternalCall("Internal_setTexture", (void*)&ScriptCSkybox::InternalSetTexture);
		metaData.scriptClass->AddInternalCall("Internal_setBrightness", (void*)&ScriptCSkybox::InternalSetBrightness);
		metaData.scriptClass->AddInternalCall("Internal_getBrightness", (void*)&ScriptCSkybox::InternalGetBrightness);

	}

	MonoObject* ScriptCSkybox::InternalGetTexture(ScriptCSkybox* thisPtr)
	{
		ResourceHandle<Texture> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetTexture();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCSkybox::InternalSetTexture(ScriptCSkybox* thisPtr, MonoObject* texture)
	{
		ResourceHandle<Texture> tmptexture;
		ScriptRRefBase* scripttexture;
		scripttexture = ScriptRRefBase::toNative(texture);
		if(scripttexture != nullptr)
			tmptexture = static_resource_cast<Texture>(scripttexture->GetHandle());
		thisPtr->GetHandle()->SetTexture(tmptexture);
	}

	void ScriptCSkybox::InternalSetBrightness(ScriptCSkybox* thisPtr, float brightness)
	{
		thisPtr->GetHandle()->SetBrightness(brightness);
	}

	float ScriptCSkybox::InternalGetBrightness(ScriptCSkybox* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetBrightness();

		float __output;
		__output = tmp__output;

		return __output;
	}
}
