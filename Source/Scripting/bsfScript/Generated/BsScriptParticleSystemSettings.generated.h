//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "Math/BsAABox.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "Math/BsVector3.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"

namespace bs { struct ParticleSystemSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptParticleSystemSettings : public TScriptReflectable<ScriptParticleSystemSettings, ParticleSystemSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ParticleSystemSettings")

		ScriptParticleSystemSettings(MonoObject* managedInstance, const SPtr<ParticleSystemSettings>& value);

		static MonoObject* Create(const SPtr<ParticleSystemSettings>& value);

	private:
		static MonoObject* InternalGetmaterial(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetmaterial(ScriptParticleSystemSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetmesh(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetmesh(ScriptParticleSystemSettings* thisPtr, MonoObject* value);
		static ParticleSimulationSpace InternalGetsimulationSpace(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetsimulationSpace(ScriptParticleSystemSettings* thisPtr, ParticleSimulationSpace value);
		static ParticleOrientation InternalGetorientation(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetorientation(ScriptParticleSystemSettings* thisPtr, ParticleOrientation value);
		static float InternalGetduration(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetduration(ScriptParticleSystemSettings* thisPtr, float value);
		static bool InternalGetisLooping(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetisLooping(ScriptParticleSystemSettings* thisPtr, bool value);
		static uint32_t InternalGetmaxParticles(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetmaxParticles(ScriptParticleSystemSettings* thisPtr, uint32_t value);
		static bool InternalGetgpuSimulation(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetgpuSimulation(ScriptParticleSystemSettings* thisPtr, bool value);
		static ParticleRenderMode InternalGetrenderMode(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetrenderMode(ScriptParticleSystemSettings* thisPtr, ParticleRenderMode value);
		static bool InternalGetorientationLockY(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetorientationLockY(ScriptParticleSystemSettings* thisPtr, bool value);
		static void InternalGetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* __output);
		static void InternalSetorientationPlaneNormal(ScriptParticleSystemSettings* thisPtr, Vector3* value);
		static ParticleSortMode InternalGetsortMode(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetsortMode(ScriptParticleSystemSettings* thisPtr, ParticleSortMode value);
		static bool InternalGetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetuseAutomaticSeed(ScriptParticleSystemSettings* thisPtr, bool value);
		static uint32_t InternalGetmanualSeed(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetmanualSeed(ScriptParticleSystemSettings* thisPtr, uint32_t value);
		static bool InternalGetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr);
		static void InternalSetuseAutomaticBounds(ScriptParticleSystemSettings* thisPtr, bool value);
		static void InternalGetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* __output);
		static void InternalSetcustomBounds(ScriptParticleSystemSettings* thisPtr, AABox* value);
	};
}
