//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPARTICLE_SKINNED_MESH_SHAPE_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsRenderable.h"
#include "BsScriptRenderable.generated.h"

namespace b3d
{
	ScriptParticleSkinnedMeshShapeOptions::ScriptParticleSkinnedMeshShapeOptions()
	{ }

	MonoObject* ScriptParticleSkinnedMeshShapeOptions::Box(const __PARTICLE_SKINNED_MESH_SHAPE_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__PARTICLE_SKINNED_MESH_SHAPE_DESCInterop ScriptParticleSkinnedMeshShapeOptions::Unbox(MonoObject* value)
	{
		return *(__PARTICLE_SKINNED_MESH_SHAPE_DESCInterop*)MonoUtil::Unbox(value);
	}

	ParticleSkinnedMeshShapeSettings ScriptParticleSkinnedMeshShapeOptions::FromInterop(const __PARTICLE_SKINNED_MESH_SHAPE_DESCInterop& value)
	{
		ParticleSkinnedMeshShapeSettings output;
		output.Type = value.Type;
		output.Sequential = value.Sequential;
		GameObjectHandle<Renderable> tmpRenderable;
		ScriptRenderable* scriptObjectWrapperRenderable;
		scriptObjectWrapperRenderable = ScriptRenderable::GetScriptObjectWrapper(value.Renderable);
		if(scriptObjectWrapperRenderable != nullptr)
			tmpRenderable = B3DStaticGameObjectCast<Renderable>(scriptObjectWrapperRenderable->GetBaseNativeObjectAsHandle());
		output.Renderable = tmpRenderable;

		return output;
	}

	__PARTICLE_SKINNED_MESH_SHAPE_DESCInterop ScriptParticleSkinnedMeshShapeOptions::ToInterop(const ParticleSkinnedMeshShapeSettings& value)
	{
		__PARTICLE_SKINNED_MESH_SHAPE_DESCInterop output;
		output.Type = value.Type;
		output.Sequential = value.Sequential;
		MonoObject* tmpRenderable;
		MonoObject* temptmpRenderable = nullptr;
		if(value.Renderable)
			temptmpRenderable = ScriptComponent::GetOrCreateScriptObject(value.Renderable);
		tmpRenderable = temptmpRenderable;
		output.Renderable = tmpRenderable;

		return output;
	}

}
