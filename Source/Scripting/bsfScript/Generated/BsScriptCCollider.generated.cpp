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
		value->onCollisionBegin.Connect(std::bind(&::bs::ScriptCCollider::OnCollisionBegin, this, std::placeholders::_1));
		value->onCollisionStay.Connect(std::bind(&::bs::ScriptCCollider::OnCollisionStay, this, std::placeholders::_1));
		value->onCollisionEnd.Connect(std::bind(&::bs::ScriptCCollider::OnCollisionEnd, this, std::placeholders::_1));
	}

	void ScriptCCollider::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setIsTrigger", (void*)&ScriptCCollider::InternalSetIsTrigger);
		metaData.scriptClass->AddInternalCall("Internal_getIsTrigger", (void*)&ScriptCCollider::InternalGetIsTrigger);
		metaData.scriptClass->AddInternalCall("Internal_setMass", (void*)&ScriptCCollider::InternalSetMass);
		metaData.scriptClass->AddInternalCall("Internal_getMass", (void*)&ScriptCCollider::InternalGetMass);
		metaData.scriptClass->AddInternalCall("Internal_setMaterial", (void*)&ScriptCCollider::InternalSetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_getMaterial", (void*)&ScriptCCollider::InternalGetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_setContactOffset", (void*)&ScriptCCollider::InternalSetContactOffset);
		metaData.scriptClass->AddInternalCall("Internal_getContactOffset", (void*)&ScriptCCollider::InternalGetContactOffset);
		metaData.scriptClass->AddInternalCall("Internal_setRestOffset", (void*)&ScriptCCollider::InternalSetRestOffset);
		metaData.scriptClass->AddInternalCall("Internal_getRestOffset", (void*)&ScriptCCollider::InternalGetRestOffset);
		metaData.scriptClass->AddInternalCall("Internal_setLayer", (void*)&ScriptCCollider::InternalSetLayer);
		metaData.scriptClass->AddInternalCall("Internal_getLayer", (void*)&ScriptCCollider::InternalGetLayer);
		metaData.scriptClass->AddInternalCall("Internal_setCollisionReportMode", (void*)&ScriptCCollider::InternalSetCollisionReportMode);
		metaData.scriptClass->AddInternalCall("Internal_getCollisionReportMode", (void*)&ScriptCCollider::InternalGetCollisionReportMode);

		onCollisionBeginThunk = (onCollisionBeginThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionBegin", "CollisionData&")->GetThunk();
		onCollisionStayThunk = (onCollisionStayThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionStay", "CollisionData&")->GetThunk();
		onCollisionEndThunk = (onCollisionEndThunkDef)metaData.scriptClass->GetMethodExact("Internal_onCollisionEnd", "CollisionData&")->GetThunk();
	}

	void ScriptCCollider::OnCollisionBegin(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::InvokeThunk(onCollisionBeginThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCCollider::OnCollisionStay(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::InvokeThunk(onCollisionStayThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCCollider::OnCollisionEnd(const CollisionData& p0)
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
		scriptmaterial = ScriptRRefBase::toNative(material);
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
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
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
