//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCReflectionProbe.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCReflectionProbe.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptVector.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"

namespace bs
{
	ScriptCReflectionProbe::ScriptCReflectionProbe(MonoObject* managedInstance, const GameObjectHandle<CReflectionProbe>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCReflectionProbe::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptCReflectionProbe::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_setType", (void*)&ScriptCReflectionProbe::Internal_setType);
		metaData.scriptClass->addInternalCall("Internal_getRadius", (void*)&ScriptCReflectionProbe::Internal_getRadius);
		metaData.scriptClass->addInternalCall("Internal_setRadius", (void*)&ScriptCReflectionProbe::Internal_setRadius);
		metaData.scriptClass->addInternalCall("Internal_getExtents", (void*)&ScriptCReflectionProbe::Internal_getExtents);
		metaData.scriptClass->addInternalCall("Internal_setExtents", (void*)&ScriptCReflectionProbe::Internal_setExtents);
		metaData.scriptClass->addInternalCall("Internal_getCustomTexture", (void*)&ScriptCReflectionProbe::Internal_getCustomTexture);
		metaData.scriptClass->addInternalCall("Internal_setCustomTexture", (void*)&ScriptCReflectionProbe::Internal_setCustomTexture);
		metaData.scriptClass->addInternalCall("Internal_capture", (void*)&ScriptCReflectionProbe::Internal_capture);

	}

	ReflectionProbeType ScriptCReflectionProbe::InternalGetType(ScriptCReflectionProbe* thisPtr)
	{
		ReflectionProbeType tmp__output;
		tmp__output = thisPtr->GetHandle()->GetType();

		ReflectionProbeType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCReflectionProbe::InternalSetType(ScriptCReflectionProbe* thisPtr, ReflectionProbeType type)
	{
		thisPtr->GetHandle()->SetType(type);
	}

	float ScriptCReflectionProbe::InternalGetRadius(ScriptCReflectionProbe* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetRadius();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCReflectionProbe::InternalSetRadius(ScriptCReflectionProbe* thisPtr, float radius)
	{
		thisPtr->GetHandle()->SetRadius(radius);
	}

	void ScriptCReflectionProbe::InternalGetExtents(ScriptCReflectionProbe* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetExtents();

		*__output = tmp__output;
	}

	void ScriptCReflectionProbe::InternalSetExtents(ScriptCReflectionProbe* thisPtr, Vector3* extents)
	{
		thisPtr->GetHandle()->SetExtents(*extents);
	}

	MonoObject* ScriptCReflectionProbe::InternalGetCustomTexture(ScriptCReflectionProbe* thisPtr)
	{
		ResourceHandle<Texture> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCustomTexture();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCReflectionProbe::InternalSetCustomTexture(ScriptCReflectionProbe* thisPtr, MonoObject* texture)
	{
		ResourceHandle<Texture> tmptexture;
		ScriptRRefBase* scripttexture;
		scripttexture = ScriptRRefBase::toNative(texture);
		if(scripttexture != nullptr)
			tmptexture = static_resource_cast<Texture>(scripttexture->GetHandle());
		thisPtr->GetHandle()->SetCustomTexture(tmptexture);
	}

	void ScriptCReflectionProbe::InternalCapture(ScriptCReflectionProbe* thisPtr)
	{
		thisPtr->GetHandle()->capture();
	}
}
