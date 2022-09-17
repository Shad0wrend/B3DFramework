//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCRenderable.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCRenderable.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Mesh/BsMesh.h"
#include "../../../Foundation/bsfCore/Material/BsMaterial.h"

namespace bs
{
	ScriptCRenderable::ScriptCRenderable(MonoObject* managedInstance, const GameObjectHandle<CRenderable>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCRenderable::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setMesh", (void*)&ScriptCRenderable::InternalSetMesh);
		metaData.scriptClass->AddInternalCall("Internal_getMesh", (void*)&ScriptCRenderable::InternalGetMesh);
		metaData.scriptClass->AddInternalCall("Internal_setMaterial", (void*)&ScriptCRenderable::InternalSetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_setMaterial0", (void*)&ScriptCRenderable::InternalSetMaterial0);
		metaData.scriptClass->AddInternalCall("Internal_getMaterial", (void*)&ScriptCRenderable::InternalGetMaterial);
		metaData.scriptClass->AddInternalCall("Internal_setMaterials", (void*)&ScriptCRenderable::InternalSetMaterials);
		metaData.scriptClass->AddInternalCall("Internal_getMaterials", (void*)&ScriptCRenderable::InternalGetMaterials);
		metaData.scriptClass->AddInternalCall("Internal_setCullDistanceFactor", (void*)&ScriptCRenderable::InternalSetCullDistanceFactor);
		metaData.scriptClass->AddInternalCall("Internal_getCullDistanceFactor", (void*)&ScriptCRenderable::InternalGetCullDistanceFactor);
		metaData.scriptClass->AddInternalCall("Internal_setWriteVelocity", (void*)&ScriptCRenderable::InternalSetWriteVelocity);
		metaData.scriptClass->AddInternalCall("Internal_getWriteVelocity", (void*)&ScriptCRenderable::InternalGetWriteVelocity);
		metaData.scriptClass->AddInternalCall("Internal_setLayer", (void*)&ScriptCRenderable::InternalSetLayer);
		metaData.scriptClass->AddInternalCall("Internal_getLayer", (void*)&ScriptCRenderable::InternalGetLayer);
		metaData.scriptClass->AddInternalCall("Internal_getBounds", (void*)&ScriptCRenderable::InternalGetBounds);

	}

	void ScriptCRenderable::InternalSetMesh(ScriptCRenderable* thisPtr, MonoObject* mesh)
	{
		ResourceHandle<Mesh> tmpmesh;
		ScriptRRefBase* scriptmesh;
		scriptmesh = ScriptRRefBase::toNative(mesh);
		if(scriptmesh != nullptr)
			tmpmesh = static_resource_cast<Mesh>(scriptmesh->GetHandle());
		thisPtr->GetHandle()->SetMesh(tmpmesh);
	}

	MonoObject* ScriptCRenderable::InternalGetMesh(ScriptCRenderable* thisPtr)
	{
		ResourceHandle<Mesh> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMesh();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCRenderable::InternalSetMaterial(ScriptCRenderable* thisPtr, uint32_t idx, MonoObject* material)
	{
		ResourceHandle<Material> tmpmaterial;
		ScriptRRefBase* scriptmaterial;
		scriptmaterial = ScriptRRefBase::toNative(material);
		if(scriptmaterial != nullptr)
			tmpmaterial = static_resource_cast<Material>(scriptmaterial->GetHandle());
		thisPtr->GetHandle()->SetMaterial(idx, tmpmaterial);
	}

	void ScriptCRenderable::InternalSetMaterial0(ScriptCRenderable* thisPtr, MonoObject* material)
	{
		ResourceHandle<Material> tmpmaterial;
		ScriptRRefBase* scriptmaterial;
		scriptmaterial = ScriptRRefBase::toNative(material);
		if(scriptmaterial != nullptr)
			tmpmaterial = static_resource_cast<Material>(scriptmaterial->GetHandle());
		thisPtr->GetHandle()->SetMaterial(tmpmaterial);
	}

	MonoObject* ScriptCRenderable::InternalGetMaterial(ScriptCRenderable* thisPtr, uint32_t idx)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMaterial(idx);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCRenderable::InternalSetMaterials(ScriptCRenderable* thisPtr, MonoArray* materials)
	{
		Vector<ResourceHandle<Material>> vecmaterials;
		if(materials != nullptr)
		{
			ScriptArray arraymaterials(materials);
			vecmaterials.resize(arraymaterials.size());
			for(int i = 0; i < (int)arraymaterials.size(); i++)
			{
				ScriptRRefBase* scriptmaterials;
				scriptmaterials = ScriptRRefBase::toNative(arraymaterials.get<MonoObject*>(i));
				if(scriptmaterials != nullptr)
				{
					ResourceHandle<Material> arrayElemPtrmaterials = static_resource_cast<Material>(scriptmaterials->GetHandle());
					vecmaterials[i] = arrayElemPtrmaterials;
				}
			}
		}
		thisPtr->GetHandle()->SetMaterials(vecmaterials);
	}

	MonoArray* ScriptCRenderable::InternalGetMaterials(ScriptCRenderable* thisPtr)
	{
		Vector<ResourceHandle<Material>> vec__output;
		vec__output = thisPtr->GetHandle()->GetMaterials();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptRRefBase>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptRRefBase* script__output;
			script__output = ScriptResourceManager::Instance().getScriptRRef(vec__output[i]);
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptCRenderable::InternalSetCullDistanceFactor(ScriptCRenderable* thisPtr, float factor)
	{
		thisPtr->GetHandle()->SetCullDistanceFactor(factor);
	}

	float ScriptCRenderable::InternalGetCullDistanceFactor(ScriptCRenderable* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCullDistanceFactor();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRenderable::InternalSetWriteVelocity(ScriptCRenderable* thisPtr, bool enable)
	{
		thisPtr->GetHandle()->SetWriteVelocity(enable);
	}

	bool ScriptCRenderable::InternalGetWriteVelocity(ScriptCRenderable* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetWriteVelocity();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRenderable::InternalSetLayer(ScriptCRenderable* thisPtr, uint64_t layer)
	{
		thisPtr->GetHandle()->SetLayer(layer);
	}

	uint64_t ScriptCRenderable::InternalGetLayer(ScriptCRenderable* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLayer();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCRenderable::InternalGetBounds(ScriptCRenderable* thisPtr, Bounds* __output)
	{
		Bounds tmp__output;
		tmp__output = thisPtr->GetHandle()->GetBounds();

		*__output = tmp__output;
	}
}
