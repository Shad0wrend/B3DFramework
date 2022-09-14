//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"

namespace bs { struct ParticleDepthCollisionSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptParticleDepthCollisionSettings : public TScriptReflectable<ScriptParticleDepthCollisionSettings, ParticleDepthCollisionSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ParticleDepthCollisionSettings")

		ScriptParticleDepthCollisionSettings(MonoObject* managedInstance, const SPtr<ParticleDepthCollisionSettings>& value);

		static MonoObject* Create(const SPtr<ParticleDepthCollisionSettings>& value);

	private:
		static void InternalParticleDepthCollisionSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptParticleDepthCollisionSettings* thisPtr);
		static void InternalSetenabled(ScriptParticleDepthCollisionSettings* thisPtr, bool value);
		static float InternalGetrestitution(ScriptParticleDepthCollisionSettings* thisPtr);
		static void InternalSetrestitution(ScriptParticleDepthCollisionSettings* thisPtr, float value);
		static float InternalGetdampening(ScriptParticleDepthCollisionSettings* thisPtr);
		static void InternalSetdampening(ScriptParticleDepthCollisionSettings* thisPtr, float value);
		static float InternalGetradiusScale(ScriptParticleDepthCollisionSettings* thisPtr);
		static void InternalSetradiusScale(ScriptParticleDepthCollisionSettings* thisPtr, float value);
	};
}
