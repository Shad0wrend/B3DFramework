//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleSystem.h"
#include "Math/BsVector3.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleDistribution.h"
#include "Math/BsQuaternion.h"

namespace bs { struct ParticleVectorFieldSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptParticleVectorFieldSettings : public TScriptReflectable<ScriptParticleVectorFieldSettings, ParticleVectorFieldSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ParticleVectorFieldSettings")

		ScriptParticleVectorFieldSettings(MonoObject* managedInstance, const SPtr<ParticleVectorFieldSettings>& value);

		static MonoObject* Create(const SPtr<ParticleVectorFieldSettings>& value);

	private:
		static MonoObject* InternalGetvectorField(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSetvectorField(ScriptParticleVectorFieldSettings* thisPtr, MonoObject* value);
		static float InternalGetintensity(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSetintensity(ScriptParticleVectorFieldSettings* thisPtr, float value);
		static float InternalGettightness(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSettightness(ScriptParticleVectorFieldSettings* thisPtr, float value);
		static void InternalGetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output);
		static void InternalSetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value);
		static void InternalGetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output);
		static void InternalSetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value);
		static void InternalGetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* __output);
		static void InternalSetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* value);
		static MonoObject* InternalGetrotationRate(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSetrotationRate(ScriptParticleVectorFieldSettings* thisPtr, MonoObject* value);
		static bool InternalGettilingX(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSettilingX(ScriptParticleVectorFieldSettings* thisPtr, bool value);
		static bool InternalGettilingY(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSettilingY(ScriptParticleVectorFieldSettings* thisPtr, bool value);
		static bool InternalGettilingZ(ScriptParticleVectorFieldSettings* thisPtr);
		static void InternalSettilingZ(ScriptParticleVectorFieldSettings* thisPtr, bool value);
	};
}
