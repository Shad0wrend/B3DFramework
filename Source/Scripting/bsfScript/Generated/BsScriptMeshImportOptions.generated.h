//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../../Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../../Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "../../../Foundation/bsfCore/Importer/BsMeshImportOptions.h"

namespace bs { class MeshImportOptions; }
namespace bs
{
#if !BS_IS_BANSHEE3D
	class BS_SCR_BE_EXPORT ScriptMeshImportOptions : public TScriptReflectable<ScriptMeshImportOptions, MeshImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "MeshImportOptions")

		ScriptMeshImportOptions(MonoObject* managedInstance, const SPtr<MeshImportOptions>& value);

		static MonoObject* Create(const SPtr<MeshImportOptions>& value);

	private:
		static bool InternalGetcpuCached(ScriptMeshImportOptions* thisPtr);
		static void InternalSetcpuCached(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportNormals(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportNormals(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportTangents(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportTangents(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportBlendShapes(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportBlendShapes(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportSkin(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportSkin(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportAnimation(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportAnimation(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetreduceKeyFrames(ScriptMeshImportOptions* thisPtr);
		static void InternalSetreduceKeyFrames(ScriptMeshImportOptions* thisPtr, bool value);
		static bool InternalGetimportRootMotion(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportRootMotion(ScriptMeshImportOptions* thisPtr, bool value);
		static float InternalGetimportScale(ScriptMeshImportOptions* thisPtr);
		static void InternalSetimportScale(ScriptMeshImportOptions* thisPtr, float value);
		static CollisionMeshType InternalGetcollisionMeshType(ScriptMeshImportOptions* thisPtr);
		static void InternalSetcollisionMeshType(ScriptMeshImportOptions* thisPtr, CollisionMeshType value);
		static MonoArray* InternalGetanimationSplits(ScriptMeshImportOptions* thisPtr);
		static void InternalSetanimationSplits(ScriptMeshImportOptions* thisPtr, MonoArray* value);
		static MonoArray* InternalGetanimationEvents(ScriptMeshImportOptions* thisPtr);
		static void InternalSetanimationEvents(ScriptMeshImportOptions* thisPtr, MonoArray* value);
		static void InternalCreate(MonoObject* managedInstance);
	};
#endif
}
