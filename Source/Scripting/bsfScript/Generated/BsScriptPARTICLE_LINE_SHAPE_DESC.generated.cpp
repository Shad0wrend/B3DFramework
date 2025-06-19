//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_LINE_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmissionMode.generated.h"

namespace b3d
{
	ScriptParticleLineShapeOptions::ScriptParticleLineShapeOptions()
	{ }

	MonoObject* ScriptParticleLineShapeOptions::Box(const __PARTICLE_LINE_SHAPE_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_LINE_SHAPE_DESCInterop ScriptParticleLineShapeOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_LINE_SHAPE_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_LINE_SHAPE_DESC ScriptParticleLineShapeOptions::FromInterop(const __PARTICLE_LINE_SHAPE_DESCInterop& value)
	{
		PARTICLE_LINE_SHAPE_DESC output;
		output.Length = value.Length;
		output.Mode = value.Mode;

		return output;
	}

	__PARTICLE_LINE_SHAPE_DESCInterop ScriptParticleLineShapeOptions::ToInterop(const PARTICLE_LINE_SHAPE_DESC& value)
	{
		__PARTICLE_LINE_SHAPE_DESCInterop output;
		output.Length = value.Length;
		output.Mode = value.Mode;

		return output;
	}

}
