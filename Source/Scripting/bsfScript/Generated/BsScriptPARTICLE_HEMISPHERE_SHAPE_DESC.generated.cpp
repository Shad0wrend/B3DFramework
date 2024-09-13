//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_HEMISPHERE_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptParticleHemisphereShapeOptions::ScriptParticleHemisphereShapeOptions()
	{ }

	MonoObject* ScriptParticleHemisphereShapeOptions::Box(const PARTICLE_HEMISPHERE_SHAPE_DESC& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	PARTICLE_HEMISPHERE_SHAPE_DESC ScriptParticleHemisphereShapeOptions::Unbox(MonoObject* value)
	{
		return *(PARTICLE_HEMISPHERE_SHAPE_DESC*)MonoUtil::Unbox(value);
	}

}
