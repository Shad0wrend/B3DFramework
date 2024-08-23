//*********************************** bs::framework - Copyright 2024 Marko Pintera ***************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGameObjectWrapper.h"
#include "BsMonoUtil.h"

using namespace bs;

ScriptGameObject2::ScriptGameObject2(const HGameObject& nativeObject, MonoObject* scriptObject)
	: TScriptGameObjectWrapper(nativeObject, scriptObject)
{}

void ScriptGameObject2::SetupScriptBindings()
{
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_GetId", (void*)&ScriptGameObject2::Internal_GetId);
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_IsDestroyed", (void*)&ScriptGameObject2::Internal_IsDestroyed);
}

void ScriptGameObject2::Internal_GetId(ScriptGameObject2* nativeInstance, UUID* outId)
{
	if(!nativeInstance->IsNativeObjectValid())
	{
		*outId = UUID::kEmpty;
		return;
	}

	*outId = nativeInstance->GetNativeObjectAsHandle()->GetId();
}

bool ScriptGameObject2::Internal_IsDestroyed(ScriptGameObject2* nativeInstance)
{
	return nativeInstance->GetNativeObjectAsHandle().IsDestroyed(true);
}
