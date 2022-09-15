//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCD6Joint.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCD6Joint.h"
#include "BsScriptD6JointDrive.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptLimitLinear.generated.h"
#include "BsScriptLimitAngularRange.generated.h"
#include "BsScriptLimitConeRange.generated.h"
#include "Wrappers/BsScriptQuaternion.h"

namespace bs
{
	ScriptCD6Joint::ScriptCD6Joint(MonoObject* managedInstance, const GameObjectHandle<CD6Joint>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCD6Joint::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getMotion", (void*)&ScriptCD6Joint::InternalGetMotion);
		metaData.scriptClass->AddInternalCall("Internal_setMotion", (void*)&ScriptCD6Joint::InternalSetMotion);
		metaData.scriptClass->AddInternalCall("Internal_getTwist", (void*)&ScriptCD6Joint::InternalGetTwist);
		metaData.scriptClass->AddInternalCall("Internal_getSwingY", (void*)&ScriptCD6Joint::InternalGetSwingY);
		metaData.scriptClass->AddInternalCall("Internal_getSwingZ", (void*)&ScriptCD6Joint::InternalGetSwingZ);
		metaData.scriptClass->AddInternalCall("Internal_getLimitLinear", (void*)&ScriptCD6Joint::InternalGetLimitLinear);
		metaData.scriptClass->AddInternalCall("Internal_setLimitLinear", (void*)&ScriptCD6Joint::InternalSetLimitLinear);
		metaData.scriptClass->AddInternalCall("Internal_getLimitTwist", (void*)&ScriptCD6Joint::InternalGetLimitTwist);
		metaData.scriptClass->AddInternalCall("Internal_setLimitTwist", (void*)&ScriptCD6Joint::InternalSetLimitTwist);
		metaData.scriptClass->AddInternalCall("Internal_getLimitSwing", (void*)&ScriptCD6Joint::InternalGetLimitSwing);
		metaData.scriptClass->AddInternalCall("Internal_setLimitSwing", (void*)&ScriptCD6Joint::InternalSetLimitSwing);
		metaData.scriptClass->AddInternalCall("Internal_getDrive", (void*)&ScriptCD6Joint::InternalGetDrive);
		metaData.scriptClass->AddInternalCall("Internal_setDrive", (void*)&ScriptCD6Joint::InternalSetDrive);
		metaData.scriptClass->AddInternalCall("Internal_getDrivePosition", (void*)&ScriptCD6Joint::InternalGetDrivePosition);
		metaData.scriptClass->AddInternalCall("Internal_getDriveRotation", (void*)&ScriptCD6Joint::InternalGetDriveRotation);
		metaData.scriptClass->AddInternalCall("Internal_setDriveTransform", (void*)&ScriptCD6Joint::InternalSetDriveTransform);
		metaData.scriptClass->AddInternalCall("Internal_getDriveLinearVelocity", (void*)&ScriptCD6Joint::InternalGetDriveLinearVelocity);
		metaData.scriptClass->AddInternalCall("Internal_getDriveAngularVelocity", (void*)&ScriptCD6Joint::InternalGetDriveAngularVelocity);
		metaData.scriptClass->AddInternalCall("Internal_setDriveVelocity", (void*)&ScriptCD6Joint::InternalSetDriveVelocity);

	}

	D6JointMotion ScriptCD6Joint::InternalGetMotion(ScriptCD6Joint* thisPtr, D6JointAxis axis)
	{
		D6JointMotion tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMotion(axis);

		D6JointMotion __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCD6Joint::InternalSetMotion(ScriptCD6Joint* thisPtr, D6JointAxis axis, D6JointMotion motion)
	{
		thisPtr->GetHandle()->SetMotion(axis, motion);
	}

	void ScriptCD6Joint::InternalGetTwist(ScriptCD6Joint* thisPtr, Radian* __output)
	{
		Radian tmp__output;
		tmp__output = thisPtr->GetHandle()->GetTwist();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalGetSwingY(ScriptCD6Joint* thisPtr, Radian* __output)
	{
		Radian tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSwingY();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalGetSwingZ(ScriptCD6Joint* thisPtr, Radian* __output)
	{
		Radian tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSwingZ();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalGetLimitLinear(ScriptCD6Joint* thisPtr, __LimitLinearInterop* __output)
	{
		LimitLinear tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLimitLinear();

		__LimitLinearInterop interop__output;
		interop__output = ScriptLimitLinear::ToInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptLimitLinear::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptCD6Joint::InternalSetLimitLinear(ScriptCD6Joint* thisPtr, __LimitLinearInterop* limit)
	{
		LimitLinear tmplimit;
		tmplimit = ScriptLimitLinear::FromInterop(*limit);
		thisPtr->GetHandle()->SetLimitLinear(tmplimit);
	}

	void ScriptCD6Joint::InternalGetLimitTwist(ScriptCD6Joint* thisPtr, __LimitAngularRangeInterop* __output)
	{
		LimitAngularRange tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLimitTwist();

		__LimitAngularRangeInterop interop__output;
		interop__output = ScriptLimitAngularRange::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptLimitAngularRange::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptCD6Joint::InternalSetLimitTwist(ScriptCD6Joint* thisPtr, __LimitAngularRangeInterop* limit)
	{
		LimitAngularRange tmplimit;
		tmplimit = ScriptLimitAngularRange::fromInterop(*limit);
		thisPtr->GetHandle()->SetLimitTwist(tmplimit);
	}

	void ScriptCD6Joint::InternalGetLimitSwing(ScriptCD6Joint* thisPtr, __LimitConeRangeInterop* __output)
	{
		LimitConeRange tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLimitSwing();

		__LimitConeRangeInterop interop__output;
		interop__output = ScriptLimitConeRange::ToInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptLimitConeRange::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptCD6Joint::InternalSetLimitSwing(ScriptCD6Joint* thisPtr, __LimitConeRangeInterop* limit)
	{
		LimitConeRange tmplimit;
		tmplimit = ScriptLimitConeRange::FromInterop(*limit);
		thisPtr->GetHandle()->SetLimitSwing(tmplimit);
	}

	void ScriptCD6Joint::InternalGetDrive(ScriptCD6Joint* thisPtr, D6JointDriveType type, D6JointDrive* __output)
	{
		D6JointDrive tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDrive(type);

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalSetDrive(ScriptCD6Joint* thisPtr, D6JointDriveType type, D6JointDrive* drive)
	{
		thisPtr->GetHandle()->SetDrive(type, *drive);
	}

	void ScriptCD6Joint::InternalGetDrivePosition(ScriptCD6Joint* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDrivePosition();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalGetDriveRotation(ScriptCD6Joint* thisPtr, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDriveRotation();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalSetDriveTransform(ScriptCD6Joint* thisPtr, Vector3* position, Quaternion* rotation)
	{
		thisPtr->GetHandle()->SetDriveTransform(*position, *rotation);
	}

	void ScriptCD6Joint::InternalGetDriveLinearVelocity(ScriptCD6Joint* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDriveLinearVelocity();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalGetDriveAngularVelocity(ScriptCD6Joint* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDriveAngularVelocity();

		*__output = tmp__output;
	}

	void ScriptCD6Joint::InternalSetDriveVelocity(ScriptCD6Joint* thisPtr, Vector3* linear, Vector3* angular)
	{
		thisPtr->GetHandle()->SetDriveVelocity(*linear, *angular);
	}
}
