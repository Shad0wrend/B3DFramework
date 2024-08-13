//********************************* bs::framework - Copyright 2024 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptMeta.h"
#include "BsMonoManager.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "Script/BsIScriptObjectWrapper.h"

namespace bs
{
	/** @addtogroup Script
	 *  @{
	 */

	/** Checks if the class @p T has a GetShared() method that accepts no parameters. */
	template <typename T, typename = void>
	struct B3DHasGetShared : std::false_type {};

	template <typename T>
	struct B3DHasGetShared<T, std::void_t<decltype(std::declval<T>().GetShared())>> : std::true_type {};

	/** Checks if the class @p T has a GetHandle() method that accepts no parameters. */
	template <typename T, typename = void>
	struct B3DHasGetHandle : std::false_type {};

	template <typename T>
	struct B3DHasGetHandle<T, std::void_t<decltype(std::declval<T>().GetHandle())>> : std::true_type {};

	// TODO - Doc
	struct ScriptObjectReloadPersistentData
	{
		ScriptObjectReloadPersistentData() {}

		explicit ScriptObjectReloadPersistentData(const Any& data)
			: Data(data)
		{}

		Any Data; // TODO - Don't use Any
	};

	/** Contains mapping between a native object type and its script export wrapper type. */
	template<class NativeObjectType>
	struct TScriptExportedTypeInformation
	{
		const ScriptMeta* ScriptMetaData = nullptr;
		u32 TypeId = ~0u; /*< RTTI ID of the native object. */
		MonoClass* ScriptClass = nullptr;
		std::function<MonoObject*(const NativeObjectType&)> CreateCallback;
	};

	// TODO - Doc
	struct ScriptExportedTypeMappings
	{
		// TODO - Doc
		static TArray<TScriptExportedTypeInformation<HResource>>& GetResourceTypeMappings()
		{
			static TArray<TScriptExportedTypeInformation<HResource>> sEntries;
			return sEntries;
		}
		
		// TODO - Doc
		static TArray<TScriptExportedTypeInformation<HGameObject>>& GetGameObjectTypeMappings()
		{
			static TArray<TScriptExportedTypeInformation<HGameObject>> sEntries;
			return sEntries;
		}

		// TODO - Doc
		static TArray<TScriptExportedTypeInformation<SPtr<IReflectable>>>& GetReflectableTypeMappings()
		{
			static TArray<TScriptExportedTypeInformation<SPtr<IReflectable>>> sEntries;
			return sEntries;
		}
	};

	// TODO - Doc
	class B3D_SCRIPT_INTEROP_EXPORT ScriptObjectWrapper : public IScriptObjectWrapper
	{
	public:
		ScriptObjectWrapper(IScriptExportable* nativeObject, MonoObject* scriptObject);
		virtual ~ScriptObjectWrapper();

		MonoObject* GetScriptObject() const;

		/** Called when the script system is notified that the script object has been destroyed. */
		virtual void NotifyScriptObjectDestroyed(bool isDestroyedDueToScriptReload);

		/**
		 * If this method returns true, then the we will keep a strong reference to the script object as long as the native object is alive. Note this is only safe for
		 * native objects that are explicitly destroyed, as the wrapper itself holds a strong reference to the native object and the object would never be freed if we only
		 * rely on reference counting. Note when explicitly destroying the native object, you need to manually call NotifyNativeObjectDestroyed() from the destroy method.
		 *
		 * // TODO - The above approach might not work for e.g. resources. I might need to add a boolean to IScriptExportable that lets the object know if its being referenced
		 * // from script or not. Then at certain intervals we can iterate over all script object wrappers and check objects that have their reference count at 1 with the
		 * // script reference flag set. In such situation we can release the script object strong handle.
		 * // - Actually this approach can be extended in the general case and we can keep strong handles in all cases, and don't need this method at all
		 * // - And I don't need a special flag, I can just check if the wrapper is assigned to IScriptExportable
		 * // - TODO - What when the object gets referenced from native code again? e.g. a weak resource handle gets converted into a strong one. Then the
		 * //   script object might get killed, but the native object is still alive. But in that case we might be okay to just re-create the script object?
		 */
		virtual bool ShouldKeepStrongReferenceToScriptObject() const { return false; }

		void NotifyNativeObjectDestroyed() override;

		/**
		 * @name Script reload
		 * @{
		 */

		/**
		 * If true, the object will be given an opportunity to back up its data before a script reload operation, and its script object will be automatically
		 * recreated once script reload ends, and given an opportunity to restore the backed up data. If false, the script object and its wrapper will
		 * be destroyed on script reload.
		 */
		virtual bool ShouldPersistScriptReload() const { return false; }

		/**
		 * Called on all script object wrappers when script reload is about to happen. Allows the script object to back up its current state
		 * so it may be restored after reload completes. Only relevant for script objects that persist script reload (i.e. ShouldPersistScriptReload() returns true).
		 */
		virtual Optional<ScriptObjectReloadPersistentData> BackupDataBeforeScriptReload() { return {}; }

		/**
		 * Called on all script object wrappers when script reload is about to happen, after BackupDataBeforeScriptReload() is called. Allows the wrapper to
		 * release any explicit strong handles it may be holding, so the object gets released correctly.
		 */
		virtual void ReleaseStrongHandlesBeforeScriptReload();

		/**
		 * Called after script reload completes. This needs to recreate the internal script object, as the old one will have been destroyed during the reload.
		 * Only relevant for script objects that persist script reload (i.e. ShouldPersistScriptReload() returns true).
		 */
		virtual void RecreateScriptObjectAfterScriptReload() { }

		/**
		 * Called as the final step after script reload completes. Allows you to restore data backed up in BackupDataBeforeScriptReload() call to the
		 * newly created script object. Only relevant for script objects that persist script reload (i.e. ShouldPersistScriptReload() returns true).
		 */
		virtual void RestoreDataAfterScriptReload(const ScriptObjectReloadPersistentData& data) { }

		/** @} */

	protected:
		u32 mStrongScriptObjectHandle = ~0u;
	};

	template <typename SelfType>
	class TScriptObjectWrapper;

	// TODO - Doc
	template <typename SelfType>
	struct InitializeScriptObjectWrapperOnLoadTime
	{
	public:
		InitializeScriptObjectWrapperOnLoadTime()
		{
			TScriptObjectWrapper<SelfType>::InitializeMetaDataAtLoadTime();
		}

		void MakeSureIAmInstantiated() {}
	};

	/**	Template version of ScriptObjectBase populates the object meta-data on library load. */
	template <typename SelfType>
	class TScriptObjectWrapper : public ScriptObjectWrapper 
	{
	public:
		TScriptObjectWrapper(IScriptExportable* nativeObject, MonoObject* scriptObject)
			: ScriptObjectWrapper(nativeObject, scriptObject)
		{
			sInitializeOnLoadTime.MakeSureIAmInstantiated();
			BindSelfToScriptObject(scriptObject);
		}

		virtual ~TScriptObjectWrapper() = default;

		// TODO - Doc
		void RecreateScriptObjectAfterScriptReload() override
		{
			MonoObject* const scriptObject = SelfType::CreateScriptObject(true);
			BindSelfToScriptObject(scriptObject);
		}

		/** Returns the script object wrapper associated with the provided script object. */
		static SelfType* GetScriptObjectWrapper(MonoObject* scriptObject)
		{
			SelfType* scriptObjectWrapper = nullptr;

			if(sInteropMetaData.ScriptObjectWrapperPointerField != nullptr && scriptObject != nullptr)
				sInteropMetaData.ScriptObjectWrapperPointerField->Get(scriptObject, &scriptObjectWrapper);

			return scriptObjectWrapper;
		}

		// TODO - Doc
		static const ScriptMeta* GetMetaData() { return &sInteropMetaData; }

		// TODO - Doc
		static void InitializeMetaDataAtLoadTime()
		{
			// Need to delay init of sInteropMetaData since it's also a static, and we can't guarantee the order
			// (if it gets initialized after this, it will just overwrite the data)
			ScriptMeta localMetaData = ScriptMeta(SelfType::GetAssemblyName(), SelfType::GetNamespace(), SelfType::GetTypeName(), &SelfType::SetupScriptBindings);

			MonoManager::RegisterScriptType(&sInteropMetaData, localMetaData);
			SelfType::RegisterNativeToScriptTypeMapping();
		}

	protected:
		// TODO - Doc
		void BindSelfToScriptObject(MonoObject* scriptObject)
		{
			SelfType* self = (SelfType*)(ScriptObjectWrapper*)this; // Needed due to multiple inheritance. Safe since SelfType must point to an class derived from this one.

			if(sInteropMetaData.ScriptObjectWrapperPointerField != nullptr)
				sInteropMetaData.ScriptObjectWrapperPointerField->Set(scriptObject, &self);

			if(sInteropMetaData.IsUsingNewScriptObjectManagerField != nullptr)
			{
				i32 value = 1;
				sInteropMetaData.IsUsingNewScriptObjectManagerField->Set(scriptObject, &value);
			}
		}

		static ScriptMeta sInteropMetaData;
		static InitializeScriptObjectWrapperOnLoadTime<SelfType> sInitializeOnLoadTime;
	};

	template <typename SelfType>
	InitializeScriptObjectWrapperOnLoadTime<SelfType> TScriptObjectWrapper<SelfType>::sInitializeOnLoadTime;

	template <typename SelfType>
	ScriptMeta TScriptObjectWrapper<SelfType>::sInteropMetaData;

	/**	Specialized version of TScriptObjectWrapper that should be used for types that are never going to be explicitly instantiated (e.g. singletons, static-only classes and base classes). */
	template <typename SelfType>
	class TNonInstantiableScriptObjectWrapper : public TScriptObjectWrapper<SelfType>
	{
	public:
		TNonInstantiableScriptObjectWrapper(MonoObject* scriptObject)
			:TScriptObjectWrapper<SelfType>(nullptr, scriptObject)
		{ }

		// TODO - Doc
		static MonoObject* CreateScriptObject(bool construct)
		{
			return nullptr;
		}

	protected:
		friend class TScriptObjectWrapper<SelfType>;

		// TODO - Doc
		static void RegisterNativeToScriptTypeMapping()
		{
			// Do nothing
		}
	};

	// TODO - Doc
	template<typename NativeType, typename SelfType>
	class TScriptReflectableWrapper : public TScriptObjectWrapper<SelfType> // TODO - Move to own file
	{
	public:
		TScriptReflectableWrapper(const SPtr<NativeType>& nativeObject, MonoObject* scriptObject)
			: TScriptObjectWrapper<SelfType>(nativeObject.get(), scriptObject), mNativeObjectStrongHandle(nativeObject)
		{ }

		/** Returns the wrapped native object as a shared pointer. */
		const SPtr<NativeType>& GetNativeObjectAsShared() const { return mNativeObjectStrongHandle; }

		// TODO - Doc
		static MonoObject* CreateScriptObjectAndWrapper(const SPtr<IReflectable>& nativeObject)
		{
			MonoObject* const scriptObject = SelfType::CreateScriptObject(false);
			B3DNew<SelfType>(B3DRTTICast<NativeType>(nativeObject), scriptObject);

			return scriptObject;
		}

		// TODO - Doc
		static MonoObject* GetOrCreateScriptObject(const SPtr<NativeType>& nativeObject)
		{
			if(ScriptObjectWrapper* const scriptObjectWrapper = (ScriptObjectWrapper*)nativeObject->GetScriptObjectWrapper())
				return scriptObjectWrapper->GetScriptObject();

			// TODO - This needs to perform RTTI ID lookup and call the creation callback on the returned info
			return CreateScriptObjectAndWrapper(nativeObject);
		}

	protected:
		friend class TScriptObjectWrapper<SelfType>;

		// TODO - Doc
		static void RegisterNativeToScriptTypeMapping()
		{
			TScriptExportedTypeInformation<SPtr<IReflectable>> typeMappingInformation;
			typeMappingInformation.TypeId = NativeType::GetRttiStatic()->GetRttiId();
			typeMappingInformation.ScriptClass = nullptr; // Populated later
			typeMappingInformation.ScriptMetaData = &SelfType::sInteropMetaData;
			typeMappingInformation.CreateCallback = &CreateScriptObjectAndWrapper;

			ScriptExportedTypeMappings::GetReflectableTypeMappings().Add(typeMappingInformation);
		}

		SPtr<NativeType> mNativeObjectStrongHandle;
	};

	// TODO - Doc
	template<typename NativeType, typename SelfType>
	class TScriptGameObjectWrapper : public TScriptObjectWrapper<SelfType> // TODO - Move to own file
	{
	public:
		TScriptGameObjectWrapper(const GameObjectHandle<NativeType>& nativeObject, MonoObject* scriptObject)
			: TScriptObjectWrapper<SelfType>(nativeObject.Get(), scriptObject), mNativeObjectStrongHandle(nativeObject)
		{ }

		/** Returns the wrapped native object as a shared pointer. */
		const SPtr<NativeType>& GetNativeObjectAsShared() const { return mNativeObjectStrongHandle.GetShared(); }

		/** Returns the wrapped native object as a handle. */
		const GameObjectHandle<NativeType>& GetNativeObjectAsHandle() const { return mNativeObjectStrongHandle; }

		// TODO - Doc
		static MonoObject* CreateScriptObjectAndWrapper(const HGameObject& nativeObject)
		{
			MonoObject* const scriptObject = SelfType::CreateScriptObject(false);
			B3DNew<SelfType>(B3DStaticGameObjectCast<NativeType>(nativeObject), scriptObject);

			return scriptObject;
		}

		// TODO - Doc
		static MonoObject* GetOrCreateScriptObject(const GameObjectHandle<NativeType>& nativeObject)
		{
			if(ScriptObjectWrapper* const scriptObjectWrapper = (ScriptObjectWrapper*)nativeObject->GetScriptObjectWrapper())
				return scriptObjectWrapper->GetScriptObject();

			// TODO - This needs to perform RTTI ID lookup and call the creation callback on the returned info
			return CreateScriptObjectAndWrapper(nativeObject);
		}

	protected:
		friend class TScriptObjectWrapper<SelfType>;

		// TODO - Doc
		static void RegisterNativeToScriptTypeMapping()
		{
			TScriptExportedTypeInformation<HGameObject> typeMappingInformation;
			typeMappingInformation.TypeId = NativeType::GetRttiStatic()->GetRttiId();
			typeMappingInformation.ScriptClass = nullptr; // Populated later
			typeMappingInformation.ScriptMetaData = &SelfType::sInteropMetaData;
			typeMappingInformation.CreateCallback = &CreateScriptObjectAndWrapper;

			ScriptExportedTypeMappings::GetGameObjectTypeMappings().Add(typeMappingInformation);
		}

		GameObjectHandle<NativeType> mNativeObjectStrongHandle;
	};

	// TODO - Doc
	template<typename NativeType, typename SelfType>
	class TScriptResourceWrapper : public TScriptObjectWrapper<SelfType> // TODO - Move to own file
	{
	public:
		TScriptResourceWrapper(const TResourceHandle<NativeType>& nativeObject, MonoObject* scriptObject)
			: TScriptObjectWrapper<SelfType>(nativeObject.Get(), scriptObject), mNativeObjectStrongHandle(nativeObject)
		{ }

		/** Returns the wrapped native object as a shared pointer. */
		const SPtr<NativeType>& GetNativeObjectAsShared() const { return mNativeObjectStrongHandle.GetShared(); }

		/** Returns the wrapped native object as a handle. */
		const TResourceHandle<NativeType>& GetNativeObjectAsHandle() const { return mNativeObjectStrongHandle; }

		// TODO - Doc
		static MonoObject* CreateScriptObjectAndWrapper(const HResource& nativeObject)
		{
			MonoObject* const scriptObject = SelfType::CreateScriptObject(false);
			B3DNew<SelfType>(B3DStaticResourceCast<NativeType>(nativeObject), scriptObject);

			return scriptObject;
		}

		// TODO - Doc
		static MonoObject* GetOrCreateScriptObject(const TResourceHandle<NativeType>& nativeObject)
		{
			if(ScriptObjectWrapper* const scriptObjectWrapper = (ScriptObjectWrapper*)nativeObject->GetScriptObjectWrapper())
				return scriptObjectWrapper->GetScriptObject();

			// TODO - This needs to perform RTTI ID lookup and call the creation callback on the returned info
			return CreateScriptObjectAndWrapper(nativeObject);
		}

	protected:
		friend class TScriptObjectWrapper<SelfType>;

		// TODO - Doc
		static void RegisterNativeToScriptTypeMapping()
		{
			TScriptExportedTypeInformation<HResource> typeMappingInformation;
			typeMappingInformation.TypeId = NativeType::GetRttiStatic()->GetRttiId();
			typeMappingInformation.ScriptClass = nullptr; // Populated later
			typeMappingInformation.ScriptMetaData = &SelfType::sInteropMetaData;
			typeMappingInformation.CreateCallback = &CreateScriptObjectAndWrapper;

			ScriptExportedTypeMappings::GetResourceTypeMappings().Add(typeMappingInformation);
		}

		TResourceHandle<NativeType> mNativeObjectStrongHandle;
	};

	// TODO - Add wrapper for GUI elements and non-reflectable classes

	// TODO - Doc
#define B3D_SCRIPT_OBJECT_WRAPPER(Assembly, Namespace, Name) \
	static const char* GetAssemblyName()      \
	{                                         \
		return Assembly;                      \
	}                                         \
	static const char* GetNamespace()         \
	{                                         \
		return Namespace;                     \
	}                                         \
	static const char* GetTypeName()          \
	{                                         \
		return Name;                          \
	}                                         \
	static void SetupScriptBindings();

	/**	Script object wrapper for ScriptObject. (Script prefix used as standard for script object wrappers, wrapping ScriptObject. Therefore ScriptScriptObject.) */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptScriptObject : public TNonInstantiableScriptObjectWrapper<ScriptScriptObject>
	{
	public:
		B3D_SCRIPT_OBJECT_WRAPPER(kEngineAssembly, kEngineNs, "ScriptObject")

	private:
		ScriptScriptObject(MonoObject* scriptObject);

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void Internal_ScriptObjectFinalizerCalled(ScriptObjectWrapper* scriptObjectWrapper);
	};

	/** @} */
} // namespace bs
