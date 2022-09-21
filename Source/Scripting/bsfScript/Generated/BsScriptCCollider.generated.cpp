//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCCollider.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCCollider.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "BsScriptCollisionData.generated.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMaterial.h"

namespace bs
{
	ScriptCColliderBase::ScriptCColliderBase(MonoObject* managedInstance)
		:ScriptComponentBase(managedInstance)
	 { }

	ScriptCCollider::onCollisionBeginThunkDef ScriptCCollider::onCollisionBeginThunk; 
	ScriptCCollider::onCollisionStayThunkDef ScriptCCollider::onCollisionStayThunk; 
	ScriptCCollider::onCollisionEndThunkDef ScriptCCollider::onCollisionEndThunk; 

	ScriptCCollider::ScriptCCollider(MonoObject* managedInstance, const GameObjectHandle<CCollider>& value)
		:TScriptComponent(managedInstance, value)
	{
		value->onCollisionBegin.Connect(std::bind(&ScriptCCollider::onCollisionBegin, this, std::placeholders::_1));
		value->onCollisionStay.Connect(std::bind(&ScriptCCollider::onCollisionStay, this, std::placeholders::_1));
		value->onCollisionEnd.Connect(std::bind(&ScriptCCollider::onCollisionEnd, this, std::placeholders::_1));
	}

	void ScriptCCollider::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_SetIsTrigger", (void*)&ScriptCCollider::InternalSetIsTrigger);
		metaData.scriptClass->AddInternalCall("Internal_GetIsTrigger", (void*)&ScriptCCollider::InternalGetIsTrigger);
		metaData.scriptClass->AddInternalCall("Internal_SetMass", (void*)&ScriptCCollider::InternalSetMass);
		metaData.scriptClass->AddInternalCall("Internal_GetMass", (void*)&ScriptCCollider::InternalGetMass);
		metaData.scriptClass->AddInternalCall("Internal_SetMaterial", (void*)&ScriptCCollider::InternalSetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_GetMaterial", (void*)&ScriptCCollider::InternalGetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_SetContactOffset", (void*)&ScriptCCollider::InternalSetContactOffset);
		metaData.scriptClass->AddInternalCall("Internal_GetContactOffset", (void*)&ScriptCCollider::InternalGetContactOffset);
		metaData.scriptClass->AddInternalCall("Internal_SetRestOffset", (void*)&ScriptCCollider::InternalSetRestOffset);
		metaData.scriptClass->AddInternalCall("Internal_GetRestOffset", (void*)&ScriptCCollider::InternalGetRestOffset);
		metaData.scriptClass->AddInternalCall("Internal_SetLayer", (void*)&ScriptCCollider::InternalSetLayer);
		metaData.scriptClass->AddInternalCall("Internal_GetLayer", (void*)&ScriptCCollider::InternalGetLayer);
		metaData.scriptClass->AddInternalCall("Internal_SetCollisionReportMode", (void*)&ScriptCCollider::InternalSetCollisionReportMode);
		metaData.scriptClass->AddInternalCall("Internal_GetCollisionReportMode", (void*)&ScriptCCollider::InternalGetCollisionReportMode);

		onCollisionBeginThunk = (onCollisionBeginThunkDef)metaData.scriptClass->GetMethodExact("InternalonCollisionBegin", "CollisionData&")->GetThunk();
		onCollisionStayThunk = (onCollisionStayThunkDef)metaData.scriptClass->GetMethodExact("InternalonCollisionStay", "CollisionData&")->GetThunk();
		onCollisionEndThunk = (onCollisionEndThunkDef)metaData.scriptClass->GetMethodExact("InternalonCollisionEnd", "CollisionData&")->GetThunk();
	}

	void ScriptCCollider::onCollisionBegin(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::InvokeThunk(onCollisionBeginThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCCollider::onCollisionStay(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::InvokeThunk(onCollisionStayThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCCollider::onCollisionEnd(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::InvokeThunk(onCollisionEndThunk, GetManagedInstance(), tmpp0);
	}
	void ScriptCCollider::InternalSetIsTrigger(ScriptCColliderBase* thisPtr, bool value)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetIsTrigger(value);
	}

	bool ScriptCCollider::InternalGetIsTrigger(ScriptCColliderBase* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetIsTrigger();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetMass(ScriptCColliderBase* thisPtr, float mass)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetMass(mass);
	}

	float ScriptCCollider::InternalGetMass(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetMass();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetMaterial(ScriptCColliderBase* thisPtr, MonoObject* material)
	{
		ResourceHandle<PhysicsMaterial> tmpmaterial;
		ScriptRRefBase* scriptmaterial;
		scriptmaterial = ScriptRRefBase::ToNative(material);
		if(scriptmaterial != nullptr)
			tmpmaterial = static_resource_cast<PhysicsMaterial>(scriptmaterial->GetHandle());
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetMaterial(tmpmaterial);
	}

	MonoObject* ScriptCCollider::InternalGetMaterial(ScriptCColliderBase* thisPtr)
	{
		ResourceHandle<PhysicsMaterial> tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetMaterial();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCCollider::InternalSetContactOffset(ScriptCColliderBase* thisPtr, float value)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetContactOffset(value);
	}

	float ScriptCCollider::InternalGetContactOffset(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetContactOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetRestOffset(ScriptCColliderBase* thisPtr, float value)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetRestOffset(value);
	}

	float ScriptCCollider::InternalGetRestOffset(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetRestOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetLayer(ScriptCColliderBase* thisPtr, uint64_t layer)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetLayer(layer);
	}

	uint64_t ScriptCCollider::InternalGetLayer(ScriptCColliderBase* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetLayer();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetCollisionReportMode(ScriptCColliderBase* thisPtr, CollisionReportMode mode)
	{
		static_object_cast<CCollider>(thisPtr->GetComponent())->SetCollisionReportMode(mode);
	}

	CollisionReportMode ScriptCCollider::InternalGetCollisionReportMode(ScriptCColliderBase* thisPtr)
	{
		CollisionReportMode tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->GetComponent())->GetCollisionReportMode();

		CollisionReportMode __output;
		__output = tmp__output;

		return __output;
	}
}
