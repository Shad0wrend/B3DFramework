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

	// TODO - Doc
	struct ScriptObjectReloadPersistentData
	{
		ScriptObjectReloadPersistentData() {}

		explicit ScriptObjectReloadPersistentData(const Any& data)
			: Data(data)
		{}

		Any Data; // TODO - Don't use Any
	};

	// TODO - Doc
	template<typename NativeObjectType>
	class ScriptExportedNativeObjectStorage
	{ };

	// TODO - Doc
	template<typename NativeObjectType>
	class ScriptExportedNativeObjectStorage<SPtr<NativeObjectType>>
	{
		ScriptExportedNativeObjectStorage(SPtr<NativeObjectType> nativeObject)
			:mNativeObject(std::move(nativeObject))
		{ }

		// TODO - Doc
		NativeObjectType* Get() const { return mNativeObject.get(); }

		// TODO - Doc
		const SPtr<NativeObjectType>& GetShared() const { return mNativeObject; }

		// TODO - Doc
		void Clear() { mNativeObject = nullptr; }

		// TODO - Doc
		static NativeObjectType* GetRawPointer(const SPtr<NativeObjectType>& object) { return object.get(); }
	private:
		SPtr<NativeObjectType> mNativeObject;
	};

	// TODO - Doc
	template<typename NativeObjectType>
	class ScriptExportedNativeObjectStorage<TResourceHandle<NativeObjectType>>
	{
		ScriptExportedNativeObjectStorage(TResourceHandle<NativeObjectType> nativeObject)
			:mNativeObject(std::move(nativeObject))
		{ }

		// TODO - Doc
		NativeObjectType* Get() const { return mNativeObject.Get(); }

		// TODO - Doc
		const SPtr<NativeObjectType>& GetShared() const { return mNativeObject.GetShared(); }

		// TODO - Doc
		const TResourceHandle<NativeObjectType>& GetHandle() const { return mNativeObject; }

		// TODO - Doc
		void Clear() { mNativeObject = nullptr; }

		// TODO - Doc
		static NativeObjectType* GetRawPointer(const TResourceHandle<NativeObjectType>& object) { return object.Get(); }
	private:
		TResourceHandle<NativeObjectType> mNativeObject;
	};

	// TODO - Doc
	template<typename NativeObjectType>
	class ScriptExportedNativeObjectStorage<GameObjectHandle<NativeObjectType>>
	{
		ScriptExportedNativeObjectStorage(GameObjectHandle<NativeObjectType> nativeObject)
			:mNativeObject(std::move(nativeObject))
		{ }

		// TODO - Doc
		NativeObjectType* Get() const { return mNativeObject.Get(); }

		// TODO - Doc
		const SPtr<NativeObjectType>& GetShared() const { return mNativeObject.GetShared(); }

		// TODO - Doc
		const GameObjectHandle<NativeObjectType>& GetHandle() const { return mNativeObject; }

		// TODO - Doc
		void Clear() { mNativeObject = nullptr; }

		// TODO - Doc
		static NativeObjectType* GetRawPointer(const GameObjectHandle<NativeObjectType>& object) { return object.Get(); }
	private:
		GameObjectHandle<NativeObjectType> mNativeObject;
	};

	// TODO - Doc
	class B3D_SCRIPT_INTEROP_EXPORT ScriptObjectWrapper : public IScriptObjectWrapper
	{
	public:
		ScriptObjectWrapper(IScriptExportable* nativeObject, MonoObject* scriptObject);
		virtual ~ScriptObjectWrapper();

		/** Called when the script system is notified that the script object has been destroyed. */
		virtual void NotifyScriptObjectDestroyed(bool isDestroyedDueToScriptReload);

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
		virtual void ReleaseStrongHandlesBeforeScriptReload() { }

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
	};

	template <typename NativeTypeContainer, typename SelfType, typename ScriptWrapperObjectBaseClass>
	class TScriptObjectWrapper;

	// TODO - Doc
	template <typename NativeTypeContainer, typename SelfType, typename ScriptWrapperObjectBaseClass>
	struct InitializeScriptObjectWrapperOnLoadTime
	{
	public:
		InitializeScriptObjectWrapperOnLoadTime()
		{
			TScriptObjectWrapper<NativeTypeContainer, SelfType, ScriptWrapperObjectBaseClass>::InitializeMetaDataAtLoadTime();
		}

		void MakeSureIAmInstantiated() {}
	};

	/**	Template version of ScriptObjectBase populates the object meta-data on library load. */
	template <typename NativeTypeContainerType, typename SelfType, typename ScriptWrapperObjectBaseClass = ScriptObjectWrapper>
	class TScriptObjectWrapper : public ScriptWrapperObjectBaseClass 
	{
	public:
		TScriptObjectWrapper(NativeTypeContainerType nativeObject, MonoObject* scriptObject)
			: ScriptWrapperObjectBaseClass(ScriptExportedNativeObjectStorage<NativeTypeContainerType>::GetRawPointer(nativeObject), scriptObject), mNativeObjectStorage(std::move(nativeObject))
		{
			sInitializeOnLoadTime.MakeSureIAmInstantiated();

			SelfType* self = (SelfType*)(ScriptWrapperObjectBaseClass*)this; // Needed due to multiple inheritance. Safe since SelfType must point to an class derived from this one.

			if(sInteropMetaData.ThisPtrField != nullptr)
				sInteropMetaData.ThisPtrField->Set(scriptObject, &self);
		}

		virtual ~TScriptObjectWrapper() = default;

		// TODO - Doc
		virtual MonoObject* CreateScriptObject(bool construct)
		{
			return sInteropMetaData.ScriptClass->CreateInstance(construct);
		}

		// TODO - Doc
		static SelfType* ToNative(MonoObject* scriptObject)
		{
			SelfType* scriptObjectWrapper = nullptr;

			if(sInteropMetaData.ThisPtrField != nullptr && scriptObject != nullptr)
				sInteropMetaData.ThisPtrField->Get(scriptObject, &scriptObjectWrapper);

			return scriptObjectWrapper;
		}

		// TODO - Doc
		static const ScriptMeta* GetMetaData() { return &sInteropMetaData; }

		// TODO - Doc
		static void InitializeMetaDataAtLoadTime()
		{
			// Need to delay init of sInteropMetaData since it's also a static, and we can't guarantee the order
			// (if it gets initialized after this, it will just overwrite the data)
			ScriptMeta localMetaData = ScriptMeta(SelfType::GetAssemblyName(), SelfType::GetNamespace(), SelfType::GetTypeName(), &SelfType::InitRuntimeData);

			MonoManager::RegisterScriptType(&sInteropMetaData, localMetaData);
		}

		// TODO - When object is finalized, ScriptObjectManager needs to notify ScriptObjectWrapper

	protected:
		static ScriptMeta sInteropMetaData;
		static InitializeScriptObjectWrapperOnLoadTime<NativeTypeContainerType, SelfType, ScriptWrapperObjectBaseClass> sInitializeOnLoadTime;

		ScriptExportedNativeObjectStorage<NativeTypeContainerType> mNativeObjectStorage;
	};

	template <typename NativeTypeContainerType, typename SelfType, typename ScriptWrapperObjectBaseClass>
	InitializeScriptObjectWrapperOnLoadTime<NativeTypeContainerType, SelfType, ScriptWrapperObjectBaseClass> TScriptObjectWrapper<NativeTypeContainerType, SelfType, ScriptWrapperObjectBaseClass>::sInitializeOnLoadTime;

	template <typename NativeTypeContainerType, typename SelfType, typename ScriptWrapperObjectBaseClass>
	ScriptMeta TScriptObjectWrapper<NativeTypeContainerType, SelfType, ScriptWrapperObjectBaseClass>::sInteropMetaData;

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
	static void InitRuntimeData();

	/** @} */
} // namespace bs
