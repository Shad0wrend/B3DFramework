//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleCollisions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptGameObjectManager.h"
#include "Wrappers/BsScriptPlane.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "BsScriptPARTICLE_COLLISIONS_DESC.generated.h"
#include "BsScriptParticleCollisions.generated.h"

namespace bs
{
	ScriptParticleCollisions::ScriptParticleCollisions(MonoObject* managedInstance, const SPtr<ParticleCollisions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptParticleCollisions::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setPlanes", (void*)&ScriptParticleCollisions::Internal_setPlanes);
		metaData.scriptClass->addInternalCall("Internal_getPlanes", (void*)&ScriptParticleCollisions::Internal_getPlanes);
		metaData.scriptClass->addInternalCall("Internal_setPlaneObjects", (void*)&ScriptParticleCollisions::Internal_setPlaneObjects);
		metaData.scriptClass->addInternalCall("Internal_getPlaneObjects", (void*)&ScriptParticleCollisions::Internal_getPlaneObjects);
		metaData.scriptClass->addInternalCall("Internal_setOptions", (void*)&ScriptParticleCollisions::Internal_setOptions);
		metaData.scriptClass->addInternalCall("Internal_getOptions", (void*)&ScriptParticleCollisions::Internal_getOptions);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptParticleCollisions::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptParticleCollisions::Internal_create0);

	}

	MonoObject* ScriptParticleCollisions::Create(const SPtr<ParticleCollisions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleCollisions>()) ScriptParticleCollisions(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleCollisions::InternalSetPlanes(ScriptParticleCollisions* thisPtr, MonoArray* planes)
	{
		Vector<Plane> vecplanes;
		if(planes != nullptr)
		{
			ScriptArray arrayplanes(planes);
			vecplanes.resize(arrayplanes.size());
			for(int i = 0; i < (int)arrayplanes.size(); i++)
			{
				vecplanes[i] = arrayplanes.get<Plane>(i);
			}
		}
		thisPtr->getInternal()->setPlanes(vecplanes);
	}

	MonoArray* ScriptParticleCollisions::InternalGetPlanes(ScriptParticleCollisions* thisPtr)
	{
		Vector<Plane> vec__output;
		vec__output = thisPtr->getInternal()->getPlanes();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptPlane>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptParticleCollisions::InternalSetPlaneObjects(ScriptParticleCollisions* thisPtr, MonoArray* objects)
	{
		Vector<GameObjectHandle<SceneObject>> vecobjects;
		if(objects != nullptr)
		{
			ScriptArray arrayobjects(objects);
			vecobjects.resize(arrayobjects.size());
			for(int i = 0; i < (int)arrayobjects.size(); i++)
			{
				ScriptSceneObject* scriptobjects;
				scriptobjects = ScriptSceneObject::toNative(arrayobjects.get<MonoObject*>(i));
				if(scriptobjects != nullptr)
				{
					GameObjectHandle<SceneObject> arrayElemPtrobjects = scriptobjects->getHandle();
					vecobjects[i] = arrayElemPtrobjects;
				}
			}
		}
		thisPtr->getInternal()->setPlaneObjects(vecobjects);
	}

	MonoArray* ScriptParticleCollisions::InternalGetPlaneObjects(ScriptParticleCollisions* thisPtr)
	{
		Vector<GameObjectHandle<SceneObject>> vec__output;
		vec__output = thisPtr->getInternal()->getPlaneObjects();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptSceneObject>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptSceneObject* script__output = nullptr;
			if(vec__output[i])
			script__output = ScriptGameObjectManager::Instance().getOrCreateScriptSceneObject(vec__output[i]);
			if(script__output != nullptr)
				array__output.set(i, script__output->getManagedInstance());
			else
				array__output.set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptParticleCollisions::InternalSetOptions(ScriptParticleCollisions* thisPtr, PARTICLE_COLLISIONS_DESC* options)
	{
		thisPtr->getInternal()->setOptions(*options);
	}

	void ScriptParticleCollisions::InternalGetOptions(ScriptParticleCollisions* thisPtr, PARTICLE_COLLISIONS_DESC* __output)
	{
		PARTICLE_COLLISIONS_DESC tmp__output;
		tmp__output = thisPtr->getInternal()->getOptions();

		*__output = tmp__output;
	}

	void ScriptParticleCollisions::InternalCreate(MonoObject* managedInstance, PARTICLE_COLLISIONS_DESC* desc)
	{
		SPtr<ParticleCollisions> instance = ParticleCollisions::Create(*desc);
		new (bs_alloc<ScriptParticleCollisions>())ScriptParticleCollisions(managedInstance, instance);
	}

	void ScriptParticleCollisions::InternalCreate0(MonoObject* managedInstance)
	{
		SPtr<ParticleCollisions> instance = ParticleCollisions::Create();
		new (bs_alloc<ScriptParticleCollisions>())ScriptParticleCollisions(managedInstance, instance);
	}
}
