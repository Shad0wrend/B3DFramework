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
		metaData.scriptClass->addInternalCall("Internal_setIsTrigger", (void*)&ScriptCCollider::Internal_setIsTrigger);
		metaData.scriptClass->addInternalCall("Internal_getIsTrigger", (void*)&ScriptCCollider::Internal_getIsTrigger);
		metaData.scriptClass->addInternalCall("Internal_setMass", (void*)&ScriptCCollider::Internal_setMass);
		metaData.scriptClass->addInternalCall("Internal_getMass", (void*)&ScriptCCollider::Internal_getMass);
		metaData.scriptClass->addInternalCall("Internal_setMaterial", (void*)&ScriptCCollider::Internal_setMaterial);
		metaData.scriptClass->addInternalCall("Internal_getMaterial", (void*)&ScriptCCollider::Internal_getMaterial);
		metaData.scriptClass->addInternalCall("Internal_setContactOffset", (void*)&ScriptCCollider::Internal_setContactOffset);
		metaData.scriptClass->addInternalCall("Internal_getContactOffset", (void*)&ScriptCCollider::Internal_getContactOffset);
		metaData.scriptClass->addInternalCall("Internal_setRestOffset", (void*)&ScriptCCollider::Internal_setRestOffset);
		metaData.scriptClass->addInternalCall("Internal_getRestOffset", (void*)&ScriptCCollider::Internal_getRestOffset);
		metaData.scriptClass->addInternalCall("Internal_setLayer", (void*)&ScriptCCollider::Internal_setLayer);
		metaData.scriptClass->addInternalCall("Internal_getLayer", (void*)&ScriptCCollider::Internal_getLayer);
		metaData.scriptClass->addInternalCall("Internal_setCollisionReportMode", (void*)&ScriptCCollider::Internal_setCollisionReportMode);
		metaData.scriptClass->addInternalCall("Internal_getCollisionReportMode", (void*)&ScriptCCollider::Internal_getCollisionReportMode);

		onCollisionBeginThunk = (onCollisionBeginThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionBegin", "CollisionData&")->getThunk();
		onCollisionStayThunk = (onCollisionStayThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionStay", "CollisionData&")->getThunk();
		onCollisionEndThunk = (onCollisionEndThunkDef)metaData.scriptClass->getMethodExact("Internal_onCollisionEnd", "CollisionData&")->getThunk();
	}

	void ScriptCCollider::OnCollisionBegin(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::invokeThunk(onCollisionBeginThunk, getManagedInstance(), tmpp0);
	}

	void ScriptCCollider::OnCollisionStay(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::invokeThunk(onCollisionStayThunk, getManagedInstance(), tmpp0);
	}

	void ScriptCCollider::OnCollisionEnd(const CollisionData& p0)
	{
		MonoObject* tmpp0;
		__CollisionDataInterop interopp0;
		interopp0 = ScriptCollisionData::ToInterop(p0);
		tmpp0 = ScriptCollisionData::Box(interopp0);
		MonoUtil::invokeThunk(onCollisionEndThunk, getManagedInstance(), tmpp0);
	}
	void ScriptCCollider::InternalSetIsTrigger(ScriptCColliderBase* thisPtr, bool value)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setIsTrigger(value);
	}

	bool ScriptCCollider::InternalGetIsTrigger(ScriptCColliderBase* thisPtr)
	{
		bool tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getIsTrigger();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetMass(ScriptCColliderBase* thisPtr, float mass)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setMass(mass);
	}

	float ScriptCCollider::InternalGetMass(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getMass();

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
			tmpmaterial = static_resource_cast<PhysicsMaterial>(scriptmaterial->getHandle());
		static_object_cast<CCollider>(thisPtr->getComponent())->setMaterial(tmpmaterial);
	}

	MonoObject* ScriptCCollider::InternalGetMaterial(ScriptCColliderBase* thisPtr)
	{
		ResourceHandle<PhysicsMaterial> tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getMaterial();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCCollider::InternalSetContactOffset(ScriptCColliderBase* thisPtr, float value)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setContactOffset(value);
	}

	float ScriptCCollider::InternalGetContactOffset(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getContactOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetRestOffset(ScriptCColliderBase* thisPtr, float value)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setRestOffset(value);
	}

	float ScriptCCollider::InternalGetRestOffset(ScriptCColliderBase* thisPtr)
	{
		float tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getRestOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetLayer(ScriptCColliderBase* thisPtr, uint64_t layer)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setLayer(layer);
	}

	uint64_t ScriptCCollider::InternalGetLayer(ScriptCColliderBase* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getLayer();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCollider::InternalSetCollisionReportMode(ScriptCColliderBase* thisPtr, CollisionReportMode mode)
	{
		static_object_cast<CCollider>(thisPtr->getComponent())->setCollisionReportMode(mode);
	}

	CollisionReportMode ScriptCCollider::InternalGetCollisionReportMode(ScriptCColliderBase* thisPtr)
	{
		CollisionReportMode tmp__output;
		tmp__output = static_object_cast<CCollider>(thisPtr->getComponent())->getCollisionReportMode();

		CollisionReportMode __output;
		__output = tmp__output;

		return __output;
	}
}
