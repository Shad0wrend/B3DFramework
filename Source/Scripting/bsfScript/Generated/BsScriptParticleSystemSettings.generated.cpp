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
		metaData.ScriptClass->AddInternalCall("Internal_Getmaterial", (void*)&ScriptParticleSystemSettings::InternalGetmaterial);
		metaData.ScriptClass->AddInternalCall("Internal_Setmaterial", (void*)&ScriptParticleSystemSettings::InternalSetmaterial);
		metaData.ScriptClass->AddInternalCall("Internal_Getmesh", (void*)&ScriptParticleSystemSettings::InternalGetmesh);
		metaData.ScriptClass->AddInternalCall("Internal_Setmesh", (void*)&ScriptParticleSystemSettings::InternalSetmesh);
		metaData.ScriptClass->AddInternalCall("Internal_GetsimulationSpace", (void*)&ScriptParticleSystemSettings::InternalGetsimulationSpace);
		metaData.ScriptClass->AddInternalCall("Internal_SetsimulationSpace", (void*)&ScriptParticleSystemSettings::InternalSetsimulationSpace);
		metaData.ScriptClass->AddInternalCall("Internal_Getorientation", (void*)&ScriptParticleSystemSettings::InternalGetorientation);
		metaData.ScriptClass->AddInternalCall("Internal_Setorientation", (void*)&ScriptParticleSystemSettings::InternalSetorientation);
		metaData.ScriptClass->AddInternalCall("Internal_Getduration", (void*)&ScriptParticleSystemSettings::InternalGetduration);
		metaData.ScriptClass->AddInternalCall("Internal_Setduration", (void*)&ScriptParticleSystemSettings::InternalSetduration);
		metaData.ScriptClass->AddInternalCall("Internal_GetisLooping", (void*)&ScriptParticleSystemSettings::InternalGetisLooping);
		metaData.ScriptClass->AddInternalCall("Internal_SetisLooping", (void*)&ScriptParticleSystemSettings::InternalSetisLooping);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxParticles", (void*)&ScriptParticleSystemSettings::InternalGetmaxParticles);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxParticles", (void*)&ScriptParticleSystemSettings::InternalSetmaxParticles);
		metaData.ScriptClass->AddInternalCall("Internal_GetgpuSimulation", (void*)&ScriptParticleSystemSettings::InternalGetgpuSimulation);
		metaData.ScriptClass->AddInternalCall("Internal_SetgpuSimulation", (void*)&ScriptParticleSystemSettings::InternalSetgpuSimulation);
		metaData.ScriptClass->AddInternalCall("Internal_GetrenderMode", (void*)&ScriptParticleSystemSettings::InternalGetrenderMode);
		metaData.ScriptClass->AddInternalCall("Internal_SetrenderMode", (void*)&ScriptParticleSystemSettings::InternalSetrenderMode);
		metaData.ScriptClass->AddInternalCall("Internal_GetorientationLockY", (void*)&ScriptParticleSystemSettings::InternalGetorientationLockY);
		metaData.ScriptClass->AddInternalCall("Internal_SetorientationLockY", (void*)&ScriptParticleSystemSettings::InternalSetorientationLockY);
		metaData.ScriptClass->AddInternalCall("Internal_GetorientationPlaneNormal", (void*)&ScriptParticleSystemSettings::InternalGetorientationPlaneNormal);
		metaData.ScriptClass->AddInternalCall("Internal_SetorientationPlaneNormal", (void*)&ScriptParticleSystemSettings::InternalSetorientationPlaneNormal);
		metaData.ScriptClass->AddInternalCall("Internal_GetsortMode", (void*)&ScriptParticleSystemSettings::InternalGetsortMode);
		metaData.ScriptClass->AddInternalCall("Internal_SetsortMode", (void*)&ScriptParticleSystemSettings::InternalSetsortMode);
		metaData.ScriptClass->AddInternalCall("Internal_GetuseAutomaticSeed", (void*)&ScriptParticleSystemSettings::InternalGetuseAutomaticSeed);
		metaData.ScriptClass->AddInternalCall("Internal_SetuseAutomaticSeed", (void*)&ScriptParticleSystemSettings::InternalSetuseAutomaticSeed);
		metaData.ScriptClass->AddInternalCall("Internal_GetmanualSeed", (void*)&ScriptParticleSystemSettings::InternalGetmanualSeed);
		metaData.ScriptClass->AddInternalCall("Internal_SetmanualSeed", (void*)&ScriptParticleSystemSettings::InternalSetmanualSeed);
		metaData.ScriptClass->AddInternalCall("Internal_GetuseAutomaticBounds", (void*)&ScriptParticleSystemSettings::InternalGetuseAutomaticBounds);
		metaData.ScriptClass->AddInternalCall("Internal_SetuseAutomaticBounds", (void*)&ScriptParticleSystemSettings::InternalSetuseAutomaticBounds);
		metaData.ScriptClass->AddInternalCall("Internal_GetcustomBounds", (void*)&ScriptParticleSystemSettings::InternalGetcustomBounds);
		metaData.ScriptClass->AddInternalCall("Internal_SetcustomBounds", (void*)&ScriptParticleSystemSettings::InternalSetcustomBounds);

	}

	MonoObject* ScriptParticleSystemSettings::Create(const SPtr<ParticleSystemSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleSystemSettings>()) ScriptParticleSystemSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleSystemSettings::InternalGetmaterial(ScriptParticleSystemSettings* thisPtr)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->GetInternal()->Material;

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
		thisPtr->GetInternal()->Material = tmpvalue;
	}

	MonoObject* ScriptParticleSystemSettings::InternalGetmesh(ScriptParticleSystemSettings* thisPtr)
	{
		ResourceHandle<Mesh> tmp__output;
		tmp__output = thisPtr->GetInternal()->Mesh;

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
		thisPtr->GetInternal()->Mesh = tmpvalue;
	}

	ParticleSimulationSpace ScriptParticleSystemSettings::InternalGetsimulationSpace(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleSimulationSpace tmp__output;
		tmp__output = thisPtr->GetInternal()->SimulationSpace;

		ParticleSimulationSpace __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetsimulationSpace(ScriptParticleSystemSettings* thisPtr, ParticleSimulationSpace value)
	{
		thisPtr->GetInternal()->SimulationSpace = value;
	}

	ParticleOrientation ScriptParticleSystemSettings::InternalGetorientation(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleOrientation tmp__output;
		tmp__output = thisPtr->GetInternal()->Orientation;

		ParticleOrientation __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetorientation(ScriptParticleSystemSettings* thisPtr, ParticleOrientation value)
	{
		thisPtr->GetInternal()->Orientation = value;
	}

	float ScriptParticleSystemSettings::InternalGetduration(ScriptParticleSystemSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Duration;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetduration(ScriptParticleSystemSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Duration = value;
	}

	bool ScriptParticleSystemSettings::InternalGetisLooping(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->IsLooping;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetisLooping(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->IsLooping = value;
	}

	uint32_t ScriptParticleSystemSettings::InternalGetmaxParticles(ScriptParticleSystemSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxParticles;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmaxParticles(ScriptParticleSystemSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->MaxParticles = value;
	}

	bool ScriptParticleSystemSettings::InternalGetgpuSimulation(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->GpuSimulation;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetgpuSimulation(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->GpuSimulation = value;
	}

	ParticleRenderMode ScriptParticleSystemSettings::InternalGetrenderMode(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleRenderMode tmp__output;
		tmp__output = thisPtr->GetInternal()->RenderMode;

		ParticleRenderMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetrenderMode(ScriptParticleSystemSettings* thisPtr, ParticleRenderMode value)
	{
		thisPtr->GetInternal()->RenderMode = value;
	}

	bool ScriptParticleSystemSettings::InternalGetorientationLockY(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->OrientationLockY;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetorientationLockY(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->OrientationLockY = value;
	}

	void ScriptParticleSystemSettings::InternalGetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->OrientationPlaneNormal;

		*__output = tmp__output;


	}

	void ScriptParticleSystemSettings::InternalSetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->OrientationPlaneNormal = *value;
	}

	ParticleSortMode ScriptParticleSystemSettings::InternalGetsortMode(ScriptParticleSystemSettings* thisPtr)
	{
		ParticleSortMode tmp__output;
		tmp__output = thisPtr->GetInternal()->SortMode;

		ParticleSortMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetsortMode(ScriptParticleSystemSettings* thisPtr, ParticleSortMode value)
	{
		thisPtr->GetInternal()->SortMode = value;
	}

	bool ScriptParticleSystemSettings::InternalGetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->UseAutomaticSeed;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->UseAutomaticSeed = value;
	}

	uint32_t ScriptParticleSystemSettings::InternalGetmanualSeed(ScriptParticleSystemSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->ManualSeed;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetmanualSeed(ScriptParticleSystemSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->ManualSeed = value;
	}

	bool ScriptParticleSystemSettings::InternalGetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->UseAutomaticBounds;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleSystemSettings::InternalSetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->UseAutomaticBounds = value;
	}

	void ScriptParticleSystemSettings::InternalGetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* __output)
	{
		AABox tmp__output;
		tmp__output = thisPtr->GetInternal()->CustomBounds;

		*__output = tmp__output;


	}

	void ScriptParticleSystemSettings::InternalSetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* value)
	{
		thisPtr->GetInternal()->CustomBounds = *value;
	}
}
