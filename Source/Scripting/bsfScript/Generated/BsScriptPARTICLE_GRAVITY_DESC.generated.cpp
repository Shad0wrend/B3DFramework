//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_GRAVITY_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace b3d
{
	ScriptParticleGravityOptions::ScriptParticleGravityOptions()
	{ }

	MonoObject* ScriptParticleGravityOptions::Box(const ParticleGravitySettings& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	ParticleGravitySettings ScriptParticleGravityOptions::Unbox(MonoObject* value)
	{
		return *(ParticleGravitySettings*)MonoUtil::Unbox(value);
	}

}
