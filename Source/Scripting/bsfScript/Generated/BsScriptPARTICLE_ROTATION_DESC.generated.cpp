//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_ROTATION_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace bs
{
	ScriptPARTICLE_ROTATION_DESC::ScriptPARTICLE_ROTATION_DESC(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptPARTICLE_ROTATION_DESC::InitRuntimeData()
	{ }

	MonoObject*ScriptPARTICLE_ROTATION_DESC::Box(const __PARTICLE_ROTATION_DESCInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__PARTICLE_ROTATION_DESCInterop ScriptPARTICLE_ROTATION_DESC::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_ROTATION_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_ROTATION_DESC ScriptPARTICLE_ROTATION_DESC::FromInterop(const __PARTICLE_ROTATION_DESCInterop& value)
	{
		PARTICLE_ROTATION_DESC output;
		SPtr<TDistribution<float>> tmprotation;
		ScriptTDistributionfloat* scriptrotation;
		scriptrotation = ScriptTDistributionfloat::ToNative(value.Rotation);
		if(scriptrotation != nullptr)
			tmprotation = scriptrotation->GetInternal();
		if(tmprotation != nullptr)
		output.Rotation = *tmprotation;
		SPtr<TDistribution<Vector3>> tmprotation3D;
		ScriptTDistributionVector3* scriptrotation3D;
		scriptrotation3D = ScriptTDistributionVector3::ToNative(value.Rotation3D);
		if(scriptrotation3D != nullptr)
			tmprotation3D = scriptrotation3D->GetInternal();
		if(tmprotation3D != nullptr)
		output.Rotation3D = *tmprotation3D;
		output.Use3DRotation = value.Use3DRotation;

		return output;
	}

	__PARTICLE_ROTATION_DESCInterop ScriptPARTICLE_ROTATION_DESC::ToInterop(const PARTICLE_ROTATION_DESC& value)
	{
		__PARTICLE_ROTATION_DESCInterop output;
		MonoObject* tmprotation;
		SPtr<TDistribution<float>> tmprotationcopy;
		tmprotationcopy = bs_shared_ptr_new<TDistribution<float>>(value.Rotation);
		tmprotation = ScriptTDistributionfloat::Create(tmprotationcopy);
		output.Rotation = tmprotation;
		MonoObject* tmprotation3D;
		SPtr<TDistribution<Vector3>> tmprotation3Dcopy;
		tmprotation3Dcopy = bs_shared_ptr_new<TDistribution<Vector3>>(value.Rotation3D);
		tmprotation3D = ScriptTDistributionVector3::Create(tmprotation3Dcopy);
		output.Rotation3D = tmprotation3D;
		output.Use3DRotation = value.Use3DRotation;

		return output;
	}

}
