//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_COLLISIONS_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace b3d
{
	ScriptParticleCollisionsOptions::ScriptParticleCollisionsOptions()
	{ }

	MonoObject* ScriptParticleCollisionsOptions::Box(const PARTICLE_COLLISIONS_DESC& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	PARTICLE_COLLISIONS_DESC ScriptParticleCollisionsOptions::Unbox(MonoObject* value)
	{
		return *(PARTICLE_COLLISIONS_DESC*)MonoUtil::Unbox(value);
	}

}
