//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCRigidbody.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCRigidbody.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptQuaternion.h"
#include "BsScriptCollisionData.generated.h"

namespace bs
{
	ScriptCRigidbody::onCollisionBeginThunkDef ScriptCRigidbody::onCollisionBeginThunk; 
	ScriptCRigidbody::onCollisionStayThunkDef ScriptCRigidbody::onCollisionStayThunk; 
	ScriptCRigidbody::onCollisionEndThunkDef ScriptCRigidbody::onCollisionEndThunk; 

	ScriptCRigidbody::ScriptCRigidbody(MonoObject* managedInstance, const GameObjectHandle<CRigidbody>& value)
		:TScriptComponent(managedInstance, value)
	{
		value->onCollisionBegin.Connect(std::bind(&ScriptCRigidbody::OnCollisionBegin, this, std::placeholders::_1));
		value->onCollisionStay.Connect(std::bind(&ScriptCRigidbody::OnCollisionStay, this, std::placeholders::_1));
		value->onCollisionEnd.Connect(std::bind(&ScriptCRigidbody::OnCollisionEnd, this, std::placeholders::_1));
	}

	void ScriptCRigidbody::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_move", (void*)&ScriptCRigidbody::InternalMove);
		metaData.scriptClass->AddInternalCall("Internal_rotate", (void*)&ScriptCRigidbody::InternalRotate);
		metaData.scriptClass->AddInternalCall("Internal_setMass", (void*)&ScriptCRigidbody::InternalSetMass);
		metaData.scriptClass->AddInternalCall("Internal_getMass", (void*)&ScriptCRigidbody::InternalGetMass);
		metaData.scriptClass->AddInternalCall("Internal_setIsKinematic", (void*)&ScriptCRigidbody::InternalSetIsKinematic);
		metaData.scriptClass->AddInternalCall("Internal_getIsKinematic", (void*)&ScriptCRigidbody::InternalGetIsKinematic);
		metaData.scriptClass->AddInternalCall("Internal_isSleeping", (void*)&ScriptCRigidbody::InternalIsSleeping);
		metaData.scriptClass->AddInternalCall("Internal_sleep", (void*)&ScriptCRigidbody::InternalSleep);
		metaData.scriptClass->AddInternalCall("Internal_wakeUp", (void*)&ScriptCRigidbody::InternalWakeUp);
		metaData.scriptClass->AddInternalCall("Internal_setSleepThreshold", (void*)&ScriptCRigidbody::InternalSetSleepThreshold);
		metaData.scriptClass->AddInternalCall("Internal_getSleepThreshold", (void*)&ScriptCRigidbody::InternalGetSleepThreshold);
		metaData.scriptClass->AddInternalCall("Internal_setUseGravity", (void*)&ScriptCRigidbody::InternalSetUseGravity);
		metaData.scriptClass->AddInternalCall("Internal_getUseGravity", (void*)&ScriptCRigidbody::InternalGetUseGravity);
		metaData.scriptClass->AddInternalCall("Internal_setVelocity", (void*)&ScriptCRigidbody::InternalSetVelocity);
		metaData.scriptClass->AddInternalCall("Internal_getVelocity", (void*)&ScriptCRigidbody::InternalGetVelocity);
		metaData.scriptClass->AddInternalCall("Internal_setAngularVelocity", (void*)&ScriptCRigidbody::InternalSetAngularVelocity);
		metaData.scriptClass->AddInternalCall("Internal_getAngularVelocity", (void*)&ScriptCRigidbody::InternalGetAngularVelocity);
		metaData.scriptClass->AddInternalCall("Internal_setDrag", (void*)&ScriptCRigidbody::InternalSetDrag);
		metaData.scriptClass->AddInternalCall("Internal_getDrag", (void*)&ScriptCRigidbody::InternalGetDrag);
		metaData.scriptClass->AddInternalCall("Internal_setAngularDrag", (void*)&ScriptCRigidbody::InternalSetAngularDrag);
		metaData.scriptClass->AddInternalCall("Internal_getAngularDrag", (void*)&ScriptCRigidbody::InternalGetAngularDrag);
		metaData.scriptClass->AddInternalCall("Internal_setInertiaTensor", (void*)&ScriptCRigidbody::InternalSetInertiaTensor);
		metaData.scriptClass->AddInternalCall("Internal_getInertiaTensor", (void*)&ScriptCRigidbody::InternalGetInertiaTensor);
		metaData.scriptClass->AddInternalCall("Internal_setMaxAngularVelocity", (void*)&ScriptCRigidbody::InternalSetMaxAngularVelocity);
		metaData.scriptClass->AddInternalCall("Internal_getMaxAngularVelocity", (void*)&ScriptCRigidbody::InternalGetMaxAngularVelocity);
		metaData.scriptClass->AddInternalCall("Internal_setCenterOfMassPosition", (void*)&ScriptCRigidbody::InternalSetCenterOfMassPosition);
		metaData.scriptClass->AddInternalCall("Internal_getCenterOfMassPosition", (void*)&ScriptCRigidbody::InternalGetCenterOfMassPosition);
		metaData.scriptClass->AddInternalCall("Internal_setCenterOfMassRotation", (void*)&ScriptCRigidbody::InternalSetCenterOfMassRotation);
		metaData.scriptClass->AddInternalCall("Internal_getCenterOfMassRotation", (void*)&ScriptCRigidbody::InternalGetCenterOfMassRotation);
		metaData.scriptClass->AddInternalCall("Internal_setPositionSolverCount", (void*)&ScriptCRigidbody::InternalSetPositionSolverCount);
		metaData.scriptClass->AddInternalCall("Internal_getPositionSolverCount", (void*)&ScriptCRigidbody::InternalGetPositionSolverCount);
		metaData.scriptClass->AddInternalCall("Internal_setVelocitySolverCount", (void*)&ScriptCRigidbody::InternalSetVelocitySolverCount);
		metaData.scriptClass->AddInternalCall("Internal_getVelocitySolverCount", (void*)&ScriptCRigidbody::InternalGetVelocitySolverCount);
		metaData.scriptClass->AddInternalCall("Internal_setCollisionReportMode", (void*)&ScriptCRigidbody::InternalSetCollisionReportMode);
		metaData.scriptClass->AddInternalCall("Internal_getCollisionReportMode", (void*)&ScriptCRigidbody::InternalGetCollisionReportMode);
		metaData.scriptClass->AddInternalCall("Internal_setFlags", (void*)&ScriptCRigidbody::InternalSetFlags);
		metaData.scriptClass->AddInternalCall("Internal_getFlags", (void*)&ScriptCRigidbody::InternalGetFlags);
		metaData.scriptClass->AddInternalCall("Internal_addForce", (void*)&ScriptCRigidbody::InternalAddForce);
		metaData.scriptClass->AddInternalCall("Internal_addTorque", (void*)&ScriptCRigidbody::InternalAddTorque);
		metaData.scriptClass->AddInternalCall("Internal_addForceAtPoint", (void*)&ScriptCRigidbody::InternalAddForceAtPoint);
		metaData.scriptClass->AddInternalCall("Internal_getVelocityAtPoint", (void*)&ScriptCRigidbody::InternalGetVelocityAtPoint);

		onCollisionBeginThunk = (onCollisionBeginThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionBegin", "CollisionData&")->GetThunk();
		onCollisionStayThunk = (onCollisionStayThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionStay", "CollisionData&")->GetThunk();
		onCollisionEndThunk = (onCollisionEndThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionEnd", "CollisionData&")->GetThunk();
	}

	void ScriptCRigidbody::OnCollisionBegin(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::InvokeThunk(onCollisionBeginThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCRigidbody::OnCollisionStay(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::InvokeThunk(onCollisionStayThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCRigidbody::OnCollisionEnd(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::InvokeThunk(onCollisionEndThunk, GetManagedInstance(), tmpp0);
	}
	void ScriptCRigidbody::InternalMove(ScriptCRigidbody* thisPtr, Vector3* position)
	{
		thisPtr->GetHandle()->move(*position);
	}

	void ScriptCRigidbody::InternalRotate(ScriptCRigidbody* thisPtr, Quaternion* rotation)
	{
		thisPtr->GetHandle()->rotate(*rotation);
	}

	void ScriptCRigidbody::InternalSetMass(ScriptCRigidbody* thisPtr, float mass)
	{
		thisPtr->GetHandle()->SetMass(mass);
	}

	float ScriptCRigidbody::InternalGetMass(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMass();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetIsKinematic(ScriptCRigidbody* thisPtr, bool kinematic)
	{
		thisPtr->GetHandle()->SetIsKinematic(kinematic);
	}

	bool ScriptCRigidbody::InternalGetIsKinematic(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetIsKinematic();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptCRigidbody::InternalIsSleeping(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->isSleeping();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSleep(ScriptCRigidbody* thisPtr)
	{
		thisPtr->GetHandle()->sleep();
	}

	void ScriptCRigidbody::InternalWakeUp(ScriptCRigidbody* thisPtr)
	{
		thisPtr->GetHandle()->wakeUp();
	}

	void ScriptCRigidbody::InternalSetSleepThreshold(ScriptCRigidbody* thisPtr, float threshold)
	{
		thisPtr->GetHandle()->SetSleepThreshold(threshold);
	}

	float ScriptCRigidbody::InternalGetSleepThreshold(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSleepThreshold();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetUseGravity(ScriptCRigidbody* thisPtr, bool gravity)
	{
		thisPtr->GetHandle()->SetUseGravity(gravity);
	}

	bool ScriptCRigidbody::InternalGetUseGravity(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetUseGravity();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetVelocity(ScriptCRigidbody* thisPtr, Vector3* velocity)
	{
		thisPtr->GetHandle()->SetVelocity(*velocity);
	}

	void ScriptCRigidbody::InternalGetVelocity(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVelocity();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetAngularVelocity(ScriptCRigidbody* thisPtr, Vector3* velocity)
	{
		thisPtr->GetHandle()->SetAngularVelocity(*velocity);
	}

	void ScriptCRigidbody::InternalGetAngularVelocity(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAngularVelocity();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetDrag(ScriptCRigidbody* thisPtr, float drag)
	{
		thisPtr->GetHandle()->SetDrag(drag);
	}

	float ScriptCRigidbody::InternalGetDrag(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDrag();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetAngularDrag(ScriptCRigidbody* thisPtr, float drag)
	{
		thisPtr->GetHandle()->SetAngularDrag(drag);
	}

	float ScriptCRigidbody::InternalGetAngularDrag(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAngularDrag();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetInertiaTensor(ScriptCRigidbody* thisPtr, Vector3* tensor)
	{
		thisPtr->GetHandle()->SetInertiaTensor(*tensor);
	}

	void ScriptCRigidbody::InternalGetInertiaTensor(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetInertiaTensor();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetMaxAngularVelocity(ScriptCRigidbody* thisPtr, float maxVelocity)
	{
		thisPtr->GetHandle()->SetMaxAngularVelocity(maxVelocity);
	}

	float ScriptCRigidbody::InternalGetMaxAngularVelocity(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMaxAngularVelocity();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetCenterOfMassPosition(ScriptCRigidbody* thisPtr, Vector3* position)
	{
		thisPtr->GetHandle()->SetCenterOfMassPosition(*position);
	}

	void ScriptCRigidbody::InternalGetCenterOfMassPosition(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCenterOfMassPosition();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetCenterOfMassRotation(ScriptCRigidbody* thisPtr, Quaternion* rotation)
	{
		thisPtr->GetHandle()->SetCenterOfMassRotation(*rotation);
	}

	void ScriptCRigidbody::InternalGetCenterOfMassRotation(ScriptCRigidbody* thisPtr, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCenterOfMassRotation();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetPositionSolverCount(ScriptCRigidbody* thisPtr, uint32_t count)
	{
		thisPtr->GetHandle()->SetPositionSolverCount(count);
	}

	uint32_t ScriptCRigidbody::InternalGetPositionSolverCount(ScriptCRigidbody* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetPositionSolverCount();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetVelocitySolverCount(ScriptCRigidbody* thisPtr, uint32_t count)
	{
		thisPtr->GetHandle()->SetVelocitySolverCount(count);
	}

	uint32_t ScriptCRigidbody::InternalGetVelocitySolverCount(ScriptCRigidbody* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVelocitySolverCount();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetCollisionReportMode(ScriptCRigidbody* thisPtr, CollisionReportMode mode)
	{
		thisPtr->GetHandle()->SetCollisionReportMode(mode);
	}

	CollisionReportMode ScriptCRigidbody::InternalGetCollisionReportMode(ScriptCRigidbody* thisPtr)
	{
		CollisionReportMode tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCollisionReportMode();

		CollisionReportMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetFlags(ScriptCRigidbody* thisPtr, RigidbodyFlag flags)
	{
		thisPtr->GetHandle()->SetFlags(flags);
	}

	RigidbodyFlag ScriptCRigidbody::InternalGetFlags(ScriptCRigidbody* thisPtr)
	{
		RigidbodyFlag tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFlags();

		RigidbodyFlag __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalAddForce(ScriptCRigidbody* thisPtr, Vector3* force, ForceMode mode)
	{
		thisPtr->GetHandle()->addForce(*force, mode);
	}

	void ScriptCRigidbody::InternalAddTorque(ScriptCRigidbody* thisPtr, Vector3* torque, ForceMode mode)
	{
		thisPtr->GetHandle()->addTorque(*torque, mode);
	}

	void ScriptCRigidbody::InternalAddForceAtPoint(ScriptCRigidbody* thisPtr, Vector3* force, Vector3* position, PointForceMode mode)
	{
		thisPtr->GetHandle()->addForceAtPoint(*force, *position, mode);
	}

	void ScriptCRigidbody::InternalGetVelocityAtPoint(ScriptCRigidbody* thisPtr, Vector3* point, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVelocityAtPoint(*point);

		*__output = tmp__output;
	}
}
