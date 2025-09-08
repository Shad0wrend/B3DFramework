//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptReflectableWrapper.h"
#include "BsScriptParticleEmitterShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"

namespace b3d { class ParticleEmitterHemisphereShape; }
namespace b3d
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptParticleEmitterHemisphereShape : public TScriptReflectableWrapper<ParticleEmitterHemisphereShape, ScriptParticleEmitterHemisphereShape, ScriptParticleEmitterShapeWrapperBase>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "ParticleEmitterHemisphereShape")

		ScriptParticleEmitterHemisphereShape(const SPtr<ParticleEmitterHemisphereShape>& nativeObject);
		~ScriptParticleEmitterHemisphereShape();

		static void SetupScriptBindings();

		static MonoObject* CreateScriptObject(bool construct);

	private:
		static void InternalSetOptions(ScriptParticleEmitterHemisphereShape* self, ParticleHemisphereShapeSettings* options);
		static void InternalGetOptions(ScriptParticleEmitterHemisphereShape* self, ParticleHemisphereShapeSettings* __output);
		static void InternalCreate(MonoObject* scriptObject, ParticleHemisphereShapeSettings* desc);
		static void InternalCreate0(MonoObject* scriptObject);
	};
}
