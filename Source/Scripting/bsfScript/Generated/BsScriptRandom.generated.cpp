//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptRandom.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfUtility/Math/BsRandom.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptRandom::ScriptRandom(MonoObject* managedInstance, const SPtr<Random>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptRandom::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Random", (void*)&ScriptRandom::InternalRandom);
		metaData.scriptClass->AddInternalCall("Internal_setSeed", (void*)&ScriptRandom::InternalSetSeed);
		metaData.scriptClass->AddInternalCall("Internal_get", (void*)&ScriptRandom::InternalGet);
		metaData.scriptClass->AddInternalCall("Internal_getRange", (void*)&ScriptRandom::InternalGetRange);
		metaData.scriptClass->AddInternalCall("Internal_getUNorm", (void*)&ScriptRandom::InternalGetUNorm);
		metaData.scriptClass->AddInternalCall("Internal_getSNorm", (void*)&ScriptRandom::InternalGetSNorm);
		metaData.scriptClass->AddInternalCall("Internal_getUnitVector", (void*)&ScriptRandom::InternalGetUnitVector);
		metaData.scriptClass->AddInternalCall("Internal_getUnitVector2D", (void*)&ScriptRandom::InternalGetUnitVector2D);
		metaData.scriptClass->AddInternalCall("Internal_getPointInSphere", (void*)&ScriptRandom::InternalGetPointInSphere);
		metaData.scriptClass->AddInternalCall("Internal_getPointInSphereShell", (void*)&ScriptRandom::InternalGetPointInSphereShell);
		metaData.scriptClass->AddInternalCall("Internal_getPointInCircle", (void*)&ScriptRandom::InternalGetPointInCircle);
		metaData.scriptClass->AddInternalCall("Internal_getPointInCircleShell", (void*)&ScriptRandom::InternalGetPointInCircleShell);
		metaData.scriptClass->AddInternalCall("Internal_getPointInArc", (void*)&ScriptRandom::InternalGetPointInArc);
		metaData.scriptClass->AddInternalCall("Internal_getPointInArcShell", (void*)&ScriptRandom::InternalGetPointInArcShell);
		metaData.scriptClass->AddInternalCall("Internal_getBarycentric", (void*)&ScriptRandom::InternalGetBarycentric);

	}

	MonoObject* ScriptRandom::Create(const SPtr<Random>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptRandom>()) ScriptRandom(managedInstance, value);
		return managedInstance;
	}
	void ScriptRandom::InternalRandom(MonoObject* managedInstance, uint32_t seed)
	{
		SPtr<Random> instance = bs_shared_ptr_new<Random>(seed);
		new (bs_alloc<ScriptRandom>())ScriptRandom(managedInstance, instance);
	}

	void ScriptRandom::InternalSetSeed(ScriptRandom* thisPtr, uint32_t seed)
	{
		thisPtr->GetInternal()->SetSeed(seed);
	}

	uint32_t ScriptRandom::InternalGet(ScriptRandom* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->get();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	int32_t ScriptRandom::InternalGetRange(ScriptRandom* thisPtr, int32_t min, int32_t max)
	{
		int32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->GetRange(min, max);

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptRandom::InternalGetUNorm(ScriptRandom* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetUNorm();

		float __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptRandom::InternalGetSNorm(ScriptRandom* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetSNorm();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRandom::InternalGetUnitVector(ScriptRandom* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetUnitVector();

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetUnitVector2D(ScriptRandom* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetUnitVector2D();

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInSphere(ScriptRandom* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInSphere();

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInSphereShell(ScriptRandom* thisPtr, float thickness, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInSphereShell(thickness);

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInCircle(ScriptRandom* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInCircle();

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInCircleShell(ScriptRandom* thisPtr, float thickness, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInCircleShell(thickness);

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInArc(ScriptRandom* thisPtr, Degree* angle, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInArc(*angle);

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetPointInArcShell(ScriptRandom* thisPtr, Degree* angle, float thickness, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetPointInArcShell(*angle, thickness);

		*__output = tmp__output;
	}

	void ScriptRandom::InternalGetBarycentric(ScriptRandom* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetBarycentric();

		*__output = tmp__output;
	}
}
