//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPhysics.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysics.h"

namespace bs
{
	ScriptPhysics::ScriptPhysics(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
	}

	void ScriptPhysics::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_toggleCollision", (void*)&ScriptPhysics::InternalToggleCollision);
		metaData.scriptClass->AddInternalCall("Internal_isCollisionEnabled", (void*)&ScriptPhysics::InternalIsCollisionEnabled);
		metaData.scriptClass->AddInternalCall("Internal__isUpdateInProgress", (void*)&ScriptPhysics::InternalIsUpdateInProgress);

	}

	void ScriptPhysics::InternalToggleCollision(uint64_t groupA, uint64_t groupB, bool enabled)
	{
		Physics::Instance().ToggleCollision(groupA, groupB, enabled);
	}

	bool ScriptPhysics::InternalIsCollisionEnabled(uint64_t groupA, uint64_t groupB)
	{
		bool tmp__output;
		tmp__output = Physics::Instance().IsCollisionEnabled(groupA, groupB);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysics::InternalIsUpdateInProgress()
	{
		bool tmp__output;
		tmp__output = Physics::Instance().IsUpdateInProgressInternal();

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
