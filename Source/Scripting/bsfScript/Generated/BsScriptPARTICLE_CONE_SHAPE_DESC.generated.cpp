//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_CONE_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmissionMode.generated.h"

namespace b3d
{
	ScriptParticleConeShapeOptions::ScriptParticleConeShapeOptions()
	{ }

	MonoObject* ScriptParticleConeShapeOptions::Box(const __PARTICLE_CONE_SHAPE_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_CONE_SHAPE_DESCInterop ScriptParticleConeShapeOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_CONE_SHAPE_DESCInterop*)MonoUtil::Unbox(value);
	}

	ParticleConeShapeSettings ScriptParticleConeShapeOptions::FromInterop(const __PARTICLE_CONE_SHAPE_DESCInterop& value)
	{
		ParticleConeShapeSettings output;
		output.Type = value.Type;
		output.Radius = value.Radius;
		output.Angle = value.Angle;
		output.Length = value.Length;
		output.Thickness = value.Thickness;
		output.Arc = value.Arc;
		output.Mode = value.Mode;

		return output;
	}

	__PARTICLE_CONE_SHAPE_DESCInterop ScriptParticleConeShapeOptions::ToInterop(const ParticleConeShapeSettings& value)
	{
		__PARTICLE_CONE_SHAPE_DESCInterop output;
		output.Type = value.Type;
		output.Radius = value.Radius;
		output.Angle = value.Angle;
		output.Length = value.Length;
		output.Thickness = value.Thickness;
		output.Arc = value.Arc;
		output.Mode = value.Mode;

		return output;
	}

}
