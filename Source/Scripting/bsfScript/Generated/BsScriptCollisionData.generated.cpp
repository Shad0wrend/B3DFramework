//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCollisionData.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptGameObjectManager.h"
#include "../../../Foundation/bsfCore/Components/BsCCollider.h"
#include "BsScriptCCollider.generated.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsCommon.h"
#include "BsScriptContactPoint.generated.h"

namespace bs
{
	ScriptCollisionData::ScriptCollisionData(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptCollisionData::InitRuntimeData()
	{ }

	MonoObject*ScriptCollisionData::Box(const __CollisionDataInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__CollisionDataInterop ScriptCollisionData::Unbox(MonoObject* value)
	{
		return *(__CollisionDataInterop*)MonoUtil::Unbox(value);
	}

	CollisionData ScriptCollisionData::FromInterop(const __CollisionDataInterop& value)
	{
		CollisionData output;
		GameObjectHandle<CCollider> veccollider[2];
		if(value.Collider != nullptr)
		{
			ScriptArray arraycollider(value.Collider);
			for(int i = 0; i < (int)arraycollider.Size(); i++)
			{
				ScriptCColliderBase* scriptcollider;
				scriptcollider = (ScriptCColliderBase*)ScriptCCollider::ToNative(arraycollider.Get<MonoObject*>(i));
				if(scriptcollider != nullptr)
				{
					GameObjectHandle<CCollider> arrayElemPtrcollider = static_object_cast<CCollider>(scriptcollider->GetComponent());
					veccollider[i] = arrayElemPtrcollider;
				}
			}
		}
		auto tmpcollider = veccollider;
		for(int i = 0; i < 2; ++i)
			output.Collider[i] = tmpcollider[i];
		Vector<ContactPoint> veccontactPoints;
		if(value.ContactPoints != nullptr)
		{
			ScriptArray arraycontactPoints(value.ContactPoints);
			veccontactPoints.resize(arraycontactPoints.Size());
			for(int i = 0; i < (int)arraycontactPoints.Size(); i++)
			{
				veccontactPoints[i] = ScriptContactPoint::FromInterop(arraycontactPoints.Get<__ContactPointInterop>(i));
			}
		}
		output.ContactPoints = veccontactPoints;

		return output;
	}

	__CollisionDataInterop ScriptCollisionData::ToInterop(const CollisionData& value)
	{
		__CollisionDataInterop output;
		int arraySizecollider = 2;
		MonoArray* veccollider;
		ScriptArray arraycollider = ScriptArray::Create<ScriptCCollider>(arraySizecollider);
		for(int i = 0; i < arraySizecollider; i++)
		{
			ScriptComponentBase* scriptcollider = nullptr;
			if(value.Collider[i])
				scriptcollider = ScriptGameObjectManager::Instance().GetBuiltinScriptComponent(static_object_cast<Component>(value.Collider[i]));
				if(scriptcollider != nullptr)
				arraycollider.Set(i, scriptcollider->GetManagedInstance());
			else
				arraycollider.Set(i, nullptr);
		}
		veccollider = arraycollider.GetInternal();
		output.Collider = veccollider;
		int arraySizecontactPoints = (int)value.ContactPoints.size();
		MonoArray* veccontactPoints;
		ScriptArray arraycontactPoints = ScriptArray::Create<ScriptContactPoint>(arraySizecontactPoints);
		for(int i = 0; i < arraySizecontactPoints; i++)
		{
			arraycontactPoints.Set(i, ScriptContactPoint::ToInterop(value.ContactPoints[i]));
		}
		veccontactPoints = arraycontactPoints.GetInternal();
		output.ContactPoints = veccontactPoints;

		return output;
	}

}
