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

	void ScriptCollisionData::initRuntimeData()
	{ }

	MonoObject*ScriptCollisionData::Box(const __CollisionDataInterop& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__CollisionDataInterop ScriptCollisionData::Unbox(MonoObject* value)
	{
		return *(__CollisionDataInterop*)MonoUtil::Unbox(value);
	}

	CollisionData ScriptCollisionData::FromInterop(const __CollisionDataInterop& value)
	{
		CollisionData output;
		GameObjectHandle<CCollider> veccollider[2];
		if(value.collider != nullptr)
		{
			ScriptArray arraycollider(value.collider);
			for(int i = 0; i < (int)arraycollider.Size(); i++)
			{
				ScriptCColliderBase* scriptcollider;
				scriptcollider = (ScriptCColliderBase*)ScriptCCollider::ToNative(arraycollider.get<MonoObject*>(i));
				if(scriptcollider != nullptr)
				{
					GameObjectHandle<CCollider> arrayElemPtrcollider = static_object_cast<CCollider>(scriptcollider->GetComponent());
					veccollider[i] = arrayElemPtrcollider;
				}
			}
		}
		auto tmpcollider = veccollider;
		for(int i = 0; i < 2; ++i)
			output.collider[i] = tmpcollider[i];
		Vector<ContactPoint> veccontactPoints;
		if(value.contactPoints != nullptr)
		{
			ScriptArray arraycontactPoints(value.contactPoints);
			veccontactPoints.resize(arraycontactPoints.Size());
			for(int i = 0; i < (int)arraycontactPoints.Size(); i++)
			{
				veccontactPoints[i] = ScriptContactPoint::FromInterop(arraycontactPoints.get<__ContactPointInterop>(i));
			}
		}
		output.contactPoints = veccontactPoints;

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
			if(value.collider[i])
				scriptcollider = ScriptGameObjectManager::Instance().getBuiltinScriptComponent(static_object_cast<Component>(value.collider[i]));
				if(scriptcollider != nullptr)
				arraycollider.set(i, scriptcollider->getManagedInstance());
			else
				arraycollider.set(i, nullptr);
		}
		veccollider = arraycollider.getInternal();
		output.collider = veccollider;
		int arraySizecontactPoints = (int)value.contactPoints.size();
		MonoArray* veccontactPoints;
		ScriptArray arraycontactPoints = ScriptArray::create<ScriptContactPoint>(arraySizecontactPoints);
		for(int i = 0; i < arraySizecontactPoints; i++)
		{
			arraycontactPoints.set(i, ScriptContactPoint::toInterop(value.contactPoints[i]));
		}
		veccontactPoints = arraycontactPoints.getInternal();
		output.contactPoints = veccontactPoints;

		return output;
	}

}
