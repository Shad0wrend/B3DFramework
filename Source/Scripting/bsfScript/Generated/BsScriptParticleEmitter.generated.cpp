//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleEmitter.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Reflection/BsRTTIType.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterHemisphereShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterCircleShape.generated.h"
#include "BsScriptParticleEmitterShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterStaticMeshShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterSkinnedMeshShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterBoxShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterConeShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterSphereShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterLineShape.generated.h"
#include "../../../Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptParticleEmitterRectShape.generated.h"
#include "BsScriptTDistribution.generated.h"
#include "BsScriptParticleBurst.generated.h"
#include "BsScriptTDistribution.generated.h"
#include "BsScriptTColorDistribution.generated.h"
#include "BsScriptParticleEmitter.generated.h"

namespace bs
{
	ScriptParticleEmitter::ScriptParticleEmitter(MonoObject* managedInstance, const SPtr<ParticleEmitter>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptParticleEmitter::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setShape", (void*)&ScriptParticleEmitter::InternalSetShape);
		metaData.scriptClass->AddInternalCall("Internal_getShape", (void*)&ScriptParticleEmitter::InternalGetShape);
		metaData.scriptClass->AddInternalCall("Internal_setEmissionRate", (void*)&ScriptParticleEmitter::InternalSetEmissionRate);
		metaData.scriptClass->AddInternalCall("Internal_getEmissionRate", (void*)&ScriptParticleEmitter::InternalGetEmissionRate);
		metaData.scriptClass->AddInternalCall("Internal_setEmissionBursts", (void*)&ScriptParticleEmitter::InternalSetEmissionBursts);
		metaData.scriptClass->AddInternalCall("Internal_getEmissionBursts", (void*)&ScriptParticleEmitter::InternalGetEmissionBursts);
		metaData.scriptClass->AddInternalCall("Internal_setInitialLifetime", (void*)&ScriptParticleEmitter::InternalSetInitialLifetime);
		metaData.scriptClass->AddInternalCall("Internal_getInitialLifetime", (void*)&ScriptParticleEmitter::InternalGetInitialLifetime);
		metaData.scriptClass->AddInternalCall("Internal_setInitialSpeed", (void*)&ScriptParticleEmitter::InternalSetInitialSpeed);
		metaData.scriptClass->AddInternalCall("Internal_getInitialSpeed", (void*)&ScriptParticleEmitter::InternalGetInitialSpeed);
		metaData.scriptClass->AddInternalCall("Internal_setInitialSize", (void*)&ScriptParticleEmitter::InternalSetInitialSize);
		metaData.scriptClass->AddInternalCall("Internal_getInitialSize", (void*)&ScriptParticleEmitter::InternalGetInitialSize);
		metaData.scriptClass->AddInternalCall("Internal_setInitialSize3D", (void*)&ScriptParticleEmitter::InternalSetInitialSize3D);
		metaData.scriptClass->AddInternalCall("Internal_getInitialSize3D", (void*)&ScriptParticleEmitter::InternalGetInitialSize3D);
		metaData.scriptClass->AddInternalCall("Internal_setUse3DSize", (void*)&ScriptParticleEmitter::InternalSetUse3DSize);
		metaData.scriptClass->AddInternalCall("Internal_getUse3DSize", (void*)&ScriptParticleEmitter::InternalGetUse3DSize);
		metaData.scriptClass->AddInternalCall("Internal_setInitialRotation", (void*)&ScriptParticleEmitter::InternalSetInitialRotation);
		metaData.scriptClass->AddInternalCall("Internal_getInitialRotation", (void*)&ScriptParticleEmitter::InternalGetInitialRotation);
		metaData.scriptClass->AddInternalCall("Internal_setInitialRotation3D", (void*)&ScriptParticleEmitter::InternalSetInitialRotation3D);
		metaData.scriptClass->AddInternalCall("Internal_getInitialRotation3D", (void*)&ScriptParticleEmitter::InternalGetInitialRotation3D);
		metaData.scriptClass->AddInternalCall("Internal_setUse3DRotation", (void*)&ScriptParticleEmitter::InternalSetUse3DRotation);
		metaData.scriptClass->AddInternalCall("Internal_getUse3DRotation", (void*)&ScriptParticleEmitter::InternalGetUse3DRotation);
		metaData.scriptClass->AddInternalCall("Internal_setInitialColor", (void*)&ScriptParticleEmitter::InternalSetInitialColor);
		metaData.scriptClass->AddInternalCall("Internal_getInitialColor", (void*)&ScriptParticleEmitter::InternalGetInitialColor);
		metaData.scriptClass->AddInternalCall("Internal_setRandomOffset", (void*)&ScriptParticleEmitter::InternalSetRandomOffset);
		metaData.scriptClass->AddInternalCall("Internal_getRandomOffset", (void*)&ScriptParticleEmitter::InternalGetRandomOffset);
		metaData.scriptClass->AddInternalCall("Internal_setFlipU", (void*)&ScriptParticleEmitter::InternalSetFlipU);
		metaData.scriptClass->AddInternalCall("Internal_getFlipU", (void*)&ScriptParticleEmitter::InternalGetFlipU);
		metaData.scriptClass->AddInternalCall("Internal_setFlipV", (void*)&ScriptParticleEmitter::InternalSetFlipV);
		metaData.scriptClass->AddInternalCall("Internal_getFlipV", (void*)&ScriptParticleEmitter::InternalGetFlipV);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptParticleEmitter::InternalCreate);

	}

	MonoObject* ScriptParticleEmitter::Create(const SPtr<ParticleEmitter>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleEmitter>()) ScriptParticleEmitter(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleEmitter::InternalSetShape(ScriptParticleEmitter* thisPtr, MonoObject* shape)
	{
		SPtr<ParticleEmitterShape> tmpshape;
		ScriptParticleEmitterShapeBase* scriptshape;
		scriptshape = (ScriptParticleEmitterShapeBase*)ScriptParticleEmitterShape::toNative(shape);
		if(scriptshape != nullptr)
			tmpshape = scriptshape->GetInternal();
		thisPtr->GetInternal()->SetShape(tmpshape);
	}

	MonoObject* ScriptParticleEmitter::InternalGetShape(ScriptParticleEmitter* thisPtr)
	{
		SPtr<ParticleEmitterShape> tmp__output;
		tmp__output = thisPtr->GetInternal()->GetShape();

		MonoObject* __output;
		if(tmp__output)
		{
			if(rtti_is_of_type<ParticleEmitterStaticMeshShape>(tmp__output))
				__output = ScriptParticleEmitterStaticMeshShape::Create(std::static_pointer_cast<ParticleEmitterStaticMeshShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterCircleShape>(tmp__output))
				__output = ScriptParticleEmitterCircleShape::Create(std::static_pointer_cast<ParticleEmitterCircleShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterConeShape>(tmp__output))
				__output = ScriptParticleEmitterConeShape::Create(std::static_pointer_cast<ParticleEmitterConeShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterSphereShape>(tmp__output))
				__output = ScriptParticleEmitterSphereShape::Create(std::static_pointer_cast<ParticleEmitterSphereShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterHemisphereShape>(tmp__output))
				__output = ScriptParticleEmitterHemisphereShape::Create(std::static_pointer_cast<ParticleEmitterHemisphereShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterBoxShape>(tmp__output))
				__output = ScriptParticleEmitterBoxShape::Create(std::static_pointer_cast<ParticleEmitterBoxShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterLineShape>(tmp__output))
				__output = ScriptParticleEmitterLineShape::Create(std::static_pointer_cast<ParticleEmitterLineShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterRectShape>(tmp__output))
				__output = ScriptParticleEmitterRectShape::Create(std::static_pointer_cast<ParticleEmitterRectShape>(tmp__output));
			else if(rtti_is_of_type<ParticleEmitterSkinnedMeshShape>(tmp__output))
				__output = ScriptParticleEmitterSkinnedMeshShape::Create(std::static_pointer_cast<ParticleEmitterSkinnedMeshShape>(tmp__output));
			else
				__output = ScriptParticleEmitterShape::Create(tmp__output);
		}
		else
			__output = ScriptParticleEmitterShape::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetEmissionRate(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetEmissionRate(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetEmissionRate(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = thisPtr->GetInternal()->GetEmissionRate();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetEmissionBursts(ScriptParticleEmitter* thisPtr, MonoArray* bursts)
	{
		Vector<ParticleBurst> vecbursts;
		if(bursts != nullptr)
		{
			ScriptArray arraybursts(bursts);
			vecbursts.resize(arraybursts.size());
			for(int i = 0; i < (int)arraybursts.size(); i++)
			{
				vecbursts[i] = ScriptParticleBurst::fromInterop(arraybursts.get<__ParticleBurstInterop>(i));
			}
		}
		thisPtr->GetInternal()->SetEmissionBursts(vecbursts);
	}

	MonoArray* ScriptParticleEmitter::InternalGetEmissionBursts(ScriptParticleEmitter* thisPtr)
	{
		Vector<ParticleBurst> vec__output;
		vec__output = thisPtr->GetInternal()->GetEmissionBursts();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptParticleBurst>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptParticleBurst::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialLifetime(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialLifetime(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialLifetime(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialLifetime();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialSpeed(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialSpeed(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialSpeed(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialSpeed();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialSize(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialSize(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialSize(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialSize();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialSize3D(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector3>> tmpvalue;
		ScriptTDistributionVector3* scriptvalue;
		scriptvalue = ScriptTDistributionVector3::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialSize3D(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialSize3D(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<Vector3>> tmp__output = bs_shared_ptr_new<TDistribution<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialSize3D();

		MonoObject* __output;
		__output = ScriptTDistributionVector3::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetUse3DSize(ScriptParticleEmitter* thisPtr, bool value)
	{
		thisPtr->GetInternal()->SetUse3DSize(value);
	}

	bool ScriptParticleEmitter::InternalGetUse3DSize(ScriptParticleEmitter* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->GetUse3DSize();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialRotation(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<float>> tmpvalue;
		ScriptTDistributionfloat* scriptvalue;
		scriptvalue = ScriptTDistributionfloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialRotation(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialRotation(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<float>> tmp__output = bs_shared_ptr_new<TDistribution<float>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialRotation();

		MonoObject* __output;
		__output = ScriptTDistributionfloat::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialRotation3D(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector3>> tmpvalue;
		ScriptTDistributionVector3* scriptvalue;
		scriptvalue = ScriptTDistributionVector3::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialRotation3D(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialRotation3D(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TDistribution<Vector3>> tmp__output = bs_shared_ptr_new<TDistribution<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialRotation3D();

		MonoObject* __output;
		__output = ScriptTDistributionVector3::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetUse3DRotation(ScriptParticleEmitter* thisPtr, bool value)
	{
		thisPtr->GetInternal()->SetUse3DRotation(value);
	}

	bool ScriptParticleEmitter::InternalGetUse3DRotation(ScriptParticleEmitter* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->GetUse3DRotation();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleEmitter::InternalSetInitialColor(ScriptParticleEmitter* thisPtr, MonoObject* value)
	{
		SPtr<TColorDistribution<ColorGradient>> tmpvalue;
		ScriptTColorDistributionColorGradient* scriptvalue;
		scriptvalue = ScriptTColorDistributionColorGradient::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SetInitialColor(*tmpvalue);
	}

	MonoObject* ScriptParticleEmitter::InternalGetInitialColor(ScriptParticleEmitter* thisPtr)
	{
		SPtr<TColorDistribution<ColorGradient>> tmp__output = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		*tmp__output = thisPtr->GetInternal()->GetInitialColor();

		MonoObject* __output;
		__output = ScriptTColorDistributionColorGradient::Create(tmp__output);

		return __output;
	}

	void ScriptParticleEmitter::InternalSetRandomOffset(ScriptParticleEmitter* thisPtr, float value)
	{
		thisPtr->GetInternal()->SetRandomOffset(value);
	}

	float ScriptParticleEmitter::InternalGetRandomOffset(ScriptParticleEmitter* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetRandomOffset();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleEmitter::InternalSetFlipU(ScriptParticleEmitter* thisPtr, float value)
	{
		thisPtr->GetInternal()->SetFlipU(value);
	}

	float ScriptParticleEmitter::InternalGetFlipU(ScriptParticleEmitter* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetFlipU();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleEmitter::InternalSetFlipV(ScriptParticleEmitter* thisPtr, float value)
	{
		thisPtr->GetInternal()->SetFlipV(value);
	}

	float ScriptParticleEmitter::InternalGetFlipV(ScriptParticleEmitter* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetFlipV();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleEmitter::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<ParticleEmitter> instance = ParticleEmitter::Create();
		new (bs_alloc<ScriptParticleEmitter>())ScriptParticleEmitter(managedInstance, instance);
	}
}
