//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Importer/BsImportOptions.h"

namespace bs
{
	class ImportOptions;
}

namespace bs
{
#if !BS_IS_BANSHEE3D
	class B3D_SCRIPT_INTEROP_EXPORT ScriptImportOptionsBase : public ScriptReflectableBase
	{
	public:
		ScriptImportOptionsBase(MonoObject* instance);

		virtual ~ScriptImportOptionsBase() {}

		SPtr<ImportOptions> GetInternal() const;
	};

	class B3D_SCRIPT_INTEROP_EXPORT ScriptImportOptions : public TScriptReflectable<ScriptImportOptions, ImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ImportOptions")

		ScriptImportOptions(MonoObject* managedInstance, const SPtr<ImportOptions>& value);

		static MonoObject* Create(const SPtr<ImportOptions>& value);

	private:
	};
#endif
} // namespace bs
