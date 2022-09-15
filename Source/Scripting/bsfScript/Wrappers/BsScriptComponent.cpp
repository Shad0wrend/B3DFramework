//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptComponent.h"
#include "BsScriptGameObjectManager.h"
#include "BsScriptObjectManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoManager.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsManagedComponent.h"
#include "Scene/BsSceneObject.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptComponentBase::ScriptComponentBase(MonoObject* instance)
		:ScriptGameObjectBase(instance)
	{ }

	void ScriptComponentBase::Destroy(bool assemblyRefresh)
	{
		// It's possible that managed component is destroyed but a reference to it is still kept during assembly refresh.
		// Such components shouldn't be restored so we delete them.

		HComponent component = GetComponent();
		if (!assemblyRefresh || component.IsDestroyed(true))
			ScriptGameObjectManager::Instance().DestroyScriptComponent(this);
	}

	bool ScriptComponentBase::CheckIfDestroyed(const GameObjectHandleBase& handle)
	{
		if (handle.IsDestroyed())
		{
			BS_LOG(Warning, Scene, "Trying to access a destroyed GameObject with instance ID: {0}", handle.getInstanceId());
			return true;
		}

		return false;
	}

	ScriptComponent::ScriptComponent(MonoObject* instance)
		:ScriptObject(instance)
	{
		assert(instance != nullptr);
	}

	void ScriptComponent::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_AddComponent", (void*)&ScriptComponent::internal_AddComponent);
		metaData.scriptClass->addInternalCall("Internal_GetComponent", (void*)&ScriptComponent::internal_GetComponent);
		metaData.scriptClass->addInternalCall("Internal_GetComponents", (void*)&ScriptComponent::internal_GetComponents);
		metaData.scriptClass->addInternalCall("Internal_GetComponentsPerType", (void*)&ScriptComponent::internal_GetComponentsPerType);
		metaData.scriptClass->addInternalCall("Internal_RemoveComponent", (void*)&ScriptComponent::internal_removeComponent);
		metaData.scriptClass->addInternalCall("Internal_GetSceneObject", (void*)&ScriptComponent::internal_getSceneObject);
		metaData.scriptClass->addInternalCall("Internal_GetNotifyFlags", (void*)&ScriptComponent::internal_getNotifyFlags);
		metaData.scriptClass->addInternalCall("Internal_SetNotifyFlags", (void*)&ScriptComponent::internal_setNotifyFlags);
		metaData.scriptClass->addInternalCall("Internal_Destroy", (void*)&ScriptComponent::internal_destroy);
	}

	MonoObject* ScriptComponent::InternalAddComponent(MonoObject* parentSceneObject, MonoReflectionType* type)
	{
		ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(parentSceneObject);
		HSceneObject so = static_object_cast<SceneObject>(scriptSO->GetNativeHandle());

		if (checkIfDestroyed(so))
			return nullptr;

		ScriptAssemblyManager& sam = ScriptAssemblyManager::Instance();

		MonoClass* managedComponent = sam.getBuiltinClasses().managedComponentClass;
		::MonoClass* requestedClass = MonoUtil::getClass(type);

		bool isManagedComponent = MonoUtil::isSubClassOf(requestedClass, managedComponent->GetInternalClassInternal());
		if(isManagedComponent)
		{
			GameObjectHandle<ManagedComponent> mc = so->AddComponent<ManagedComponent>(type);
			return mc->GetManagedInstance();
		}
		else
		{
			BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(type);
			if (info == nullptr)
				return nullptr;

			HComponent component = so->AddComponent(info->typeId);
			ScriptComponentBase* scriptComponent =
				ScriptGameObjectManager::Instance().createBuiltinScriptComponent(component);

			return scriptComponent->GetManagedInstance();
		}
	}

	MonoObject* ScriptComponent::InternalGetComponent(MonoObject* parentSceneObject, MonoReflectionType* type)
	{
		ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(parentSceneObject);
		HSceneObject so = static_object_cast<SceneObject>(scriptSO->GetNativeHandle());

		if (checkIfDestroyed(so))
			return nullptr;

		ScriptAssemblyManager& sam = ScriptAssemblyManager::Instance();
		BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(type);

		::MonoClass* baseClass = MonoUtil::getClass(type);

		const Vector<HComponent>& mComponents = so->GetComponents();
		for(auto& component : mComponents)
		{
			if(component->GetTypeId() == TID_ManagedComponent)
			{
				GameObjectHandle<ManagedComponent> managedComponent = static_object_cast<ManagedComponent>(component);

				MonoReflectionType* componentReflType = managedComponent->GetRuntimeType();
				::MonoClass* componentClass = MonoUtil::getClass(componentReflType);
				
				if(MonoUtil::isSubClassOf(componentClass, baseClass))
				{
					return managedComponent->GetManagedInstance();
				}
			}
			else
			{
				if(info == nullptr)
					continue;

				if(info->typeId == component->GetTypeId())
				{
					ScriptComponentBase* scriptComponent = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(component);
					return scriptComponent->GetManagedInstance();
				}
			}
		}

		return nullptr;
	}

	MonoArray* ScriptComponent::InternalGetComponentsPerType(MonoObject* parentSceneObject, MonoReflectionType* type)
	{
		ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(parentSceneObject);
		HSceneObject so = static_object_cast<SceneObject>(scriptSO->GetNativeHandle());

		ScriptAssemblyManager& sam = ScriptAssemblyManager::Instance();
		BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(type);

		::MonoClass* baseClass = MonoUtil::getClass(type);
		Vector<MonoObject*> managedComponents;

		if (!checkIfDestroyed(so))
		{
			const Vector<HComponent>& mComponents = so->GetComponents();
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == TID_ManagedComponent)
				{
					GameObjectHandle<ManagedComponent> managedComponent = static_object_cast<ManagedComponent>(component);

					MonoReflectionType* componentReflType = managedComponent->GetRuntimeType();
					::MonoClass* componentClass = MonoUtil::getClass(componentReflType);

					if (MonoUtil::isSubClassOf(componentClass, baseClass))
						managedComponents.push_back(managedComponent->GetManagedInstance());
				}
				else
				{
					if(info == nullptr)
						continue;

					if(info->typeId == component->GetTypeId())
					{
						ScriptComponentBase* scriptComponent = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(component);
						managedComponents.push_back(scriptComponent->GetManagedInstance());
					}
				}
			}
		}

		ScriptArray scriptArray(metaData.scriptClass->GetInternalClassInternal(), (UINT32)managedComponents.size());
		for (UINT32 i = 0; i < (UINT32)managedComponents.size(); i++)
			scriptArray.Set(i, managedComponents[i]);

		return scriptArray.getInternal();
	}

	MonoArray* ScriptComponent::InternalGetComponents(MonoObject* parentSceneObject)
	{
		ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(parentSceneObject);
		HSceneObject so = static_object_cast<SceneObject>(scriptSO->GetNativeHandle());

		Vector<MonoObject*> managedComponents;

		if (!checkIfDestroyed(so))
		{
			const Vector<HComponent>& mComponents = so->GetComponents();
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == TID_ManagedComponent)
				{
					GameObjectHandle<ManagedComponent> managedComponent = static_object_cast<ManagedComponent>(component);

					managedComponents.push_back(managedComponent->GetManagedInstance());
				}
				else
				{
					ScriptComponentBase* scriptComponent = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(component);
					if(scriptComponent != nullptr)
						managedComponents.push_back(scriptComponent->GetManagedInstance());
				}
			}
		}

		ScriptArray scriptArray(metaData.scriptClass->GetInternalClassInternal(), (UINT32)managedComponents.size());
		for(UINT32 i = 0; i < (UINT32)managedComponents.size(); i++)
			scriptArray.Set(i, managedComponents[i]);

		return scriptArray.getInternal();
	}

	void ScriptComponent::InternalRemoveComponent(MonoObject* parentSceneObject, MonoReflectionType* type)
	{
		ScriptSceneObject* scriptSO = ScriptSceneObject::toNative(parentSceneObject);
		HSceneObject so = static_object_cast<SceneObject>(scriptSO->GetNativeHandle());

		if (checkIfDestroyed(so))
			return;

		ScriptAssemblyManager& sam = ScriptAssemblyManager::Instance();
		BuiltinComponentInfo* info = sam.getBuiltinComponentInfo(type);

		::MonoClass* baseClass = MonoUtil::getClass(type);

		const Vector<HComponent>& mComponents = so->GetComponents();
		for(auto& component : mComponents)
		{
			if (component->GetTypeId() == TID_ManagedComponent)
			{
				GameObjectHandle<ManagedComponent> managedComponent = static_object_cast<ManagedComponent>(component);

				MonoReflectionType* componentReflType = managedComponent->GetRuntimeType();
				::MonoClass* componentClass = MonoUtil::getClass(componentReflType);

				if (MonoUtil::isSubClassOf(componentClass, baseClass))
				{
					managedComponent->Destroy();
					return;
				}
			}
			else
			{
				if(info == nullptr)
					continue;

				if(info->typeId == component->GetTypeId())
				{
					component->Destroy();
					return;
				}
			}
		}

		BS_LOG(Warning, Scene, "Attempting to remove a component that doesn't exists on SceneObject \"{0}\"", so->GetName());
	}

	MonoObject* ScriptComponent::InternalGetSceneObject(ScriptComponentBase* nativeInstance)
	{
		HComponent component = nativeInstance->GetComponent();
		if (checkIfDestroyed(component))
			return nullptr;

		HSceneObject sceneObject = component->SceneObject();

		ScriptSceneObject* scriptSO = ScriptGameObjectManager::Instance().getOrCreateScriptSceneObject(sceneObject);

		assert(scriptSO->GetManagedInstance() != nullptr);
		return scriptSO->GetManagedInstance();
	}

	TransformChangedFlags ScriptComponent::InternalGetNotifyFlags(ScriptComponentBase* nativeInstance)
	{
		HComponent component = nativeInstance->GetComponent();

		if (!checkIfDestroyed(component))
			return component->GetNotifyFlagsInternal();

		return TCF_None;
	}

	void ScriptComponent::InternalSetNotifyFlags(ScriptComponentBase* nativeInstance, TransformChangedFlags flags)
	{
		HComponent component = nativeInstance->GetComponent();

		if (!checkIfDestroyed(component))
			component->SetNotifyFlags(flags);
	}
	
	void ScriptComponent::InternalDestroy(ScriptComponentBase* nativeInstance, bool immediate)
	{
		HComponent component = nativeInstance->GetComponent();

		if (!checkIfDestroyed(component))
			component->Destroy(immediate);
	}
}
