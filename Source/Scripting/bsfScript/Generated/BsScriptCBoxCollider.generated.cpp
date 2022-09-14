//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCBoxCollider.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCBoxCollider.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptCBoxCollider::ScriptCBoxCollider(MonoObject* managedInstance, const GameObjectHandle<CBoxCollider>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCBoxCollider::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setExtents", (void*)&ScriptCBoxCollider::InternalSetExtents);
		metaData.scriptClass->AddInternalCall("Internal_getExtents", (void*)&ScriptCBoxCollider::InternalGetExtents);
		metaData.scriptClass->AddInternalCall("Internal_setCenter", (void*)&ScriptCBoxCollider::InternalSetCenter);
		metaData.scriptClass->AddInternalCall("Internal_getCenter", (void*)&ScriptCBoxCollider::InternalGetCenter);

	}

	void ScriptCBoxCollider::InternalSetExtents(ScriptCBoxCollider* thisPtr, Vector3* extents)
	{
		thisPtr->GetHandle()->SetExtents(*extents);
	}

	void ScriptCBoxCollider::InternalGetExtents(ScriptCBoxCollider* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetExtents();

		*__output = tmp__output;
	}

	void ScriptCBoxCollider::InternalSetCenter(ScriptCBoxCollider* thisPtr, Vector3* center)
	{
		thisPtr->GetHandle()->SetCenter(*center);
	}

	void ScriptCBoxCollider::InternalGetCenter(ScriptCBoxCollider* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCenter();

		*__output = tmp__output;
	}
}
