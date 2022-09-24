//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_ORBIT_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace bs
{
	ScriptPARTICLE_ORBIT_DESC::ScriptPARTICLE_ORBIT_DESC(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptPARTICLE_ORBIT_DESC::InitRuntimeData()
	{ }

	MonoObject*ScriptPARTICLE_ORBIT_DESC::Box(const __PARTICLE_ORBIT_DESCInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__PARTICLE_ORBIT_DESCInterop ScriptPARTICLE_ORBIT_DESC::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_ORBIT_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_ORBIT_DESC ScriptPARTICLE_ORBIT_DESC::FromInterop(const __PARTICLE_ORBIT_DESCInterop& value)
	{
		PARTICLE_ORBIT_DESC output;
		SPtr<TDistribution<Vector3>> tmpcenter;
		ScriptTDistributionVector3* scriptcenter;
		scriptcenter = ScriptTDistributionVector3::ToNative(value.Center);
		if(scriptcenter != nullptr)
			tmpcenter = scriptcenter->GetInternal();
		if(tmpcenter != nullptr)
		output.Center = *tmpcenter;
		SPtr<TDistribution<Vector3>> tmpvelocity;
		ScriptTDistributionVector3* scriptvelocity;
		scriptvelocity = ScriptTDistributionVector3::ToNative(value.Velocity);
		if(scriptvelocity != nullptr)
			tmpvelocity = scriptvelocity->GetInternal();
		if(tmpvelocity != nullptr)
		output.Velocity = *tmpvelocity;
		SPtr<TDistribution<float>> tmpradial;
		ScriptTDistributionfloat* scriptradial;
		scriptradial = ScriptTDistributionfloat::ToNative(value.Radial);
		if(scriptradial != nullptr)
			tmpradial = scriptradial->GetInternal();
		if(tmpradial != nullptr)
		output.Radial = *tmpradial;
		output.WorldSpace = value.WorldSpace;

		return output;
	}

	__PARTICLE_ORBIT_DESCInterop ScriptPARTICLE_ORBIT_DESC::ToInterop(const PARTICLE_ORBIT_DESC& value)
	{
		__PARTICLE_ORBIT_DESCInterop output;
		MonoObject* tmpcenter;
		SPtr<TDistribution<Vector3>> tmpcentercopy;
		tmpcentercopy = bs_shared_ptr_new<TDistribution<Vector3>>(value.Center);
		tmpcenter = ScriptTDistributionVector3::Create(tmpcentercopy);
		output.Center = tmpcenter;
		MonoObject* tmpvelocity;
		SPtr<TDistribution<Vector3>> tmpvelocitycopy;
		tmpvelocitycopy = bs_shared_ptr_new<TDistribution<Vector3>>(value.Velocity);
		tmpvelocity = ScriptTDistributionVector3::Create(tmpvelocitycopy);
		output.Velocity = tmpvelocity;
		MonoObject* tmpradial;
		SPtr<TDistribution<float>> tmpradialcopy;
		tmpradialcopy = bs_shared_ptr_new<TDistribution<float>>(value.Radial);
		tmpradial = ScriptTDistributionfloat::Create(tmpradialcopy);
		output.Radial = tmpradial;
		output.WorldSpace = value.WorldSpace;

		return output;
	}

}
