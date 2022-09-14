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
		metaData.scriptClass->addInternalCall("Internal_setMaterial", (void*)&ScriptCDecal::Internal_setMaterial);
		metaData.scriptClass->addInternalCall("Internal_getMaterial", (void*)&ScriptCDecal::Internal_getMaterial);
		metaData.scriptClass->addInternalCall("Internal_setSize", (void*)&ScriptCDecal::Internal_setSize);
		metaData.scriptClass->addInternalCall("Internal_getSize", (void*)&ScriptCDecal::Internal_getSize);
		metaData.scriptClass->addInternalCall("Internal_setMaxDistance", (void*)&ScriptCDecal::Internal_setMaxDistance);
		metaData.scriptClass->addInternalCall("Internal_getMaxDistance", (void*)&ScriptCDecal::Internal_getMaxDistance);
		metaData.scriptClass->addInternalCall("Internal_setLayer", (void*)&ScriptCDecal::Internal_setLayer);
		metaData.scriptClass->addInternalCall("Internal_getLayer", (void*)&ScriptCDecal::Internal_getLayer);
		metaData.scriptClass->addInternalCall("Internal_setLayerMask", (void*)&ScriptCDecal::Internal_setLayerMask);
		metaData.scriptClass->addInternalCall("Internal_getLayerMask", (void*)&ScriptCDecal::Internal_getLayerMask);

	}

	void ScriptCDecal::InternalSetMaterial(ScriptCDecal* thisPtr, MonoObject* material)
	{
		ResourceHandle<Material> tmpmaterial;
		ScriptRRefBase* scriptmaterial;
		scriptmaterial = ScriptRRefBase::toNative(material);
		if(scriptmaterial != nullptr)
			tmpmaterial = static_resource_cast<Material>(scriptmaterial->getHandle());
		thisPtr->getHandle()->setMaterial(tmpmaterial);
	}

	MonoObject* ScriptCDecal::InternalGetMaterial(ScriptCDecal* thisPtr)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->getHandle()->getMaterial();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCDecal::InternalSetSize(ScriptCDecal* thisPtr, Vector2* size)
	{
		thisPtr->getHandle()->setSize(*size);
	}

	void ScriptCDecal::InternalGetSize(ScriptCDecal* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->getHandle()->getSize();

		*__output = tmp__output;
	}

	void ScriptCDecal::InternalSetMaxDistance(ScriptCDecal* thisPtr, float distance)
	{
		thisPtr->getHandle()->setMaxDistance(distance);
	}

	float ScriptCDecal::InternalGetMaxDistance(ScriptCDecal* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getMaxDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDecal::InternalSetLayer(ScriptCDecal* thisPtr, uint64_t layer)
	{
		thisPtr->getHandle()->setLayer(layer);
	}

	uint64_t ScriptCDecal::InternalGetLayer(ScriptCDecal* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = thisPtr->getHandle()->getLayer();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDecal::InternalSetLayerMask(ScriptCDecal* thisPtr, uint32_t mask)
	{
		thisPtr->getHandle()->setLayerMask(mask);
	}

	uint32_t ScriptCDecal::InternalGetLayerMask(ScriptCDecal* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getLayerMask();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}
}
