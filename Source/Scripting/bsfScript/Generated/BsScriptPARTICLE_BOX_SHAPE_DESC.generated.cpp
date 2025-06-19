//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_BOX_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfUtility/Math/BsVector3.h"
#include "BsScriptTVector3.generated.h"

namespace b3d
{
	ScriptParticleBoxShapeOptions::ScriptParticleBoxShapeOptions()
	{ }

	MonoObject* ScriptParticleBoxShapeOptions::Box(const __PARTICLE_BOX_SHAPE_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_BOX_SHAPE_DESCInterop ScriptParticleBoxShapeOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_BOX_SHAPE_DESCInterop*)MonoUtil::Unbox(value);
	}

	PARTICLE_BOX_SHAPE_DESC ScriptParticleBoxShapeOptions::FromInterop(const __PARTICLE_BOX_SHAPE_DESCInterop& value)
	{
		PARTICLE_BOX_SHAPE_DESC output;
		output.Type = value.Type;
		output.Extents = value.Extents;

		return output;
	}

	__PARTICLE_BOX_SHAPE_DESCInterop ScriptParticleBoxShapeOptions::ToInterop(const PARTICLE_BOX_SHAPE_DESC& value)
	{
		__PARTICLE_BOX_SHAPE_DESCInterop output;
		output.Type = value.Type;
		output.Extents = value.Extents;

		return output;
	}

}
