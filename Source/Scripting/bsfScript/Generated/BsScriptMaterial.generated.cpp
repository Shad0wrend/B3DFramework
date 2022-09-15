//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptMaterial.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Material/BsMaterial.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptColor.h"
#include "../../../Foundation/bsfCore/Material/BsMaterial.h"
#include "../../../Foundation/bsfCore/Material/BsShader.h"
#include "BsScriptShaderVariation.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptColorGradientHDR.generated.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"
#include "../Extensions/BsMaterialEx.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteTexture.h"

namespace bs
{
	ScriptMaterial::ScriptMaterial(MonoObject* managedInstance, const ResourceHandle<Material>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptMaterial::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptMaterial::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_setShader", (void*)&ScriptMaterial::InternalSetShader);
		metaData.scriptClass->AddInternalCall("Internal_setVariation", (void*)&ScriptMaterial::InternalSetVariation);
		metaData.scriptClass->addInternalCall("Internal_clone", (void*)&ScriptMaterial::InternalClone);
		metaData.scriptClass->addInternalCall("Internal_getShader", (void*)&ScriptMaterial::InternalGetShader);
		metaData.scriptClass->addInternalCall("Internal_getVariation", (void*)&ScriptMaterial::InternalGetVariation);
		metaData.scriptClass->addInternalCall("Internal_setFloat", (void*)&ScriptMaterial::InternalSetFloat);
		metaData.scriptClass->addInternalCall("Internal_setFloatCurve", (void*)&ScriptMaterial::InternalSetFloatCurve);
		metaData.scriptClass->addInternalCall("Internal_setColor", (void*)&ScriptMaterial::InternalSetColor);
		metaData.scriptClass->addInternalCall("Internal_setColorGradient", (void*)&ScriptMaterial::InternalSetColorGradient);
		metaData.scriptClass->addInternalCall("Internal_setVec2", (void*)&ScriptMaterial::InternalSetVec2);
		metaData.scriptClass->addInternalCall("Internal_setVec3", (void*)&ScriptMaterial::InternalSetVec3);
		metaData.scriptClass->addInternalCall("Internal_setVec4", (void*)&ScriptMaterial::InternalSetVec4);
		metaData.scriptClass->addInternalCall("Internal_setMat3", (void*)&ScriptMaterial::InternalSetMat3);
		metaData.scriptClass->addInternalCall("Internal_setMat4", (void*)&ScriptMaterial::InternalSetMat4);
		metaData.scriptClass->addInternalCall("Internal_getFloat", (void*)&ScriptMaterial::InternalGetFloat);
		metaData.scriptClass->addInternalCall("Internal_getFloatCurve", (void*)&ScriptMaterial::InternalGetFloatCurve);
		metaData.scriptClass->addInternalCall("Internal_getColor", (void*)&ScriptMaterial::InternalGetColor);
		metaData.scriptClass->addInternalCall("Internal_getColorGradient", (void*)&ScriptMaterial::InternalGetColorGradient);
		metaData.scriptClass->addInternalCall("Internal_getVec2", (void*)&ScriptMaterial::InternalGetVec2);
		metaData.scriptClass->addInternalCall("Internal_getVec3", (void*)&ScriptMaterial::InternalGetVec3);
		metaData.scriptClass->addInternalCall("Internal_getVec4", (void*)&ScriptMaterial::InternalGetVec4);
		metaData.scriptClass->addInternalCall("Internal_getMat3", (void*)&ScriptMaterial::InternalGetMat3);
		metaData.scriptClass->addInternalCall("Internal_getMat4", (void*)&ScriptMaterial::InternalGetMat4);
		metaData.scriptClass->addInternalCall("Internal_isAnimated", (void*)&ScriptMaterial::InternalIsAnimated);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptMaterial::InternalCreate);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptMaterial::InternalCreate0);
		metaData.scriptClass->addInternalCall("Internal_setTexture", (void*)&ScriptMaterial::InternalSetTexture);
		metaData.scriptClass->addInternalCall("Internal_getTexture", (void*)&ScriptMaterial::InternalGetTexture);
		metaData.scriptClass->addInternalCall("Internal_setSpriteTexture", (void*)&ScriptMaterial::InternalSetSpriteTexture);
		metaData.scriptClass->addInternalCall("Internal_getSpriteTexture", (void*)&ScriptMaterial::InternalGetSpriteTexture);

	}

	 MonoObject*ScriptMaterial::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->createInstance("bool", ctorParams);
	}
	MonoObject* ScriptMaterial::InternalGetRef(ScriptMaterial* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	void ScriptMaterial::InternalSetShader(ScriptMaterial* thisPtr, MonoObject* shader)
	{
		ResourceHandle<Shader> tmpshader;
		ScriptRRefBase* scriptshader;
		scriptshader = ScriptRRefBase::toNative(shader);
		if(scriptshader != nullptr)
			tmpshader = static_resource_cast<Shader>(scriptshader->GetHandle());
		thisPtr->GetHandle()->SetShader(tmpshader);
	}

	void ScriptMaterial::InternalSetVariation(ScriptMaterial* thisPtr, MonoObject* variation)
	{
		SPtr<ShaderVariation> tmpvariation;
		ScriptShaderVariation* scriptvariation;
		scriptvariation = ScriptShaderVariation::toNative(variation);
		if(scriptvariation != nullptr)
			tmpvariation = scriptvariation->GetInternal();
		thisPtr->GetHandle()->SetVariation(*tmpvariation);
	}

	MonoObject* ScriptMaterial::InternalClone(ScriptMaterial* thisPtr)
	{
		ResourceHandle<Material> tmp__output;
		tmp__output = thisPtr->GetHandle()->clone();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptMaterial::InternalGetShader(ScriptMaterial* thisPtr)
	{
		ResourceHandle<Shader> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetShader();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptMaterial::InternalGetVariation(ScriptMaterial* thisPtr)
	{
		SPtr<ShaderVariation> tmp__output = bs_shared_ptr_new<ShaderVariation>();
		*tmp__output = thisPtr->GetHandle()->GetVariation();

		MonoObject* __output;
		__output = ScriptShaderVariation::Create(tmp__output);

		return __output;
	}

	void ScriptMaterial::InternalSetFloat(ScriptMaterial* thisPtr, MonoString* name, float value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetFloat(tmpname, value, arrayIdx);
	}

	void ScriptMaterial::InternalSetFloatCurve(ScriptMaterial* thisPtr, MonoString* name, MonoObject* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<TAnimationCurve<float>> tmpvalue;
		ScriptTAnimationCurvefloat* scriptvalue;
		scriptvalue = ScriptTAnimationCurvefloat::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetHandle()->SetFloatCurve(tmpname, *tmpvalue, arrayIdx);
	}

	void ScriptMaterial::InternalSetColor(ScriptMaterial* thisPtr, MonoString* name, Color* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetColor(tmpname, *value, arrayIdx);
	}

	void ScriptMaterial::InternalSetColorGradient(ScriptMaterial* thisPtr, MonoString* name, MonoObject* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<ColorGradientHDR> tmpvalue;
		ScriptColorGradientHDR* scriptvalue;
		scriptvalue = ScriptColorGradientHDR::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetHandle()->SetColorGradient(tmpname, *tmpvalue, arrayIdx);
	}

	void ScriptMaterial::InternalSetVec2(ScriptMaterial* thisPtr, MonoString* name, Vector2* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetVec2(tmpname, *value, arrayIdx);
	}

	void ScriptMaterial::InternalSetVec3(ScriptMaterial* thisPtr, MonoString* name, Vector3* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetVec3(tmpname, *value, arrayIdx);
	}

	void ScriptMaterial::InternalSetVec4(ScriptMaterial* thisPtr, MonoString* name, Vector4* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetVec4(tmpname, *value, arrayIdx);
	}

	void ScriptMaterial::InternalSetMat3(ScriptMaterial* thisPtr, MonoString* name, Matrix3* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetMat3(tmpname, *value, arrayIdx);
	}

	void ScriptMaterial::InternalSetMat4(ScriptMaterial* thisPtr, MonoString* name, Matrix4* value, uint32_t arrayIdx)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetHandle()->SetMat4(tmpname, *value, arrayIdx);
	}

	float ScriptMaterial::InternalGetFloat(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx)
	{
		float tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = thisPtr->GetHandle()->GetFloat(tmpname, arrayIdx);

		float __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptMaterial::InternalGetFloatCurve(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		*tmp__output = thisPtr->GetHandle()->GetFloatCurve(tmpname, arrayIdx);

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::Create(tmp__output);

		return __output;
	}

	void ScriptMaterial::InternalGetColor(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Color* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Color tmp__output;
		tmp__output = thisPtr->GetHandle()->GetColor(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	MonoObject* ScriptMaterial::InternalGetColorGradient(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx)
	{
		SPtr<ColorGradientHDR> tmp__output = bs_shared_ptr_new<ColorGradientHDR>();
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		*tmp__output = thisPtr->GetHandle()->GetColorGradient(tmpname, arrayIdx);

		MonoObject* __output;
		__output = ScriptColorGradientHDR::Create(tmp__output);

		return __output;
	}

	void ScriptMaterial::InternalGetVec2(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Vector2* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVec2(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	void ScriptMaterial::InternalGetVec3(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Vector3* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVec3(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	void ScriptMaterial::InternalGetVec4(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Vector4* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Vector4 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVec4(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	void ScriptMaterial::InternalGetMat3(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Matrix3* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Matrix3 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMat3(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	void ScriptMaterial::InternalGetMat4(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx, Matrix4* __output)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		Matrix4 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMat4(tmpname, arrayIdx);

		*__output = tmp__output;
	}

	bool ScriptMaterial::InternalIsAnimated(ScriptMaterial* thisPtr, MonoString* name, uint32_t arrayIdx)
	{
		bool tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = thisPtr->GetHandle()->isAnimated(tmpname, arrayIdx);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMaterial::InternalCreate(MonoObject* managedInstance)
	{
		ResourceHandle<Material> instance = Material::Create();
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	void ScriptMaterial::InternalCreate0(MonoObject* managedInstance, MonoObject* shader)
	{
		ResourceHandle<Shader> tmpshader;
		ScriptRRefBase* scriptshader;
		scriptshader = ScriptRRefBase::toNative(shader);
		if(scriptshader != nullptr)
			tmpshader = static_resource_cast<Shader>(scriptshader->GetHandle());
		ResourceHandle<Material> instance = Material::Create(tmpshader);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	void ScriptMaterial::InternalSetTexture(ScriptMaterial* thisPtr, MonoString* name, MonoObject* value, uint32_t mipLevel, uint32_t numMipLevels, uint32_t arraySlice, uint32_t numArraySlices)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		ResourceHandle<Texture> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Texture>(scriptvalue->GetHandle());
		MaterialEx::setTexture(thisPtr->GetHandle(), tmpname, tmpvalue, mipLevel, numMipLevels, arraySlice, numArraySlices);
	}

	MonoObject* ScriptMaterial::InternalGetTexture(ScriptMaterial* thisPtr, MonoString* name)
	{
		ResourceHandle<Texture> tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = MaterialEx::getTexture(thisPtr->GetHandle(), tmpname);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptMaterial::InternalSetSpriteTexture(ScriptMaterial* thisPtr, MonoString* name, MonoObject* value)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		ResourceHandle<SpriteTexture> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<SpriteTexture>(scriptvalue->GetHandle());
		MaterialEx::setSpriteTexture(thisPtr->GetHandle(), tmpname, tmpvalue);
	}

	MonoObject* ScriptMaterial::InternalGetSpriteTexture(ScriptMaterial* thisPtr, MonoString* name)
	{
		ResourceHandle<SpriteTexture> tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = MaterialEx::getSpriteTexture(thisPtr->GetHandle(), tmpname);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}
}
