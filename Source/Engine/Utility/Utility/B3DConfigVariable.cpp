//************************************ B3D Framework - Copyright 2026 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/B3DConfigVariable.h"
#include "Utility/B3DCommandLine.h"
#include "FileSystem/B3DFileSystem.h"
#include "FileSystem/B3DDataStream.h"
#include "Debug/B3DDebug.h"

namespace b3d
{
	B3D_LOG_CATEGORY_STATIC(LogConfigVariable, Log)

	// Thread-safe registration helper for static initialization order safety
	namespace ConfigVariableRegistry
	{
		static std::atomic<bool> sManagerStarted{false};
		static std::mutex sPendingMutex;

		static Vector<ConfigVariable*>& GetPendingVariables()
		{
			static Vector<ConfigVariable*> sPendingVariables;
			return sPendingVariables;
		}

		void Register(ConfigVariable* variable)
		{
			// Fast path: manager already started
			if (sManagerStarted.load(std::memory_order_acquire))
			{
				ConfigVariableManager::Instance().RegisterVariable(variable);
				return;
			}

			// Slow path: add to pending list with lock
			Lock lock(sPendingMutex);
			if (sManagerStarted.load(std::memory_order_relaxed))
			{
				ConfigVariableManager::Instance().RegisterVariable(variable);
				return;
			}

			GetPendingVariables().push_back(variable);
		}

		void Unregister(ConfigVariable* variable)
		{
			if (sManagerStarted.load(std::memory_order_acquire))
			{
				if (ConfigVariableManager::IsStarted())
					ConfigVariableManager::Instance().UnregisterVariable(variable);

				return;
			}

			// If manager hasn't started, remove from pending list
			Lock lock(sPendingMutex);
			if (sManagerStarted.load(std::memory_order_relaxed))
			{
				if (ConfigVariableManager::IsStarted())
					ConfigVariableManager::Instance().UnregisterVariable(variable);

				return;
			}

			Vector<ConfigVariable*>& pending = GetPendingVariables();

			auto found = std::find(pending.begin(), pending.end(), variable);
			if (found != pending.end())
				pending.erase(found);
		}

		void OnManagerStarted()
		{
			Lock lock(sPendingMutex);

			Vector<ConfigVariable*>& pending = GetPendingVariables();
			for (ConfigVariable* variable : pending)
				ConfigVariableManager::Instance().RegisterVariable(variable);

			pending.clear();
			pending.shrink_to_fit();
			sManagerStarted.store(true, std::memory_order_release);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ConfigVariable
	//////////////////////////////////////////////////////////////////////////

	ConfigVariable::ConfigVariable(const char* name, const char* description, ConfigVariableFlags flags)
		: mName(name), mDescription(description), mFlags(flags)
	{
		ConfigVariableRegistry::Register(this);
	}

	ConfigVariable::~ConfigVariable()
	{
		ConfigVariableRegistry::Unregister(this);
	}

	//////////////////////////////////////////////////////////////////////////
	// TConfigVariable specializations
	//////////////////////////////////////////////////////////////////////////

	template<>
	const char* TConfigVariable<bool>::GetTypeName() const { return "bool"; }

	template<>
	const char* TConfigVariable<i32>::GetTypeName() const { return "i32"; }

	template<>
	const char* TConfigVariable<u32>::GetTypeName() const { return "u32"; }

	template<>
	const char* TConfigVariable<float>::GetTypeName() const { return "float"; }

	template<>
	bool TConfigVariable<bool>::SetFromString(const String& value, ConfigVariableSource source)
	{
		SetValueWithoutChecks(ParseBool(value), source);
		return true;
	}

	template<>
	bool TConfigVariable<i32>::SetFromString(const String& value, ConfigVariableSource source)
	{
		SetValueWithoutChecks(ParseI32(value), source);
		return true;
	}

	template<>
	bool TConfigVariable<u32>::SetFromString(const String& value, ConfigVariableSource source)
	{
		SetValueWithoutChecks(ParseU32(value), source);
		return true;
	}

	template<>
	bool TConfigVariable<float>::SetFromString(const String& value, ConfigVariableSource source)
	{
		SetValueWithoutChecks(ParseFloat(value), source);
		return true;
	}

	// Explicit template instantiations
	template class B3D_EXPORT TConfigVariable<bool>;
	template class B3D_EXPORT TConfigVariable<i32>;
	template class B3D_EXPORT TConfigVariable<u32>;
	template class B3D_EXPORT TConfigVariable<float>;

	//////////////////////////////////////////////////////////////////////////
	// ConfigVariableManager
	//////////////////////////////////////////////////////////////////////////

	void ConfigVariableManager::OnStartUp()
	{
		ConfigVariableRegistry::OnManagerStarted();

		// Load config file (if exists)
		Path configPath = FileSystem::GetWorkingFolderPath();
		configPath.SetFilename("engine.ini");
		LoadFromFile(configPath);

		// Apply command-line overrides (highest priority)
		ApplyCommandLineOverrides();

		// Enable ReadOnly enforcement
		FinalizeInitialization();
	}

	void ConfigVariableManager::RegisterVariable(ConfigVariable* variable)
	{
		std::lock_guard<std::mutex> lock(mMutex);

		const String name(variable->GetName());
		auto iter = mVariables.find(name);
		if (iter != mVariables.end())
		{
			B3D_LOG(Warning, LogConfigVariable, "Duplicate config variable registration: {0}", name);
			return;
		}

		mVariables[name] = variable;

		if (variable->IsRenderThreadSafe())
			mRenderThreadSafeVariables.push_back(variable);
	}

	void ConfigVariableManager::UnregisterVariable(ConfigVariable* variable)
	{
		std::lock_guard<std::mutex> lock(mMutex);

		const String name(variable->GetName());
		mVariables.erase(name);

		if (variable->IsRenderThreadSafe())
		{
			auto iter = std::find(mRenderThreadSafeVariables.begin(), mRenderThreadSafeVariables.end(), variable);
			if (iter != mRenderThreadSafeVariables.end())
				mRenderThreadSafeVariables.erase(iter);
		}
	}

	bool ConfigVariableManager::LoadFromFile(const Path& path)
	{
		if (!FileSystem::Exists(path))
		{
			B3D_LOG(Info, LogConfigVariable, "Config file not found: {0}", path);
			return false;
		}

		SPtr<DataStream> stream = FileSystem::OpenFile(path, true);
		if (stream == nullptr)
		{
			B3D_LOG(Warning, LogConfigVariable, "Failed to open config file: {0}", path);
			return false;
		}

		String content = stream->GetAsString();
		stream->Close();

		StringView remaining(content);
		u32 lineNumber = 0;

		while (!remaining.empty())
		{
			lineNumber++;

			// Find end of line
			size_t lineEnd = remaining.find('\n');
			StringView line;
			if (lineEnd != StringView::npos)
			{
				line = remaining.substr(0, lineEnd);
				remaining = remaining.substr(lineEnd + 1);
			}
			else
			{
				line = remaining;
				remaining = StringView();
			}

			// Strip carriage return if present (Windows line endings)
			if (!line.empty() && line.back() == '\r')
				line = line.substr(0, line.size() - 1);

			ParseIniLine(line, lineNumber, path);
		}

		B3D_LOG(Info, LogConfigVariable, "Loaded config file: {0}", path.ToString());
		return true;
	}

	void ConfigVariableManager::ParseIniLine(StringView line, u32 lineNumber, const Path& filePath)
	{
		StringView trimmedLine = StringUtility::Trim(line);

		// Skip empty lines and comments
		if (trimmedLine.empty() || trimmedLine[0] == '#')
			return;

		// Remove inline comments
		size_t commentPos = trimmedLine.find('#');
		if (commentPos != StringView::npos)
			trimmedLine = StringUtility::Trim(trimmedLine.substr(0, commentPos));

		// Find the = separator
		size_t equalsPos = trimmedLine.find('=');
		if (equalsPos == StringView::npos)
		{
			B3D_LOG(Warning, LogConfigVariable, "Invalid config line {0} in {1}: missing '='", lineNumber, filePath);
			return;
		}

		StringView key = StringUtility::Trim(trimmedLine.substr(0, equalsPos));
		StringView value = StringUtility::Trim(trimmedLine.substr(equalsPos + 1));

		if (key.empty())
		{
			B3D_LOG(Warning, LogConfigVariable, "Invalid config line {0} in {1}: empty key", lineNumber, filePath);
			return;
		}

		// Create String for map lookup (required for current map implementation)
		ConfigVariable* variable = FindVariable(String(key));
		if (variable == nullptr)
		{
			B3D_LOG(Warning, LogConfigVariable, "Unknown config variable '{0}' at line {1} in {2}", key, lineNumber, filePath);
			return;
		}

		variable->SetFromString(String(value), ConfigVariableSource::ConfigFile);
	}

	void ConfigVariableManager::ApplyCommandLineOverrides()
	{
		Lock lock(mMutex);

		for (auto& pair : mVariables)
		{
			const String& name = pair.first;
			ConfigVariable* variable = pair.second;

			if (CommandLine::HasParameter(name))
			{
				String value = CommandLine::GetParameterValue(name);
				variable->SetFromString(value, ConfigVariableSource::CommandLine);
			}
		}
	}

	void ConfigVariableManager::FinalizeInitialization()
	{
		Lock lock(mMutex);

		for (auto& pair : mVariables)
			pair.second->MarkInitialized();
	}

	void ConfigVariableManager::ApplyPendingUpdates()
	{
		// No lock needed - mRenderThreadSafeVariables is only modified during registration
		// which happens before the main loop starts
		for (ConfigVariable* variable : mRenderThreadSafeVariables)
			variable->ApplyPendingUpdate();
	}

	void ConfigVariableManager::PrintHelp() const
	{
		Lock lock(mMutex);

		// Collect and sort variable names
		Vector<String> names;
		names.reserve(mVariables.size());
		for (const auto& pair : mVariables)
			names.push_back(pair.first);

		std::sort(names.begin(), names.end());

		std::cout << "\nConfiguration Variables:\n";
		std::cout << "========================\n\n";

		String currentPrefix;
		for (const String& name : names)
		{
			ConfigVariable* variable = mVariables.at(name);

			// Group by prefix (everything before first '.')
			size_t dotPos = name.find('.');
			String prefix = (dotPos != String::npos) ? name.substr(0, dotPos) : "";
			if (prefix != currentPrefix)
			{
				if (!currentPrefix.empty())
					std::cout << "\n";
				currentPrefix = prefix;
			}

			// Print variable info
			std::cout << "  " << name << " (" << variable->GetTypeName() << ")\n";
			std::cout << "      " << variable->GetDescription() << "\n";
			std::cout << "      Default: " << variable->GetDefaultValueAsString().c_str();

			if (variable->IsReadOnly())
				std::cout << " [ReadOnly]";
			if (variable->IsRenderThreadSafe())
				std::cout << " [RenderThreadSafe]";

			std::cout << "\n";
		}

		std::cout << "\nUsage:\n";
		std::cout << "  Command line: --variable.name=value\n";
		std::cout << "  Config file (engine.ini): variable.name = value\n";
		std::cout << std::endl;
	}

	ConfigVariable* ConfigVariableManager::FindVariable(const String& name) const
	{
		Lock lock(mMutex);

		auto found = mVariables.find(name);
		if (found != mVariables.end())
			return found->second;

		return nullptr;
	}

	ConfigVariableManager& GetConfigVariableManager()
	{
		return ConfigVariableManager::Instance();
	}

} // namespace b3d
