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

	MonoObject* ScriptParticleCollisionsOptions::Box(const ParticleCollisionSettings& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	ParticleCollisionSettings ScriptParticleCollisionsOptions::Unbox(MonoObject* value)
	{
		return *(ParticleCollisionSettings*)MonoUtil::Unbox(value);
	}

}
