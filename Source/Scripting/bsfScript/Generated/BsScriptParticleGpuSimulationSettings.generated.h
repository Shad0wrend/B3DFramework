//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "Math/BsVector3.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"

namespace bs { struct ParticleGpuSimulationSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptParticleGpuSimulationSettings : public TScriptReflectable<ScriptParticleGpuSimulationSettings, ParticleGpuSimulationSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ParticleGpuSimulationSettings")

		ScriptParticleGpuSimulationSettings(MonoObject* managedInstance, const SPtr<ParticleGpuSimulationSettings>& value);

		static MonoObject* Create(const SPtr<ParticleGpuSimulationSettings>& value);

	private:
		static MonoObject* InternalGetvectorField(ScriptParticleGpuSimulationSettings* thisPtr);
		static void InternalSetvectorField(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr);
		static void InternalSetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr);
		static void InternalSetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value);
		static void InternalGetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* __output);
		static void InternalSetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* value);
		static float InternalGetdrag(ScriptParticleGpuSimulationSettings* thisPtr);
		static void InternalSetdrag(ScriptParticleGpuSimulationSettings* thisPtr, float value);
		static MonoObject* InternalGetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr);
		static void InternalSetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value);
	};
}
