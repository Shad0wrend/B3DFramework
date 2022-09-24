//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptControllerColliderCollision.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptGameObjectManager.h"
#include "../../../Foundation/bsfCore/Components/BsCCollider.h"
#include "BsScriptCCollider.generated.h"
#include "Math/BsVector3.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptControllerColliderCollision::ScriptControllerColliderCollision(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptControllerColliderCollision::InitRuntimeData()
	{ }

	MonoObject*ScriptControllerColliderCollision::Box(const __ControllerColliderCollisionInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__ControllerColliderCollisionInterop ScriptControllerColliderCollision::Unbox(MonoObject* value)
	{
		return *(__ControllerColliderCollisionInterop*)MonoUtil::Unbox(value);
	}

	ControllerColliderCollision ScriptControllerColliderCollision::FromInterop(const __ControllerColliderCollisionInterop& value)
	{
		ControllerColliderCollision output;
		GameObjectHandle<CCollider> tmpcollider;
		ScriptCColliderBase* scriptcollider;
		scriptcollider = (ScriptCColliderBase*)ScriptCCollider::ToNative(value.Collider);
		if(scriptcollider != nullptr)
			tmpcollider = static_object_cast<CCollider>(scriptcollider->GetComponent());
		output.Collider = tmpcollider;
		output.TriangleIndex = value.TriangleIndex;
		output.Position = value.Position;
		output.Normal = value.Normal;
		output.MotionDir = value.MotionDir;
		output.MotionAmount = value.MotionAmount;

		return output;
	}

	__ControllerColliderCollisionInterop ScriptControllerColliderCollision::ToInterop(const ControllerColliderCollision& value)
	{
		__ControllerColliderCollisionInterop output;
		ScriptComponentBase* scriptcollider = nullptr;
		if(value.Collider)
			scriptcollider = ScriptGameObjectManager::Instance().GetBuiltinScriptComponent(static_object_cast<Component>(value.Collider));
		MonoObject* tmpcollider;
		if(scriptcollider != nullptr)
			tmpcollider = scriptcollider->GetManagedInstance();
		else
			tmpcollider = nullptr;
		output.Collider = tmpcollider;
		output.TriangleIndex = value.TriangleIndex;
		output.Position = value.Position;
		output.Normal = value.Normal;
		output.MotionDir = value.MotionDir;
		output.MotionAmount = value.MotionAmount;

		return output;
	}

}
