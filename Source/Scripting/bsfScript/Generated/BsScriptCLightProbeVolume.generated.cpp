//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCLightProbeVolume.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCLightProbeVolume.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptLightProbeInfo.generated.h"
#include "BsScriptVector3I.generated.h"

namespace bs
{
	ScriptCLightProbeVolume::ScriptCLightProbeVolume(MonoObject* managedInstance, const GameObjectHandle<CLightProbeVolume>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCLightProbeVolume::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_addProbe", (void*)&ScriptCLightProbeVolume::InternalAddProbe);
		metaData.scriptClass->AddInternalCall("Internal_setProbePosition", (void*)&ScriptCLightProbeVolume::InternalSetProbePosition);
		metaData.scriptClass->AddInternalCall("Internal_getProbePosition", (void*)&ScriptCLightProbeVolume::InternalGetProbePosition);
		metaData.scriptClass->AddInternalCall("Internal_removeProbe", (void*)&ScriptCLightProbeVolume::InternalRemoveProbe);
		metaData.scriptClass->AddInternalCall("Internal_getProbes", (void*)&ScriptCLightProbeVolume::InternalGetProbes);
		metaData.scriptClass->AddInternalCall("Internal_renderProbe", (void*)&ScriptCLightProbeVolume::InternalRenderProbe);
		metaData.scriptClass->AddInternalCall("Internal_renderProbes", (void*)&ScriptCLightProbeVolume::InternalRenderProbes);
		metaData.scriptClass->AddInternalCall("Internal_resize", (void*)&ScriptCLightProbeVolume::InternalResize);
		metaData.scriptClass->AddInternalCall("Internal_clip", (void*)&ScriptCLightProbeVolume::InternalClip);
		metaData.scriptClass->AddInternalCall("Internal_reset", (void*)&ScriptCLightProbeVolume::InternalReset);
		metaData.scriptClass->AddInternalCall("Internal_getGridVolume", (void*)&ScriptCLightProbeVolume::InternalGetGridVolume);
		metaData.scriptClass->AddInternalCall("Internal_getCellCount", (void*)&ScriptCLightProbeVolume::InternalGetCellCount);

	}

	uint32_t ScriptCLightProbeVolume::InternalAddProbe(ScriptCLightProbeVolume* thisPtr, Vector3* position)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->addProbe(*position);

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLightProbeVolume::InternalSetProbePosition(ScriptCLightProbeVolume* thisPtr, uint32_t handle, Vector3* position)
	{
		thisPtr->GetHandle()->SetProbePosition(handle, *position);
	}

	void ScriptCLightProbeVolume::InternalGetProbePosition(ScriptCLightProbeVolume* thisPtr, uint32_t handle, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetProbePosition(handle);

		*__output = tmp__output;
	}

	void ScriptCLightProbeVolume::InternalRemoveProbe(ScriptCLightProbeVolume* thisPtr, uint32_t handle)
	{
		thisPtr->GetHandle()->removeProbe(handle);
	}

	MonoArray* ScriptCLightProbeVolume::InternalGetProbes(ScriptCLightProbeVolume* thisPtr)
	{
		Vector<LightProbeInfo> vec__output;
		vec__output = thisPtr->GetHandle()->GetProbes();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptLightProbeInfo>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptLightProbeInfo::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptCLightProbeVolume::InternalRenderProbe(ScriptCLightProbeVolume* thisPtr, uint32_t handle)
	{
		thisPtr->GetHandle()->renderProbe(handle);
	}

	void ScriptCLightProbeVolume::InternalRenderProbes(ScriptCLightProbeVolume* thisPtr)
	{
		thisPtr->GetHandle()->renderProbes();
	}

	void ScriptCLightProbeVolume::InternalResize(ScriptCLightProbeVolume* thisPtr, AABox* volume, Vector3I* cellCount)
	{
		thisPtr->GetHandle()->resize(*volume, *cellCount);
	}

	void ScriptCLightProbeVolume::InternalClip(ScriptCLightProbeVolume* thisPtr)
	{
		thisPtr->GetHandle()->clip();
	}

	void ScriptCLightProbeVolume::InternalReset(ScriptCLightProbeVolume* thisPtr)
	{
		thisPtr->GetHandle()->reset();
	}

	void ScriptCLightProbeVolume::InternalGetGridVolume(ScriptCLightProbeVolume* thisPtr, AABox* __output)
	{
		AABox tmp__output;
		tmp__output = thisPtr->GetHandle()->GetGridVolume();

		*__output = tmp__output;
	}

	void ScriptCLightProbeVolume::InternalGetCellCount(ScriptCLightProbeVolume* thisPtr, Vector3I* __output)
	{
		Vector3I tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCellCount();

		*__output = tmp__output;
	}

}
