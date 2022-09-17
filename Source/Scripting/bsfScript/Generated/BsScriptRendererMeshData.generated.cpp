//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptRendererMeshData.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Renderer/BsRendererMeshData.h"
#include "BsScriptRendererMeshData.generated.h"
#include "BsScriptBoneWeight.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "../Extensions/BsMeshDataEx.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptColor.h"

namespace bs
{
	ScriptRendererMeshData::ScriptRendererMeshData(MonoObject* managedInstance, const SPtr<RendererMeshData>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptRendererMeshData::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptRendererMeshData::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_getPositions", (void*)&ScriptRendererMeshData::InternalGetPositions);
		metaData.scriptClass->AddInternalCall("Internal_setPositions", (void*)&ScriptRendererMeshData::InternalSetPositions);
		metaData.scriptClass->AddInternalCall("Internal_getNormals", (void*)&ScriptRendererMeshData::InternalGetNormals);
		metaData.scriptClass->AddInternalCall("Internal_setNormals", (void*)&ScriptRendererMeshData::InternalSetNormals);
		metaData.scriptClass->AddInternalCall("Internal_getTangents", (void*)&ScriptRendererMeshData::InternalGetTangents);
		metaData.scriptClass->AddInternalCall("Internal_setTangents", (void*)&ScriptRendererMeshData::InternalSetTangents);
		metaData.scriptClass->AddInternalCall("Internal_getColors", (void*)&ScriptRendererMeshData::InternalGetColors);
		metaData.scriptClass->AddInternalCall("Internal_setColors", (void*)&ScriptRendererMeshData::InternalSetColors);
		metaData.scriptClass->AddInternalCall("Internal_getUV0", (void*)&ScriptRendererMeshData::InternalGetUV0);
		metaData.scriptClass->AddInternalCall("Internal_setUV0", (void*)&ScriptRendererMeshData::InternalSetUV0);
		metaData.scriptClass->AddInternalCall("Internal_getUV1", (void*)&ScriptRendererMeshData::InternalGetUV1);
		metaData.scriptClass->AddInternalCall("Internal_setUV1", (void*)&ScriptRendererMeshData::InternalSetUV1);
		metaData.scriptClass->AddInternalCall("Internal_getBoneWeights", (void*)&ScriptRendererMeshData::InternalGetBoneWeights);
		metaData.scriptClass->AddInternalCall("Internal_setBoneWeights", (void*)&ScriptRendererMeshData::InternalSetBoneWeights);
		metaData.scriptClass->AddInternalCall("Internal_getIndices", (void*)&ScriptRendererMeshData::InternalGetIndices);
		metaData.scriptClass->AddInternalCall("Internal_setIndices", (void*)&ScriptRendererMeshData::InternalSetIndices);
		metaData.scriptClass->AddInternalCall("Internal_getVertexCount", (void*)&ScriptRendererMeshData::InternalGetVertexCount);
		metaData.scriptClass->AddInternalCall("Internal_getIndexCount", (void*)&ScriptRendererMeshData::InternalGetIndexCount);

	}

	MonoObject* ScriptRendererMeshData::Create(const SPtr<RendererMeshData>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptRendererMeshData>()) ScriptRendererMeshData(managedInstance, value);
		return managedInstance;
	}
	void ScriptRendererMeshData::InternalCreate(MonoObject* managedInstance, uint32_t numVertices, uint32_t numIndices, VertexLayout layout, IndexType indexType)
	{
		SPtr<RendererMeshData> instance = MeshDataEx::Create(numVertices, numIndices, layout, indexType);
		new (bs_alloc<ScriptRendererMeshData>())ScriptRendererMeshData(managedInstance, instance);
	}

	MonoArray* ScriptRendererMeshData::InternalGetPositions(ScriptRendererMeshData* thisPtr)
	{
		Vector<Vector3> vec__output;
		vec__output = MeshDataEx::getPositions(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptVector3>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetPositions(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Vector3> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Vector3>(i);
			}
		}
		MeshDataEx::setPositions(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetNormals(ScriptRendererMeshData* thisPtr)
	{
		Vector<Vector3> vec__output;
		vec__output = MeshDataEx::getNormals(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptVector3>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetNormals(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Vector3> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Vector3>(i);
			}
		}
		MeshDataEx::setNormals(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetTangents(ScriptRendererMeshData* thisPtr)
	{
		Vector<Vector4> vec__output;
		vec__output = MeshDataEx::getTangents(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptVector4>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetTangents(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Vector4> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Vector4>(i);
			}
		}
		MeshDataEx::setTangents(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetColors(ScriptRendererMeshData* thisPtr)
	{
		Vector<Color> vec__output;
		vec__output = MeshDataEx::getColors(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptColor>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetColors(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Color> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Color>(i);
			}
		}
		MeshDataEx::setColors(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetUV0(ScriptRendererMeshData* thisPtr)
	{
		Vector<Vector2> vec__output;
		vec__output = MeshDataEx::getUV0(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptVector2>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetUV0(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Vector2> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Vector2>(i);
			}
		}
		MeshDataEx::setUV0(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetUV1(ScriptRendererMeshData* thisPtr)
	{
		Vector<Vector2> vec__output;
		vec__output = MeshDataEx::getUV1(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptVector2>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetUV1(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<Vector2> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Vector2>(i);
			}
		}
		MeshDataEx::setUV1(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetBoneWeights(ScriptRendererMeshData* thisPtr)
	{
		Vector<BoneWeight> vec__output;
		vec__output = MeshDataEx::getBoneWeights(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptBoneWeight>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetBoneWeights(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<BoneWeight> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<BoneWeight>(i);
			}
		}
		MeshDataEx::setBoneWeights(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptRendererMeshData::InternalGetIndices(ScriptRendererMeshData* thisPtr)
	{
		Vector<uint32_t> vec__output;
		vec__output = MeshDataEx::getIndices(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<uint32_t>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptRendererMeshData::InternalSetIndices(ScriptRendererMeshData* thisPtr, MonoArray* value)
	{
		Vector<uint32_t> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<uint32_t>(i);
			}
		}
		MeshDataEx::setIndices(thisPtr->GetInternal(), vecvalue);
	}

	int32_t ScriptRendererMeshData::InternalGetVertexCount(ScriptRendererMeshData* thisPtr)
	{
		int32_t tmp__output;
		tmp__output = MeshDataEx::getVertexCount(thisPtr->GetInternal());

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	int32_t ScriptRendererMeshData::InternalGetIndexCount(ScriptRendererMeshData* thisPtr)
	{
		int32_t tmp__output;
		tmp__output = MeshDataEx::getIndexCount(thisPtr->GetInternal());

		int32_t __output;
		__output = tmp__output;

		return __output;
	}
}
