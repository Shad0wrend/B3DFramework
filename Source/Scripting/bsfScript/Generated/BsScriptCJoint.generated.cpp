//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCJoint.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCJoint.h"
#include "BsScriptGameObjectManager.h"
#include "BsScriptCRigidbody.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptQuaternion.h"

namespace bs
{
	ScriptCJointBase::ScriptCJointBase(MonoObject* managedInstance)
		:ScriptComponentBase(managedInstance)
	 { }

	ScriptCJoint::onJointBreakThunkDef ScriptCJoint::onJointBreakThunk; 

	ScriptCJoint::ScriptCJoint(MonoObject* managedInstance, const GameObjectHandle<CJoint>& value)
		:TScriptComponent(managedInstance, value)
	{
		value->onJointBreak.Connect(std::bind(&::bs::ScriptCJoint::OnJointBreak, this));
	}

	void ScriptCJoint::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getBody", (void*)&ScriptCJoint::Internal_getBody);
		metaData.scriptClass->addInternalCall("Internal_setBody", (void*)&ScriptCJoint::Internal_setBody);
		metaData.scriptClass->AddInternalCall("Internal_getPosition", (void*)&ScriptCJoint::InternalGetPosition);
		metaData.scriptClass->AddInternalCall("Internal_getRotation", (void*)&ScriptCJoint::InternalGetRotation);
		metaData.scriptClass->AddInternalCall("Internal_setTransform", (void*)&ScriptCJoint::InternalSetTransform);
		metaData.scriptClass->addInternalCall("Internal_getBreakForce", (void*)&ScriptCJoint::Internal_getBreakForce);
		metaData.scriptClass->addInternalCall("Internal_setBreakForce", (void*)&ScriptCJoint::Internal_setBreakForce);
		metaData.scriptClass->addInternalCall("Internal_getBreakTorque", (void*)&ScriptCJoint::Internal_getBreakTorque);
		metaData.scriptClass->addInternalCall("Internal_setBreakTorque", (void*)&ScriptCJoint::Internal_setBreakTorque);
		metaData.scriptClass->addInternalCall("Internal_getEnableCollision", (void*)&ScriptCJoint::Internal_getEnableCollision);
		metaData.scriptClass->addInternalCall("Internal_setEnableCollision", (void*)&ScriptCJoint::Internal_setEnableCollision);

		onJointBreakThunk = (onJointBreakThunkDef)metaData.scriptClass->getMethodExact("Internal_onJointBreak", "")->getThunk();
	}

	void ScriptCJoint::OnJointBreak()
	{
		MonoUtil::invokeThunk(onJointBreakThunk, getManagedInstance());
	}
	MonoObject* ScriptCJoint::InternalGetBody(ScriptCJointBase* thisPtr, JointBody body)
	{
		GameObjectHandle<CRigidbody> tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getBody(body);

		MonoObject* __output;
		ScriptComponentBase* script__output = nullptr;
		if(tmp__output)
			script__output = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(tmp__output));
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCJoint::InternalSetBody(ScriptCJointBase* thisPtr, JointBody body, MonoObject* value)
	{
		GameObjectHandle<CRigidbody> tmpvalue;
		ScriptCRigidbody* scriptvalue;
		scriptvalue = ScriptCRigidbody::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getHandle();
		static_object_cast<CJoint>(thisPtr->getComponent())->setBody(body, tmpvalue);
	}

	void ScriptCJoint::InternalGetPosition(ScriptCJointBase* thisPtr, JointBody body, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getPosition(body);

		*__output = tmp__output;
	}

	void ScriptCJoint::InternalGetRotation(ScriptCJointBase* thisPtr, JointBody body, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getRotation(body);

		*__output = tmp__output;
	}

	void ScriptCJoint::InternalSetTransform(ScriptCJointBase* thisPtr, JointBody body, Vector3* position, Quaternion* rotation)
	{
		static_object_cast<CJoint>(thisPtr->getComponent())->setTransform(body, *position, *rotation);
	}

	float ScriptCJoint::InternalGetBreakForce(ScriptCJointBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getBreakForce();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCJoint::InternalSetBreakForce(ScriptCJointBase* thisPtr, float force)
	{
		static_object_cast<CJoint>(thisPtr->getComponent())->setBreakForce(force);
	}

	float ScriptCJoint::InternalGetBreakTorque(ScriptCJointBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getBreakTorque();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCJoint::InternalSetBreakTorque(ScriptCJointBase* thisPtr, float torque)
	{
		static_object_cast<CJoint>(thisPtr->getComponent())->setBreakTorque(torque);
	}

	bool ScriptCJoint::InternalGetEnableCollision(ScriptCJointBase* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_object_cast<CJoint>(thisPtr->getComponent())->getEnableCollision();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCJoint::InternalSetEnableCollision(ScriptCJointBase* thisPtr, bool value)
	{
		static_object_cast<CJoint>(thisPtr->getComponent())->setEnableCollision(value);
	}
}
