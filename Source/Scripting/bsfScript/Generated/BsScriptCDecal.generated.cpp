//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCDecal.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCDecal.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptVector.h"
#include "../../../Foundation/bsfCore/Material/BsMaterial.h"

namespace bs
{
	ScriptCDecal::ScriptCDecal(MonoObject* managedInstance, const GameObjectHandle<CDecal>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCDecal::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setMaterial", (void*)&ScriptCDecal::InternalSetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_getMaterial", (void*)&ScriptCDecal::InternalGetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_setSize", (void*)&ScriptCDecal::InternalSetSize);
		metaData.scriptClass->AddInternalCall("Internal_getSize", (void*)&ScriptCDecal::InternalGetSize);
		metaData.scriptClass->AddInternalCall("Internal_setMaxDistance", (void*)&ScriptCDecal::InternalSetMaxDistance);
		metaData.scriptClass->AddInternalCall("Internal_getMaxDistance", (void*)&ScriptCDecal::InternalGetMaxDistance);
		metaData.scriptClass->AddInternalCall("Internal_setLayer", (void*)&ScriptCDecal::InternalSetLayer);
		metaData.scriptClass->AddInternalCall("Internal_getLayer", (void*)&ScriptCDecal::InternalGetLayer);
		metaData.scriptClass->AddInternalCall("Internal_setLayerMask", (void*)&ScriptCDecal::InternalSetLayerMask);
		metaData.scriptClass->AddInternalCall("Internal_getLayerMask", (void*)&ScriptCDecal::InternalGetLayerMask);

	}

	void ScriptCDecal::InternalSetMaterial(ScriptCDecal* thisPtr, MonoObject* material)
	{
		ResourceHandle<Material> tmpmaterial;
		ScriptRRefBase* scriptmaterial;
		scriptmaterial = ScriptRRefBase::toNative(material);
		if(scriptmaterial != nullptr)
			tmpmaterial = static_resource_cast<Material>(scriptmaterial->GetHandle());
		thisPtr->GetHandle()->SetMaterial(tmpmaterial);
	}

	MonoObject* ScriptCDecal::InternalGetMaterial(ScriptCDecal* thisPtr)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMaterial();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCDecal::InternalSetSize(ScriptCDecal* thisPtr, Vector2* size)
	{
		thisPtr->GetHandle()->SetSize(*size);
	}

	void ScriptCDecal::InternalGetSize(ScriptCDecal* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSize();

		*__output = tmp__output;
	}

	void ScriptCDecal::InternalSetMaxDistance(ScriptCDecal* thisPtr, float distance)
	{
		thisPtr->GetHandle()->SetMaxDistance(distance);
	}

	float ScriptCDecal::InternalGetMaxDistance(ScriptCDecal* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMaxDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDecal::InternalSetLayer(ScriptCDecal* thisPtr, uint64_t layer)
	{
		thisPtr->GetHandle()->SetLayer(layer);
	}

	uint64_t ScriptCDecal::InternalGetLayer(ScriptCDecal* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLayer();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDecal::InternalSetLayerMask(ScriptCDecal* thisPtr, uint32_t mask)
	{
		thisPtr->GetHandle()->SetLayerMask(mask);
	}

	uint32_t ScriptCDecal::InternalGetLayerMask(ScriptCDecal* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLayerMask();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}
}
