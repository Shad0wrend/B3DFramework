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
		metaData.scriptClass->addInternalCall("Internal_getAngularVelocity", (void*)&ScriptCRigidbody::InternalGetAngularVelocity);
		metaData.scriptClass->addInternalCall("Internal_setDrag", (void*)&ScriptCRigidbody::InternalSetDrag);
		metaData.scriptClass->addInternalCall("Internal_getDrag", (void*)&ScriptCRigidbody::InternalGetDrag);
		metaData.scriptClass->addInternalCall("Internal_setAngularDrag", (void*)&ScriptCRigidbody::InternalSetAngularDrag);
		metaData.scriptClass->addInternalCall("Internal_getAngularDrag", (void*)&ScriptCRigidbody::InternalGetAngularDrag);
		metaData.scriptClass->addInternalCall("Internal_setInertiaTensor", (void*)&ScriptCRigidbody::InternalSetInertiaTensor);
		metaData.scriptClass->addInternalCall("Internal_getInertiaTensor", (void*)&ScriptCRigidbody::InternalGetInertiaTensor);
		metaData.scriptClass->addInternalCall("Internal_setMaxAngularVelocity", (void*)&ScriptCRigidbody::InternalSetMaxAngularVelocity);
		metaData.scriptClass->addInternalCall("Internal_getMaxAngularVelocity", (void*)&ScriptCRigidbody::InternalGetMaxAngularVelocity);
		metaData.scriptClass->addInternalCall("Internal_setCenterOfMassPosition", (void*)&ScriptCRigidbody::InternalSetCenterOfMassPosition);
		metaData.scriptClass->addInternalCall("Internal_getCenterOfMassPosition", (void*)&ScriptCRigidbody::InternalGetCenterOfMassPosition);
		metaData.scriptClass->addInternalCall("Internal_setCenterOfMassRotation", (void*)&ScriptCRigidbody::InternalSetCenterOfMassRotation);
		metaData.scriptClass->addInternalCall("Internal_getCenterOfMassRotation", (void*)&ScriptCRigidbody::InternalGetCenterOfMassRotation);
		metaData.scriptClass->addInternalCall("Internal_setPositionSolverCount", (void*)&ScriptCRigidbody::InternalSetPositionSolverCount);
		metaData.scriptClass->addInternalCall("Internal_getPositionSolverCount", (void*)&ScriptCRigidbody::InternalGetPositionSolverCount);
		metaData.scriptClass->addInternalCall("Internal_setVelocitySolverCount", (void*)&ScriptCRigidbody::InternalSetVelocitySolverCount);
		metaData.scriptClass->addInternalCall("Internal_getVelocitySolverCount", (void*)&ScriptCRigidbody::InternalGetVelocitySolverCount);
		metaData.scriptClass->addInternalCall("Internal_setCollisionReportMode", (void*)&ScriptCRigidbody::InternalSetCollisionReportMode);
		metaData.scriptClass->addInternalCall("Internal_getCollisionReportMode", (void*)&ScriptCRigidbody::InternalGetCollisionReportMode);
		metaData.scriptClass->addInternalCall("Internal_setFlags", (void*)&ScriptCRigidbody::InternalSetFlags);
		metaData.scriptClass->addInternalCall("Internal_getFlags", (void*)&ScriptCRigidbody::InternalGetFlags);
		metaData.scriptClass->addInternalCall("Internal_addForce", (void*)&ScriptCRigidbody::InternalAddForce);
		metaData.scriptClass->addInternalCall("Internal_addTorque", (void*)&ScriptCRigidbody::InternalAddTorque);
		metaData.scriptClass->addInternalCall("Internal_addForceAtPoint", (void*)&ScriptCRigidbody::InternalAddForceAtPoint);
		metaData.scriptClass->addInternalCall("Internal_getVelocityAtPoint", (void*)&ScriptCRigidbody::InternalGetVelocityAtPoint);

		onCollisionBeginThunk = (onCollisionBeginThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionBegin", "CollisionData&")->getThunk();
		onCollisionStayThunk = (onCollisionStayThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionStay", "CollisionData&")->getThunk();
		onCollisionEndThunk = (onCollisionEndThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionEnd", "CollisionData&")->getThunk();
	}

	void ScriptCRigidbody::OnCollisionBegin(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::invokeThunk(onCollisionBeginThunk, getManagedInstance(), tmpp0);
	}

	void ScriptCRigidbody::OnCollisionStay(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::invokeThunk(onCollisionStayThunk, getManagedInstance(), tmpp0);
	}

	void ScriptCRigidbody::OnCollisionEnd(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::toInterop(p0);
		tmpp0 = ScriptCollisionData::box(interopp0);
		MonoUtil::invokeThunk(onCollisionEndThunk, getManagedInstance(), tmpp0);
	}
	void ScriptCRigidbody::InternalMove(ScriptCRigidbody* thisPtr, Vector3* position)
	{
		thisPtr->getHandle()->move(*position);
	}

	void ScriptCRigidbody::InternalRotate(ScriptCRigidbody* thisPtr, Quaternion* rotation)
	{
		thisPtr->getHandle()->rotate(*rotation);
	}

	void ScriptCRigidbody::InternalSetMass(ScriptCRigidbody* thisPtr, float mass)
	{
		thisPtr->getHandle()->setMass(mass);
	}

	float ScriptCRigidbody::InternalGetMass(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getMass();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetIsKinematic(ScriptCRigidbody* thisPtr, bool kinematic)
	{
		thisPtr->getHandle()->setIsKinematic(kinematic);
	}

	bool ScriptCRigidbody::InternalGetIsKinematic(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getIsKinematic();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptCRigidbody::InternalIsSleeping(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->isSleeping();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSleep(ScriptCRigidbody* thisPtr)
	{
		thisPtr->getHandle()->sleep();
	}

	void ScriptCRigidbody::InternalWakeUp(ScriptCRigidbody* thisPtr)
	{
		thisPtr->getHandle()->wakeUp();
	}

	void ScriptCRigidbody::InternalSetSleepThreshold(ScriptCRigidbody* thisPtr, float threshold)
	{
		thisPtr->getHandle()->setSleepThreshold(threshold);
	}

	float ScriptCRigidbody::InternalGetSleepThreshold(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getSleepThreshold();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetUseGravity(ScriptCRigidbody* thisPtr, bool gravity)
	{
		thisPtr->getHandle()->setUseGravity(gravity);
	}

	bool ScriptCRigidbody::InternalGetUseGravity(ScriptCRigidbody* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getUseGravity();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetVelocity(ScriptCRigidbody* thisPtr, Vector3* velocity)
	{
		thisPtr->getHandle()->setVelocity(*velocity);
	}

	void ScriptCRigidbody::InternalGetVelocity(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getHandle()->getVelocity();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetAngularVelocity(ScriptCRigidbody* thisPtr, Vector3* velocity)
	{
		thisPtr->getHandle()->setAngularVelocity(*velocity);
	}

	void ScriptCRigidbody::InternalGetAngularVelocity(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getHandle()->getAngularVelocity();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetDrag(ScriptCRigidbody* thisPtr, float drag)
	{
		thisPtr->getHandle()->setDrag(drag);
	}

	float ScriptCRigidbody::InternalGetDrag(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getDrag();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetAngularDrag(ScriptCRigidbody* thisPtr, float drag)
	{
		thisPtr->getHandle()->setAngularDrag(drag);
	}

	float ScriptCRigidbody::InternalGetAngularDrag(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getAngularDrag();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetInertiaTensor(ScriptCRigidbody* thisPtr, Vector3* tensor)
	{
		thisPtr->getHandle()->setInertiaTensor(*tensor);
	}

	void ScriptCRigidbody::InternalGetInertiaTensor(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getHandle()->getInertiaTensor();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetMaxAngularVelocity(ScriptCRigidbody* thisPtr, float maxVelocity)
	{
		thisPtr->getHandle()->setMaxAngularVelocity(maxVelocity);
	}

	float ScriptCRigidbody::InternalGetMaxAngularVelocity(ScriptCRigidbody* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getMaxAngularVelocity();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetCenterOfMassPosition(ScriptCRigidbody* thisPtr, Vector3* position)
	{
		thisPtr->getHandle()->setCenterOfMassPosition(*position);
	}

	void ScriptCRigidbody::InternalGetCenterOfMassPosition(ScriptCRigidbody* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getHandle()->getCenterOfMassPosition();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetCenterOfMassRotation(ScriptCRigidbody* thisPtr, Quaternion* rotation)
	{
		thisPtr->getHandle()->setCenterOfMassRotation(*rotation);
	}

	void ScriptCRigidbody::InternalGetCenterOfMassRotation(ScriptCRigidbody* thisPtr, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = thisPtr->getHandle()->getCenterOfMassRotation();

		*__output = tmp__output;
	}

	void ScriptCRigidbody::InternalSetPositionSolverCount(ScriptCRigidbody* thisPtr, uint32_t count)
	{
		thisPtr->getHandle()->setPositionSolverCount(count);
	}

	uint32_t ScriptCRigidbody::InternalGetPositionSolverCount(ScriptCRigidbody* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getPositionSolverCount();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetVelocitySolverCount(ScriptCRigidbody* thisPtr, uint32_t count)
	{
		thisPtr->getHandle()->setVelocitySolverCount(count);
	}

	uint32_t ScriptCRigidbody::InternalGetVelocitySolverCount(ScriptCRigidbody* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getVelocitySolverCount();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetCollisionReportMode(ScriptCRigidbody* thisPtr, CollisionReportMode mode)
	{
		thisPtr->getHandle()->setCollisionReportMode(mode);
	}

	CollisionReportMode ScriptCRigidbody::InternalGetCollisionReportMode(ScriptCRigidbody* thisPtr)
	{
		CollisionReportMode tmp__output;
		tmp__output = thisPtr->getHandle()->getCollisionReportMode();

		CollisionReportMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalSetFlags(ScriptCRigidbody* thisPtr, RigidbodyFlag flags)
	{
		thisPtr->getHandle()->setFlags(flags);
	}

	RigidbodyFlag ScriptCRigidbody::InternalGetFlags(ScriptCRigidbody* thisPtr)
	{
		RigidbodyFlag tmp__output;
		tmp__output = thisPtr->getHandle()->getFlags();

		RigidbodyFlag __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRigidbody::InternalAddForce(ScriptCRigidbody* thisPtr, Vector3* force, ForceMode mode)
	{
		thisPtr->getHandle()->addForce(*force, mode);
	}

	void ScriptCRigidbody::InternalAddTorque(ScriptCRigidbody* thisPtr, Vector3* torque, ForceMode mode)
	{
		thisPtr->getHandle()->addTorque(*torque, mode);
	}

	void ScriptCRigidbody::InternalAddForceAtPoint(ScriptCRigidbody* thisPtr, Vector3* force, Vector3* position, PointForceMode mode)
	{
		thisPtr->getHandle()->addForceAtPoint(*force, *position, mode);
	}

	void ScriptCRigidbody::InternalGetVelocityAtPoint(ScriptCRigidbody* thisPtr, Vector3* point, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getHandle()->getVelocityAtPoint(*point);

		*__output = tmp__output;
	}
}
