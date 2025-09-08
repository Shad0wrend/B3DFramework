//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_HEMISPHERE_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace b3d
{
	ScriptParticleHemisphereShapeOptions::ScriptParticleHemisphereShapeOptions()
	{ }

	MonoObject* ScriptParticleHemisphereShapeOptions::Box(const ParticleHemisphereShapeSettings& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	ParticleHemisphereShapeSettings ScriptParticleHemisphereShapeOptions::Unbox(MonoObject* value)
	{
		return *(ParticleHemisphereShapeSettings*)MonoUtil::Unbox(value);
	}

}
