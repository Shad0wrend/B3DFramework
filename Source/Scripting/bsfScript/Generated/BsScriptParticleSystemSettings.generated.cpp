//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleSystemSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Material/BsMaterial.h"
#include "../../../Foundation/bsfCore/Mesh/BsMesh.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptParticleSystemSettings::ScriptParticleSystemSettings(MonoObject* managedInstance, const SPtr<ParticleSystemSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptParticleSystemSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Getmaterial", (void*)&ScriptParticleSystemSettings::InternalGetmaterial);
		metaData.scriptClass->AddInternalCall("Internal_Setmaterial", (void*)&ScriptParticleSystemSettings::InternalSetmaterial);
		metaData.scriptClass->AddInternalCall("Internal_Getmesh", (void*)&ScriptParticleSystemSettings::InternalGetmesh);
		metaData.scriptClass->AddInternalCall("Internal_Setmesh", (void*)&ScriptParticleSystemSettings::InternalSetmesh);
		metaData.scriptClass->AddInternalCall("Internal_GetsimulationSpace", (void*)&ScriptParticleSystemSettings::InternalGetsimulationSpace);
		metaData.scriptClass->AddInternalCall("Internal_SetsimulationSpace", (void*)&ScriptParticleSystemSettings::InternalSetsimulationSpace);
		metaData.scriptClass->AddInternalCall("Internal_Getorientation", (void*)&ScriptParticleSystemSettings::InternalGetorientation);
		metaData.scriptClass->AddInternalCall("Internal_Setorientation", (void*)&ScriptParticleSystemSettings::InternalSetorientation);
		metaData.scriptClass->AddInternalCall("Internal_Getduration", (void*)&ScriptParticleSystemSettings::InternalGetduration);
		metaData.scriptClass->AddInternalCall("Internal_Setduration", (void*)&ScriptParticleSystemSettings::InternalSetduration);
		metaData.scriptClass->AddInternalCall("Internal_GetisLooping", (void*)&ScriptParticleSystemSettings::InternalGetisLooping);
		metaData.scriptClass->AddInternalCall("Internal_SetisLooping", (void*)&ScriptParticleSystemSettings::InternalSetisLooping);
		metaData.scriptClass->AddInternalCall("Internal_GetmaxParticles", (void*)&ScriptParticleSystemSettings::InternalGetmaxParticles);
		metaData.scriptClass->AddInternalCall("Internal_SetmaxParticles", (void*)&ScriptParticleSystemSettings::InternalSetmaxParticles);
		metaData.scriptClass->AddInternalCall("Internal_GetgpuSimulation", (void*)&ScriptParticleSystemSettings::InternalGetgpuSimulation);
		metaData.scriptClass->AddInternalCall("Internal_SetgpuSimulation", (void*)&ScriptParticleSystemSettings::InternalSetgpuSimulation);
		metaData.scriptClass->AddInternalCall("Internal_GetrenderMode", (void*)&ScriptParticleSystemSettings::InternalGetrenderMode);
		metaData.scriptClass->AddInternalCall("Internal_SetrenderMode", (void*)&ScriptParticleSystemSettings::InternalSetrenderMode);
		metaData.scriptClass->AddInternalCall("Internal_GetorientationLockY", (void*)&ScriptParticleSystemSettings::InternalGetorientationLockY);
		metaData.scriptClass->AddInternalCall("Internal_SetorientationLockY", (void*)&ScriptParticleSystemSettings::InternalSetorientationLockY);
		metaData.scriptClass->AddInternalCall("Internal_GetorientationPlaneNormal", (void*)&ScriptParticleSystemSettings::InternalGetorientationPlaneNormal);
		metaData.scriptClass->AddInternalCall("Internal_SetorientationPlaneNormal", (void*)&ScriptParticleSystemSettings::InternalSetorientationPlaneNormal);
		metaData.scriptClass->AddInternalCall("Internal_GetsortMode", (void*)&ScriptParticleSystemSettings::InternalGetsortMode);
		metaData.scriptClass->AddInternalCall("Internal_SetsortMode", (void*)&ScriptParticleSystemSettings::InternalSetsortMode);
		metaData.scriptClass->AddInternalCall("Internal_GetuseAutomaticSeed", (void*)&ScriptParticleSystemSettings::InternalGetuseAutomaticSeed);
		metaData.scriptClass->AddInternalCall("Internal_SetuseAutomaticSeed", (void*)&ScriptParticleSystemSettings::InternalSetuseAutomaticSeed);
		metaData.scriptClass->AddInternalCall("Internal_GetmanualSeed", (void*)&ScriptParticleSystemSettings::InternalGetmanualSeed);
		metaData.scriptClass->AddInternalCall("Internal_SetmanualSeed", (void*)&ScriptParticleSystemSettings::InternalSetmanualSeed);
		metaData.scriptClass->AddInternalCall("Internal_GetuseAutomaticBounds", (void*)&ScriptParticleSystemSettings::InternalGetuseAutomaticBounds);
		metaData.scriptClass->AddInternalCall("Internal_SetuseAutomaticBounds", (void*)&ScriptParticleSystemSettings::InternalSetuseAutomaticBounds);
		metaData.scriptClass->AddInternalCall("Internal_GetcustomBounds", (void*)&ScriptParticleSystemSettings::InternalGetcustomBounds);
		metaData.scriptClass->AddInternalCall("Internal_SetcustomBounds", (void*)&ScriptParticleSystemSettings::InternalSetcustomBounds);

	}

	MonoObject* ScriptParticleSystemSettings::Create(const SPtr<ParticleSystemSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleSystemSettings>()) ScriptParticleSystemSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleSystemSettings::InternalGetmaterial(ScriptParticleSystemSettings* thisPtr)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->GetInternal()->material;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmaterial(ScriptParticleSystemSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<Material> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Material>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->material = tmpvalue;
	}

	MonoObject* ScriptParticleSystemSettings::InternalGetmesh(ScriptParticleSystemSettings* thisPtr)
	{
		ResourceHandle<Mesh> tmp__output;
		tmp__output = thisPtr->GetInternal()->mesh;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmesh(ScriptParticleSystemSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<Mesh> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Mesh>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->mesh = tmpvalue;
	}

	ParticleSimulationSpace ScriptParticleSystemSettings::InternalGetsimulationSpace(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleSimulationSpace tmp__output;
		tmp__output = thisPtr->GetInternal()->simulationSpace;

		ParticleSimulationSpace __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetsimulationSpace(ScriptParticleSystemSettings* thisPtr, ParticleSimulationSpace value)
	{
		thisPtr->GetInternal()->simulationSpace = value;
	}

	ParticleOrientation ScriptParticleSystemSettings::InternalGetorientation(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleOrientation tmp__output;
		tmp__output = thisPtr->GetInternal()->orientation;

		ParticleOrientation __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetorientation(ScriptParticleSystemSettings* thisPtr, ParticleOrientation value)
	{
		thisPtr->GetInternal()->orientation = value;
	}

	float ScriptParticleSystemSettings::InternalGetduration(ScriptParticleSystemSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->duration;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetduration(ScriptParticleSystemSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->duration = value;
	}

	bool ScriptParticleSystemSettings::InternalGetisLooping(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->isLooping;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetisLooping(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->isLooping = value;
	}

	uint32_t ScriptParticleSystemSettings::InternalGetmaxParticles(ScriptParticleSystemSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->maxParticles;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmaxParticles(ScriptParticleSystemSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->maxParticles = value;
	}

	bool ScriptParticleSystemSettings::InternalGetgpuSimulation(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->gpuSimulation;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetgpuSimulation(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->gpuSimulation = value;
	}

	ParticleRenderMode ScriptParticleSystemSettings::InternalGetrenderMode(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleRenderMode tmp__output;
		tmp__output = thisPtr->GetInternal()->renderMode;

		ParticleRenderMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetrenderMode(ScriptParticleSystemSettings* thisPtr, ParticleRenderMode value)
	{
		thisPtr->GetInternal()->renderMode = value;
	}

	bool ScriptParticleSystemSettings::InternalGetorientationLockY(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->orientationLockY;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetorientationLockY(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->orientationLockY = value;
	}

	void ScriptParticleSystemSettings::InternalGetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->orientationPlaneNormal;

		*__output = tmp__output;


	}

	void ScriptParticleSystemSettings::InternalSetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->orientationPlaneNormal = *value;
	}

	ParticleSortMode ScriptParticleSystemSettings::InternalGetsortMode(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleSortMode tmp__output;
		tmp__output = thisPtr->GetInternal()->sortMode;

		ParticleSortMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetsortMode(ScriptParticleSystemSettings* thisPtr, ParticleSortMode value)
	{
		thisPtr->GetInternal()->sortMode = value;
	}

	bool ScriptParticleSystemSettings::InternalGetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->useAutomaticSeed;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->useAutomaticSeed = value;
	}

	uint32_t ScriptParticleSystemSettings::InternalGetmanualSeed(ScriptParticleSystemSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->manualSeed;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmanualSeed(ScriptParticleSystemSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->manualSeed = value;
	}

	bool ScriptParticleSystemSettings::InternalGetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->useAutomaticBounds;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->useAutomaticBounds = value;
	}

	void ScriptParticleSystemSettings::InternalGetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* __output)
	{
		AABox tmp__output;
		tmp__output = thisPtr->GetInternal()->customBounds;

		*__output = tmp__output;


	}

	void ScriptParticleSystemSettings::InternalSetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* value)
	{
		thisPtr->GetInternal()->customBounds = *value;
	}
}
