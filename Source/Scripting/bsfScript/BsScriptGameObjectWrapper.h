//********************************* bs::framework - Copyright 2024 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObjectWrapper.h"
#include "Script/BsIScriptExportable.h"

namespace bs
{
	/** @addtogroup Script
	 *  @{
	 */

	/** Provides a base class for all script object wrappers that wrap a GameObject object that may be passed as a shared pointer. */
	class ScriptGameObjectWrapper : public ScriptObjectWrapper
	{
	public:
		using ScriptObjectWrapper::ScriptObjectWrapper;

		/** Returns the root base class of the wrapped native object as a shared pointer. */
		virtual SPtr<GameObject> GetBaseNativeObjectAsShared() const = 0;

		/** Returns the root base class of the wrapped native object as a handle. */
		virtual HGameObject GetBaseNativeObjectAsHandle() const = 0;

		/**
		 * Attempts to retrieve an existing associated script object from the provided native object. If one doesn't exist, a new script
		 * object and the associated script wrapper will be created.
		 *
		 * Unlike GetOrCreateScriptObject implemented on TScriptGameObjectWrapper, this always accepts the object as a GameObject, and
		 * needs to perform type lookup to get the exact script wrapper type.
		 */
		static MonoObject* GetOrCreateScriptObject(const HGameObject& nativeObject)
		{
			if(!nativeObject.IsValid())
				return nullptr;

			const u32 rttiId = nativeObject->GetTypeId();
			const ScriptWrapperObjectMetaData* const scriptWrapperObjectMetaData = ScriptAssemblyManager::Instance().GetScriptWrapperMetaData(rttiId);
			if(scriptWrapperObjectMetaData == nullptr)
			{
				B3D_LOG(Error, Script, "Cannot retrieve script object. Mapping between a game object and a managed type is missing for type \"{0}\"", rttiId);
				return nullptr;
			}

			if(scriptWrapperObjectMetaData->CreateCallbackType != ScriptWrapperCreateCallbackType::GameObject)
			{
				B3D_LOG(Error, Script, "Cannot retrieve script object. Script wrapper for type \"{0}\" does not support creation of a GameObject handle.", rttiId);
				return nullptr;
			}

			if(!B3D_ENSURE(scriptWrapperObjectMetaData->GetScriptExportable != nullptr))
				return nullptr;

			IScriptExportable* const scriptExportableObject = scriptWrapperObjectMetaData->GetScriptExportable(nativeObject.Get());
			if(ScriptObjectWrapper* const scriptObjectWrapper = (ScriptObjectWrapper*)scriptExportableObject->GetScriptObjectWrapper())
				return scriptObjectWrapper->GetScriptObject();

			return scriptWrapperObjectMetaData->GameObjectCreateCallback(nativeObject);
		}

		/** Returns the script object wrapper associated with the provided script object, and wrapped by a wrapper that owns the provided meta-data. */
		static ScriptGameObjectWrapper* GetScriptObjectWrapper(const ScriptWrapperObjectMetaData& wrapperMetaData, MonoObject* scriptObject)
		{
			ScriptGameObjectWrapper* scriptObjectWrapper = nullptr;

			if(wrapperMetaData.ScriptObjectWrapperPointerField != nullptr && scriptObject != nullptr)
				wrapperMetaData.ScriptObjectWrapperPointerField->Get(scriptObject, &scriptObjectWrapper);

			return scriptObjectWrapper;
		}
	};

	/** Extends TScriptObjectWrapper by providing functionality required for types that may be passed along as a GameObject handle. */
	template<typename NativeType, typename SelfType, typename BaseType = ScriptGameObjectWrapper>
	class TScriptGameObjectWrapper : public TScriptObjectWrapper<SelfType, BaseType>
	{
	public:
		TScriptGameObjectWrapper(const GameObjectHandle<NativeType>& nativeObject, MonoObject* scriptObject)
			: TScriptObjectWrapper<SelfType, BaseType>(nativeObject.Get(), scriptObject), mNativeObjectStrongHandle(nativeObject)
		{ }

		/** Returns the wrapped native object as a shared pointer. */
		const SPtr<NativeType>& GetNativeObjectAsShared() const { return mNativeObjectStrongHandle.GetShared(); }

		/** Returns the wrapped native object as a handle. */
		const GameObjectHandle<NativeType>& GetNativeObjectAsHandle() const { return mNativeObjectStrongHandle; }

		SPtr<GameObject> GetBaseNativeObjectAsShared() const override { return GetNativeObjectAsShared(); }
		HGameObject GetBaseNativeObjectAsHandle() const override { return GetNativeObjectAsHandle(); }

		/**
		 * Creates a new script object and a script object wrapper of @p SelfType, and associates them with the provided native object. Should not be called if @p nativeObject
		 * already has an associated script object.
		 */
		static MonoObject* CreateScriptObjectAndWrapper(const HGameObject& nativeObject)
		{
			MonoObject* const scriptObject = SelfType::CreateScriptObject(false);
			B3DNew<SelfType>(B3DStaticGameObjectCast<NativeType>(nativeObject), scriptObject);

			return scriptObject;
		}

		/** Casts the reflectable object to script exportable. */
		static IScriptExportable* GetScriptExportable(IReflectable* nativeObject)
		{
			return (IScriptExportable*)(NativeType*)nativeObject;
		}

		/**
		 * Attempts to retrieve an existing associated script object from the provided native object. If one doesn't exist, a new script
		 * object and the associated script wrapper will be created.
		 */
		static MonoObject* GetOrCreateScriptObject(const GameObjectHandle<NativeType>& nativeObject)
		{
			if(!nativeObject.IsValid())
				return nullptr;

			if(ScriptObjectWrapper* const scriptObjectWrapper = (ScriptObjectWrapper*)nativeObject->GetScriptObjectWrapper())
				return scriptObjectWrapper->GetScriptObject();

			// TODO: Could skip expensive lookup if the type has no derived classes (should be most cases). In that case the code-gen could generate
			// code that calls a streamlined version of this method, with no lookup.
			const ScriptWrapperObjectMetaData* metaData = ScriptAssemblyManager::Instance().GetScriptWrapperMetaData(nativeObject->GetTypeId());
			if(!B3D_ENSURE(metaData))
				return metaData->GameObjectCreateCallback(nativeObject);

			return CreateScriptObjectAndWrapper(nativeObject);
		}

	protected:
		friend class TScriptObjectWrapper<SelfType, BaseType>;

		/** Initialize RTTI type ID and callback used to create the script object/script object wrapper. */
		static void InitializeAdditionalMetaData(ScriptWrapperObjectMetaData& metaData)
		{
			metaData.TypeId = NativeType::GetRttiStatic()->GetRttiId();
			metaData.GameObjectCreateCallback = &CreateScriptObjectAndWrapper;
			metaData.CreateCallbackType = ScriptWrapperCreateCallbackType::GameObject;
			metaData.GetScriptExportable = &GetScriptExportable;
		}

		GameObjectHandle<NativeType> mNativeObjectStrongHandle;
	};

	/** @} */
} // namespace bs
