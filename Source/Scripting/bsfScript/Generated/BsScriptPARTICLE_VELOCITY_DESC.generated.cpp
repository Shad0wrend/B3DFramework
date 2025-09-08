//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_VELOCITY_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace b3d
{
	ScriptParticleVelocityOptions::ScriptParticleVelocityOptions()
	{ }

	MonoObject* ScriptParticleVelocityOptions::Box(const __PARTICLE_VELOCITY_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_VELOCITY_DESCInterop ScriptParticleVelocityOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_VELOCITY_DESCInterop*)MonoUtil::Unbox(value);
	}

	ParticleVelocitySettings ScriptParticleVelocityOptions::FromInterop(const __PARTICLE_VELOCITY_DESCInterop& value)
	{
		ParticleVelocitySettings output;
		SPtr<TDistribution<TVector3<float>>> tmpVelocity;
		ScriptVector3Distribution* scriptObjectWrapperVelocity;
		scriptObjectWrapperVelocity = ScriptVector3Distribution::GetScriptObjectWrapper(value.Velocity);
		if(scriptObjectWrapperVelocity != nullptr)
			tmpVelocity = std::static_pointer_cast<TDistribution<TVector3<float>>>(scriptObjectWrapperVelocity->GetBaseNativeObjectAsShared());
		if(tmpVelocity != nullptr)
		output.Velocity = *tmpVelocity;
		output.WorldSpace = value.WorldSpace;

		return output;
	}

	__PARTICLE_VELOCITY_DESCInterop ScriptParticleVelocityOptions::ToInterop(const ParticleVelocitySettings& value)
	{
		__PARTICLE_VELOCITY_DESCInterop output;
		MonoObject* tmpVelocity;
		SPtr<TDistribution<TVector3<float>>> tmpVelocitycopy;
		tmpVelocitycopy = B3DMakeShared<TDistribution<TVector3<float>>>(value.Velocity);
		tmpVelocity = ScriptVector3Distribution::GetOrCreateScriptObject(tmpVelocitycopy);
		output.Velocity = tmpVelocity;
		output.WorldSpace = value.WorldSpace;

		return output;
	}

}
