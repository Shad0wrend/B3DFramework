//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPhysicsScene.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysics.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "BsScriptGameObjectManager.h"
#include "BsScriptPhysicsQueryHit.generated.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMesh.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptQuaternion.h"
#include "BsScriptCCollider.generated.h"

namespace bs
{
	ScriptPhysicsScene::ScriptPhysicsScene(MonoObject* managedInstance, const SPtr<PhysicsScene>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptPhysicsScene::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_rayCast", (void*)&ScriptPhysicsScene::InternalRayCast);
		metaData.scriptClass->AddInternalCall("Internal_rayCast0", (void*)&ScriptPhysicsScene::InternalRayCast0);
		metaData.scriptClass->AddInternalCall("Internal_boxCast", (void*)&ScriptPhysicsScene::InternalBoxCast);
		metaData.scriptClass->AddInternalCall("Internal_sphereCast", (void*)&ScriptPhysicsScene::InternalSphereCast);
		metaData.scriptClass->AddInternalCall("Internal_capsuleCast", (void*)&ScriptPhysicsScene::InternalCapsuleCast);
		metaData.scriptClass->AddInternalCall("Internal_convexCast", (void*)&ScriptPhysicsScene::InternalConvexCast);
		metaData.scriptClass->AddInternalCall("Internal_rayCastAll", (void*)&ScriptPhysicsScene::InternalRayCastAll);
		metaData.scriptClass->AddInternalCall("Internal_rayCastAll0", (void*)&ScriptPhysicsScene::InternalRayCastAll0);
		metaData.scriptClass->AddInternalCall("Internal_boxCastAll", (void*)&ScriptPhysicsScene::InternalBoxCastAll);
		metaData.scriptClass->addInternalCall("Internal_sphereCastAll", (void*)&ScriptPhysicsScene::InternalSphereCastAll);
		metaData.scriptClass->addInternalCall("Internal_capsuleCastAll", (void*)&ScriptPhysicsScene::InternalCapsuleCastAll);
		metaData.scriptClass->addInternalCall("Internal_convexCastAll", (void*)&ScriptPhysicsScene::InternalConvexCastAll);
		metaData.scriptClass->addInternalCall("Internal_rayCastAny", (void*)&ScriptPhysicsScene::InternalRayCastAny);
		metaData.scriptClass->addInternalCall("Internal_rayCastAny0", (void*)&ScriptPhysicsScene::InternalRayCastAny0);
		metaData.scriptClass->addInternalCall("Internal_boxCastAny", (void*)&ScriptPhysicsScene::InternalBoxCastAny);
		metaData.scriptClass->addInternalCall("Internal_sphereCastAny", (void*)&ScriptPhysicsScene::InternalSphereCastAny);
		metaData.scriptClass->addInternalCall("Internal_capsuleCastAny", (void*)&ScriptPhysicsScene::InternalCapsuleCastAny);
		metaData.scriptClass->addInternalCall("Internal_convexCastAny", (void*)&ScriptPhysicsScene::InternalConvexCastAny);
		metaData.scriptClass->addInternalCall("Internal_boxOverlap", (void*)&ScriptPhysicsScene::InternalBoxOverlap);
		metaData.scriptClass->addInternalCall("Internal_sphereOverlap", (void*)&ScriptPhysicsScene::InternalSphereOverlap);
		metaData.scriptClass->addInternalCall("Internal_capsuleOverlap", (void*)&ScriptPhysicsScene::InternalCapsuleOverlap);
		metaData.scriptClass->addInternalCall("Internal_convexOverlap", (void*)&ScriptPhysicsScene::InternalConvexOverlap);
		metaData.scriptClass->addInternalCall("Internal_boxOverlapAny", (void*)&ScriptPhysicsScene::InternalBoxOverlapAny);
		metaData.scriptClass->addInternalCall("Internal_sphereOverlapAny", (void*)&ScriptPhysicsScene::InternalSphereOverlapAny);
		metaData.scriptClass->addInternalCall("Internal_capsuleOverlapAny", (void*)&ScriptPhysicsScene::InternalCapsuleOverlapAny);
		metaData.scriptClass->addInternalCall("Internal_convexOverlapAny", (void*)&ScriptPhysicsScene::InternalConvexOverlapAny);
		metaData.scriptClass->addInternalCall("Internal_getGravity", (void*)&ScriptPhysicsScene::InternalGetGravity);
		metaData.scriptClass->addInternalCall("Internal_setGravity", (void*)&ScriptPhysicsScene::InternalSetGravity);
		metaData.scriptClass->addInternalCall("Internal_addBroadPhaseRegion", (void*)&ScriptPhysicsScene::InternalAddBroadPhaseRegion);
		metaData.scriptClass->addInternalCall("Internal_removeBroadPhaseRegion", (void*)&ScriptPhysicsScene::InternalRemoveBroadPhaseRegion);
		metaData.scriptClass->addInternalCall("Internal_clearBroadPhaseRegions", (void*)&ScriptPhysicsScene::InternalClearBroadPhaseRegions);

	}

	MonoObject* ScriptPhysicsScene::Create(const SPtr<PhysicsScene>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptPhysicsScene>()) ScriptPhysicsScene(managedInstance, value);
		return managedInstance;
	}
	bool ScriptPhysicsScene::InternalRayCast(ScriptPhysicsScene* thisPtr, Ray* ray, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->rayCast(*ray, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	bool ScriptPhysicsScene::InternalRayCast0(ScriptPhysicsScene* thisPtr, Vector3* origin, Vector3* unitDir, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->rayCast(*origin, *unitDir, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	bool ScriptPhysicsScene::InternalBoxCast(ScriptPhysicsScene* thisPtr, AABox* box, Quaternion* rotation, Vector3* unitDir, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->boxCast(*box, *rotation, *unitDir, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	bool ScriptPhysicsScene::InternalSphereCast(ScriptPhysicsScene* thisPtr, Sphere* sphere, Vector3* unitDir, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->sphereCast(*sphere, *unitDir, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	bool ScriptPhysicsScene::InternalCapsuleCast(ScriptPhysicsScene* thisPtr, Capsule* capsule, Quaternion* rotation, Vector3* unitDir, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->capsuleCast(*capsule, *rotation, *unitDir, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	bool ScriptPhysicsScene::InternalConvexCast(ScriptPhysicsScene* thisPtr, MonoObject* mesh, Vector3* position, Quaternion* rotation, Vector3* unitDir, __PhysicsQueryHitInterop* hit, uint64_t layer, float max)
	{
		bool tmp__output;
		ResourceHandle<PhysicsMesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<PhysicsMesh>(scriptmesh->GetHandle());
		PhysicsQueryHit tmphit;
		tmp__output = thisPtr->GetInternal()->convexCast(tmpmesh, *position, *rotation, *unitDir, tmphit, layer, max);

		bool __output;
		__output = tmp__output;
		__PhysicsQueryHitInterop interophit;
		interophit = ScriptPhysicsQueryHit::ToInterop(tmphit);
		MonoUtil::valueCopy(hit, &interophit, ScriptPhysicsQueryHit::getMetaData()->scriptClass->GetInternalClassInternal());

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalRayCastAll(ScriptPhysicsScene* thisPtr, Ray* ray, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		vec__output = thisPtr->GetInternal()->rayCastAll(*ray, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalRayCastAll0(ScriptPhysicsScene* thisPtr, Vector3* origin, Vector3* unitDir, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		vec__output = thisPtr->GetInternal()->rayCastAll(*origin, *unitDir, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalBoxCastAll(ScriptPhysicsScene* thisPtr, AABox* box, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		vec__output = thisPtr->GetInternal()->boxCastAll(*box, *rotation, *unitDir, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalSphereCastAll(ScriptPhysicsScene* thisPtr, Sphere* sphere, Vector3* unitDir, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		vec__output = thisPtr->GetInternal()->sphereCastAll(*sphere, *unitDir, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalCapsuleCastAll(ScriptPhysicsScene* thisPtr, Capsule* capsule, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		vec__output = thisPtr->GetInternal()->capsuleCastAll(*capsule, *rotation, *unitDir, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalConvexCastAll(ScriptPhysicsScene* thisPtr, MonoObject* mesh, Vector3* position, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		Vector<PhysicsQueryHit> vec__output;
		ResourceHandle<PhysicsMesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<PhysicsMesh>(scriptmesh->GetHandle());
		vec__output = thisPtr->GetInternal()->convexCastAll(tmpmesh, *position, *rotation, *unitDir, layer, max);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPhysicsQueryHit>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptPhysicsQueryHit::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	bool ScriptPhysicsScene::InternalRayCastAny(ScriptPhysicsScene* thisPtr, Ray* ray, uint64_t layer, float max)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->rayCastAny(*ray, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalRayCastAny0(ScriptPhysicsScene* thisPtr, Vector3* origin, Vector3* unitDir, uint64_t layer, float max)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->rayCastAny(*origin, *unitDir, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalBoxCastAny(ScriptPhysicsScene* thisPtr, AABox* box, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->boxCastAny(*box, *rotation, *unitDir, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalSphereCastAny(ScriptPhysicsScene* thisPtr, Sphere* sphere, Vector3* unitDir, uint64_t layer, float max)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->sphereCastAny(*sphere, *unitDir, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalCapsuleCastAny(ScriptPhysicsScene* thisPtr, Capsule* capsule, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->capsuleCastAny(*capsule, *rotation, *unitDir, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalConvexCastAny(ScriptPhysicsScene* thisPtr, MonoObject* mesh, Vector3* position, Quaternion* rotation, Vector3* unitDir, uint64_t layer, float max)
	{
		bool tmp__output;
		ResourceHandle<PhysicsMesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<PhysicsMesh>(scriptmesh->GetHandle());
		tmp__output = thisPtr->GetInternal()->convexCastAny(tmpmesh, *position, *rotation, *unitDir, layer, max);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalBoxOverlap(ScriptPhysicsScene* thisPtr, AABox* box, Quaternion* rotation, uint64_t layer)
	{
		Vector<GameObjectHandle<CCollider>> vec__output;
		vec__output = thisPtr->GetInternal()->boxOverlap(*box, *rotation, layer);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCCollider>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptComponentBase* script__output = nullptr;
			if(vec__output[i])
				script__output = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(vec__output[i]));
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalSphereOverlap(ScriptPhysicsScene* thisPtr, Sphere* sphere, uint64_t layer)
	{
		Vector<GameObjectHandle<CCollider>> vec__output;
		vec__output = thisPtr->GetInternal()->sphereOverlap(*sphere, layer);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCCollider>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptComponentBase* script__output = nullptr;
			if(vec__output[i])
				script__output = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(vec__output[i]));
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalCapsuleOverlap(ScriptPhysicsScene* thisPtr, Capsule* capsule, Quaternion* rotation, uint64_t layer)
	{
		Vector<GameObjectHandle<CCollider>> vec__output;
		vec__output = thisPtr->GetInternal()->capsuleOverlap(*capsule, *rotation, layer);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCCollider>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptComponentBase* script__output = nullptr;
			if(vec__output[i])
				script__output = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(vec__output[i]));
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptPhysicsScene::InternalConvexOverlap(ScriptPhysicsScene* thisPtr, MonoObject* mesh, Vector3* position, Quaternion* rotation, uint64_t layer)
	{
		Vector<GameObjectHandle<CCollider>> vec__output;
		ResourceHandle<PhysicsMesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<PhysicsMesh>(scriptmesh->GetHandle());
		vec__output = thisPtr->GetInternal()->convexOverlap(tmpmesh, *position, *rotation, layer);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCCollider>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptComponentBase* script__output = nullptr;
			if(vec__output[i])
				script__output = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(vec__output[i]));
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	bool ScriptPhysicsScene::InternalBoxOverlapAny(ScriptPhysicsScene* thisPtr, AABox* box, Quaternion* rotation, uint64_t layer)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->boxOverlapAny(*box, *rotation, layer);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalSphereOverlapAny(ScriptPhysicsScene* thisPtr, Sphere* sphere, uint64_t layer)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->sphereOverlapAny(*sphere, layer);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalCapsuleOverlapAny(ScriptPhysicsScene* thisPtr, Capsule* capsule, Quaternion* rotation, uint64_t layer)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->capsuleOverlapAny(*capsule, *rotation, layer);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptPhysicsScene::InternalConvexOverlapAny(ScriptPhysicsScene* thisPtr, MonoObject* mesh, Vector3* position, Quaternion* rotation, uint64_t layer)
	{
		bool tmp__output;
		ResourceHandle<PhysicsMesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<PhysicsMesh>(scriptmesh->GetHandle());
		tmp__output = thisPtr->GetInternal()->convexOverlapAny(tmpmesh, *position, *rotation, layer);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsScene::InternalGetGravity(ScriptPhysicsScene* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetGravity();

		*__output = tmp__output;
	}

	void ScriptPhysicsScene::InternalSetGravity(ScriptPhysicsScene* thisPtr, Vector3* gravity)
	{
		thisPtr->GetInternal()->SetGravity(*gravity);
	}

	uint32_t ScriptPhysicsScene::InternalAddBroadPhaseRegion(ScriptPhysicsScene* thisPtr, AABox* region)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->addBroadPhaseRegion(*region);

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsScene::InternalRemoveBroadPhaseRegion(ScriptPhysicsScene* thisPtr, uint32_t handle)
	{
		thisPtr->GetInternal()->removeBroadPhaseRegion(handle);
	}

	void ScriptPhysicsScene::InternalClearBroadPhaseRegions(ScriptPhysicsScene* thisPtr)
	{
		thisPtr->GetInternal()->clearBroadPhaseRegions();
	}
}
