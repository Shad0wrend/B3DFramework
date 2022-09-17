//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCDistanceJoint.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCDistanceJoint.h"
#include "BsScriptSpring.generated.h"

namespace bs
{
	ScriptCDistanceJoint::ScriptCDistanceJoint(MonoObject* managedInstance, const GameObjectHandle<CDistanceJoint>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCDistanceJoint::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getDistance", (void*)&ScriptCDistanceJoint::InternalGetDistance);
		metaData.scriptClass->AddInternalCall("Internal_getMinDistance", (void*)&ScriptCDistanceJoint::InternalGetMinDistance);
		metaData.scriptClass->AddInternalCall("Internal_setMinDistance", (void*)&ScriptCDistanceJoint::InternalSetMinDistance);
		metaData.scriptClass->AddInternalCall("Internal_getMaxDistance", (void*)&ScriptCDistanceJoint::InternalGetMaxDistance);
		metaData.scriptClass->AddInternalCall("Internal_setMaxDistance", (void*)&ScriptCDistanceJoint::InternalSetMaxDistance);
		metaData.scriptClass->AddInternalCall("Internal_getTolerance", (void*)&ScriptCDistanceJoint::InternalGetTolerance);
		metaData.scriptClass->AddInternalCall("Internal_setTolerance", (void*)&ScriptCDistanceJoint::InternalSetTolerance);
		metaData.scriptClass->AddInternalCall("Internal_getSpring", (void*)&ScriptCDistanceJoint::InternalGetSpring);
		metaData.scriptClass->AddInternalCall("Internal_setSpring", (void*)&ScriptCDistanceJoint::InternalSetSpring);
		metaData.scriptClass->AddInternalCall("Internal_setFlag", (void*)&ScriptCDistanceJoint::InternalSetFlag);
		metaData.scriptClass->AddInternalCall("Internal_hasFlag", (void*)&ScriptCDistanceJoint::InternalHasFlag);

	}

	float ScriptCDistanceJoint::InternalGetDistance(ScriptCDistanceJoint* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptCDistanceJoint::InternalGetMinDistance(ScriptCDistanceJoint* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMinDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDistanceJoint::InternalSetMinDistance(ScriptCDistanceJoint* thisPtr, float value)
	{
		thisPtr->GetHandle()->SetMinDistance(value);
	}

	float ScriptCDistanceJoint::InternalGetMaxDistance(ScriptCDistanceJoint* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMaxDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDistanceJoint::InternalSetMaxDistance(ScriptCDistanceJoint* thisPtr, float value)
	{
		thisPtr->GetHandle()->SetMaxDistance(value);
	}

	float ScriptCDistanceJoint::InternalGetTolerance(ScriptCDistanceJoint* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetTolerance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCDistanceJoint::InternalSetTolerance(ScriptCDistanceJoint* thisPtr, float value)
	{
		thisPtr->GetHandle()->SetTolerance(value);
	}

	void ScriptCDistanceJoint::InternalGetSpring(ScriptCDistanceJoint* thisPtr, Spring* __output)
	{
		Spring tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSpring();

		*__output = tmp__output;
	}

	void ScriptCDistanceJoint::InternalSetSpring(ScriptCDistanceJoint* thisPtr, Spring* value)
	{
		thisPtr->GetHandle()->SetSpring(*value);
	}

	void ScriptCDistanceJoint::InternalSetFlag(ScriptCDistanceJoint* thisPtr, DistanceJointFlag flag, bool enabled)
	{
		thisPtr->GetHandle()->SetFlag(flag, enabled);
	}

	bool ScriptCDistanceJoint::InternalHasFlag(ScriptCDistanceJoint* thisPtr, DistanceJointFlag flag)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->HasFlag(flag);

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
