//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsEngineScriptLibrary.h"
#include "BsMonoManager.h"
#include "BsMonoAssembly.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsScriptResourceManager.h"
#include "BsScriptGameObjectManager.h"
#include "BsManagedResourceManager.h"
#include "Script/BsScriptManager.h"
#include "Wrappers/BsScriptInput.h"
#include "Wrappers/BsScriptVirtualInput.h"
#include "BsScriptObjectManager.h"
#include "Resources/BsGameResourceManager.h"
#include "BsApplication.h"
#include "FileSystem/BsFileSystem.h"
#include "Wrappers/BsScriptDebug.h"
#include "Wrappers/GUI/BsScriptGUI.h"
#include "BsPlayInEditor.h"
#include "Wrappers/BsScriptScene.h"
#include "GUI/BsGUIManager.h"

#include "Serialization/BsBuiltinResourceLookup.h"
#include "Generated/BsBuiltinComponentLookup.generated.h"
#include "Generated/BsBuiltinReflectableTypesLookup.generated.h"

namespace bs
{
	void EngineScriptLibrary::Initialize()
	{
		Path engineAssemblyPath = getEngineAssemblyPath();
		const String ASSEMBLY_ENTRY_POINT = "Program::Start";

#if BS_IS_BANSHEE3D
		MonoManager::StartUp();
		MonoAssembly& engineAssembly = MonoManager::Instance().loadAssembly(engineAssemblyPath.toString(), ENGINE_ASSEMBLY);
#endif

		PlayInEditor::StartUp();
		ScriptDebug::StartUp();
		GameResourceManager::StartUp();
		ScriptObjectManager::StartUp();
		ManagedResourceManager::StartUp();
		ScriptAssemblyManager::StartUp();
		ScriptResourceManager::StartUp();
		ScriptGameObjectManager::StartUp();
		ScriptScene::StartUp();
		ScriptInput::StartUp();
		ScriptVirtualInput::StartUp();
		ScriptGUI::StartUp();

		mEngineTypeMappings.resources = BuiltinResourceTypes::getEntries();
		mEngineTypeMappings.components = BuiltinComponent::getEntries();
		mEngineTypeMappings.reflectableObjects = BuiltinReflectableTypes::getEntries();

		ScriptAssemblyManager::Instance().loadAssemblyInfo(ENGINE_ASSEMBLY, mEngineTypeMappings);

#if BS_IS_BANSHEE3D
		engineAssembly.invoke(ASSEMBLY_ENTRY_POINT);
#endif
	}

	void EngineScriptLibrary::Update()
	{
		ScriptScene::Update();
		PlayInEditor::Instance().update();
		ScriptObjectManager::Instance().update();
		ScriptGUI::Update();
	}

	void EngineScriptLibrary::Reload()
	{
#if BS_IS_BANSHEE3D
		Path engineAssemblyPath = getEngineAssemblyPath();

		// Do a full refresh if we have already loaded script assemblies
		if (mScriptAssembliesLoaded)
		{
			Vector<AssemblyRefreshInfo> assemblies;
			assemblies.push_back(AssemblyRefreshInfo(ENGINE_ASSEMBLY, &engineAssemblyPath, &mEngineTypeMappings));

			Path gameAssemblyPath = getGameAssemblyPath();
			if (FileSystem::exists(gameAssemblyPath))
				assemblies.push_back(AssemblyRefreshInfo(SCRIPT_GAME_ASSEMBLY, &gameAssemblyPath, &BuiltinTypeMappings::EMPTY));

			ScriptObjectManager::Instance().refreshAssemblies(assemblies);
		}
		else // Otherwise just additively load them
		{
			Path gameAssemblyPath = getGameAssemblyPath();
			if (FileSystem::exists(gameAssemblyPath))
			{
				MonoManager::Instance().loadAssembly(gameAssemblyPath.toString(), SCRIPT_GAME_ASSEMBLY);
				ScriptAssemblyManager::Instance().loadAssemblyInfo(SCRIPT_GAME_ASSEMBLY, BuiltinTypeMappings());
			}

			mScriptAssembliesLoaded = true;
		}
#endif
	}

	void EngineScriptLibrary::Destroy()
	{
		unloadAssemblies();
		shutdownModules();
	}

	void EngineScriptLibrary::UnloadAssemblies()
	{
		ManagedResourceManager::Instance().clear();
		MonoManager::Instance().unloadScriptDomain();
		ScriptObjectManager::Instance().processFinalizedObjects();
	}

	void EngineScriptLibrary::ShutdownModules()
	{
		ScriptGUI::ShutDown();
		ScriptVirtualInput::ShutDown();
		ScriptInput::ShutDown();
		ScriptScene::ShutDown();
		ManagedResourceManager::ShutDown();

#if BS_IS_BANSHEE3D
		MonoManager::ShutDown();
#else
		MonoManager::Instance().unloadAll();
#endif

		ScriptGameObjectManager::ShutDown();
		ScriptResourceManager::ShutDown();
		ScriptAssemblyManager::ShutDown();
		ScriptObjectManager::ShutDown();
		GameResourceManager::ShutDown();
		ScriptDebug::ShutDown();
		PlayInEditor::ShutDown();

		// Make sure all GUI elements are actually destroyed
		GUIManager::Instance().processDestroyQueue();
	}

	Path EngineScriptLibrary::GetEngineAssemblyPath() const
	{
		Path assemblyPath = getBuiltinAssemblyFolder();
		assemblyPath.append(String(ENGINE_ASSEMBLY) + ".dll");

		return assemblyPath;
	}

#if BS_IS_BANSHEE3D
	Path EngineScriptLibrary::GetGameAssemblyPath() const
	{
		Path assemblyPath = getScriptAssemblyFolder();
		assemblyPath.append(String(SCRIPT_GAME_ASSEMBLY) + ".dll");

		return assemblyPath;
	}
#endif

	Path EngineScriptLibrary::GetBuiltinAssemblyFolder() const
	{
		Path releaseAssemblyFolder = getReleaseAssemblyPath();
		Path debugAssemblyFolder = getDebugAssemblyPath();

#if BS_DEBUG_MODE == 0
		if (FileSystem::exists(releaseAssemblyFolder))
			return releaseAssemblyFolder;

		return debugAssemblyFolder;
#else
		if (FileSystem::exists(debugAssemblyFolder))
			return debugAssemblyFolder;

		return releaseAssemblyFolder;
#endif
	}

	Path EngineScriptLibrary::GetScriptAssemblyFolder() const
	{
		return getBuiltinAssemblyFolder();
	}

	const Path& EngineScriptLibrary::GetReleaseAssemblyPath()
	{
		static Path path = Paths::findPath(Paths::RELEASE_ASSEMBLY_PATH);
		return path;
	}

	const Path& EngineScriptLibrary::GetDebugAssemblyPath()
	{
		static Path path = Paths::findPath(Paths::DEBUG_ASSEMBLY_PATH);
		return path;
	}
}
