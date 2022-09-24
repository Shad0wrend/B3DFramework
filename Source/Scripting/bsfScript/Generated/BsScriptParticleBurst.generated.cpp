//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleBurst.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace bs
{
	ScriptParticleBurst::ScriptParticleBurst(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptParticleBurst::InitRuntimeData()
	{ }

	MonoObject*ScriptParticleBurst::Box(const __ParticleBurstInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__ParticleBurstInterop ScriptParticleBurst::Unbox(MonoObject* value)
	{
		return *(__ParticleBurstInterop*)MonoUtil::Unbox(value);
	}

	ParticleBurst ScriptParticleBurst::FromInterop(const __ParticleBurstInterop& value)
	{
		ParticleBurst output;
		output.Time = value.Time;
		SPtr<TDistribution<float>> tmpcount;
		ScriptTDistributionfloat* scriptcount;
		scriptcount = ScriptTDistributionfloat::ToNative(value.Count);
		if(scriptcount != nullptr)
			tmpcount = scriptcount->GetInternal();
		if(tmpcount != nullptr)
		output.Count = *tmpcount;
		output.Cycles = value.Cycles;
		output.Interval = value.Interval;

		return output;
	}

	__ParticleBurstInterop ScriptParticleBurst::ToInterop(const ParticleBurst& value)
	{
		__ParticleBurstInterop output;
		output.Time = value.Time;
		MonoObject* tmpcount;
		SPtr<TDistribution<float>> tmpcountcopy;
		tmpcountcopy = bs_shared_ptr_new<TDistribution<float>>(value.Count);
		tmpcount = ScriptTDistributionfloat::Create(tmpcountcopy);
		output.Count = tmpcount;
		output.Cycles = value.Cycles;
		output.Interval = value.Interval;

		return output;
	}

}
