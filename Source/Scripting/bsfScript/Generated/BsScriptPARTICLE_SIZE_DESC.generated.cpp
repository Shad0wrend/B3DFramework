//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_SIZE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace bs
{
	ScriptPARTICLE_SIZE_DESC::ScriptPARTICLE_SIZE_DESC(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptPARTICLE_SIZE_DESC::InitRuntimeData()
	{ }

	MonoObject*ScriptPARTICLE_SIZE_DESC::Box(const __PARTICLE_SIZE_DESCInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__PARTICLE_SIZE_DESCInterop ScriptPARTICLE_SIZE_DESC::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_SIZE_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_SIZE_DESC ScriptPARTICLE_SIZE_DESC::FromInterop(const __PARTICLE_SIZE_DESCInterop& value)
	{
		PARTICLE_SIZE_DESC output;
		SPtr<TDistribution<float>> tmpsize;
		ScriptTDistributionfloat* scriptsize;
		scriptsize = ScriptTDistributionfloat::ToNative(value.Size);
		if(scriptsize != nullptr)
			tmpsize = scriptsize->GetInternal();
		if(tmpsize != nullptr)
		output.Size = *tmpsize;
		SPtr<TDistribution<Vector3>> tmpsize3D;
		ScriptTDistributionVector3* scriptsize3D;
		scriptsize3D = ScriptTDistributionVector3::ToNative(value.Size3D);
		if(scriptsize3D != nullptr)
			tmpsize3D = scriptsize3D->GetInternal();
		if(tmpsize3D != nullptr)
		output.Size3D = *tmpsize3D;
		output.Use3DSize = value.Use3DSize;

		return output;
	}

	__PARTICLE_SIZE_DESCInterop ScriptPARTICLE_SIZE_DESC::ToInterop(const PARTICLE_SIZE_DESC& value)
	{
		__PARTICLE_SIZE_DESCInterop output;
		MonoObject* tmpsize;
		SPtr<TDistribution<float>> tmpsizecopy;
		tmpsizecopy = bs_shared_ptr_new<TDistribution<float>>(value.Size);
		tmpsize = ScriptTDistributionfloat::Create(tmpsizecopy);
		output.Size = tmpsize;
		MonoObject* tmpsize3D;
		SPtr<TDistribution<Vector3>> tmpsize3Dcopy;
		tmpsize3Dcopy = bs_shared_ptr_new<TDistribution<Vector3>>(value.Size3D);
		tmpsize3D = ScriptTDistributionVector3::Create(tmpsize3Dcopy);
		output.Size3D = tmpsize3D;
		output.Use3DSize = value.Use3DSize;

		return output;
	}

}
