//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_SIZE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "BsScriptTDistribution.generated.h"

namespace b3d
{
	ScriptParticleSizeOptions::ScriptParticleSizeOptions()
	{ }

	MonoObject* ScriptParticleSizeOptions::Box(const __PARTICLE_SIZE_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_SIZE_DESCInterop ScriptParticleSizeOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_SIZE_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_SIZE_DESC ScriptParticleSizeOptions::FromInterop(const __PARTICLE_SIZE_DESCInterop& value)
	{
		PARTICLE_SIZE_DESC output;
		SPtr<TDistribution<float>> tmpSize;
		ScriptFloatDistribution* scriptObjectWrapperSize;
		scriptObjectWrapperSize = ScriptFloatDistribution::GetScriptObjectWrapper(value.Size);
		if(scriptObjectWrapperSize != nullptr)
			tmpSize = std::static_pointer_cast<TDistribution<float>>(scriptObjectWrapperSize->GetBaseNativeObjectAsShared());
		if(tmpSize != nullptr)
		output.Size = *tmpSize;
		SPtr<TDistribution<TVector3<float>>> tmpSize3D;
		ScriptVector3Distribution* scriptObjectWrapperSize3D;
		scriptObjectWrapperSize3D = ScriptVector3Distribution::GetScriptObjectWrapper(value.Size3D);
		if(scriptObjectWrapperSize3D != nullptr)
			tmpSize3D = std::static_pointer_cast<TDistribution<TVector3<float>>>(scriptObjectWrapperSize3D->GetBaseNativeObjectAsShared());
		if(tmpSize3D != nullptr)
		output.Size3D = *tmpSize3D;
		output.Use3DSize = value.Use3DSize;

		return output;
	}

	__PARTICLE_SIZE_DESCInterop ScriptParticleSizeOptions::ToInterop(const PARTICLE_SIZE_DESC& value)
	{
		__PARTICLE_SIZE_DESCInterop output;
		MonoObject* tmpSize;
		SPtr<TDistribution<float>> tmpSizecopy;
		tmpSizecopy = B3DMakeShared<TDistribution<float>>(value.Size);
		tmpSize = ScriptFloatDistribution::GetOrCreateScriptObject(tmpSizecopy);
		output.Size = tmpSize;
		MonoObject* tmpSize3D;
		SPtr<TDistribution<TVector3<float>>> tmpSize3Dcopy;
		tmpSize3Dcopy = B3DMakeShared<TDistribution<TVector3<float>>>(value.Size3D);
		tmpSize3D = ScriptVector3Distribution::GetOrCreateScriptObject(tmpSize3Dcopy);
		output.Size3D = tmpSize3D;
		output.Use3DSize = value.Use3DSize;

		return output;
	}

}
