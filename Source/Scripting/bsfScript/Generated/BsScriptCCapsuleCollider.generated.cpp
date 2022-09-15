//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCCapsuleCollider.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCCapsuleCollider.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptCCapsuleCollider::ScriptCCapsuleCollider(MonoObject* managedInstance, const GameObjectHandle<CCapsuleCollider>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCCapsuleCollider::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setNormal", (void*)&ScriptCCapsuleCollider::InternalSetNormal);
		metaData.scriptClass->AddInternalCall("Internal_getNormal", (void*)&ScriptCCapsuleCollider::InternalGetNormal);
		metaData.scriptClass->AddInternalCall("Internal_setCenter", (void*)&ScriptCCapsuleCollider::InternalSetCenter);
		metaData.scriptClass->AddInternalCall("Internal_getCenter", (void*)&ScriptCCapsuleCollider::InternalGetCenter);
		metaData.scriptClass->AddInternalCall("Internal_setHalfHeight", (void*)&ScriptCCapsuleCollider::InternalSetHalfHeight);
		metaData.scriptClass->AddInternalCall("Internal_getHalfHeight", (void*)&ScriptCCapsuleCollider::InternalGetHalfHeight);
		metaData.scriptClass->AddInternalCall("Internal_setRadius", (void*)&ScriptCCapsuleCollider::InternalSetRadius);
		metaData.scriptClass->AddInternalCall("Internal_getRadius", (void*)&ScriptCCapsuleCollider::InternalGetRadius);

	}

	void ScriptCCapsuleCollider::InternalSetNormal(ScriptCCapsuleCollider* thisPtr, Vector3* normal)
	{
		thisPtr->GetHandle()->SetNormal(*normal);
	}

	void ScriptCCapsuleCollider::InternalGetNormal(ScriptCCapsuleCollider* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNormal();

		*__output = tmp__output;
	}

	void ScriptCCapsuleCollider::InternalSetCenter(ScriptCCapsuleCollider* thisPtr, Vector3* center)
	{
		thisPtr->GetHandle()->SetCenter(*center);
	}

	void ScriptCCapsuleCollider::InternalGetCenter(ScriptCCapsuleCollider* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCenter();

		*__output = tmp__output;
	}

	void ScriptCCapsuleCollider::InternalSetHalfHeight(ScriptCCapsuleCollider* thisPtr, float halfHeight)
	{
		thisPtr->GetHandle()->SetHalfHeight(halfHeight);
	}

	float ScriptCCapsuleCollider::InternalGetHalfHeight(ScriptCCapsuleCollider* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetHalfHeight();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCapsuleCollider::InternalSetRadius(ScriptCCapsuleCollider* thisPtr, float radius)
	{
		thisPtr->GetHandle()->SetRadius(radius);
	}

	float ScriptCCapsuleCollider::InternalGetRadius(ScriptCCapsuleCollider* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetRadius();

		float __output;
		__output = tmp__output;

		return __output;
	}
}
