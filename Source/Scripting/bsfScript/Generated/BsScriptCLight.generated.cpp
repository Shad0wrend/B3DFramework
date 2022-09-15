//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCLight.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCLight.h"
#include "Wrappers/BsScriptColor.h"

namespace bs
{
	ScriptCLight::ScriptCLight(MonoObject* managedInstance, const GameObjectHandle<CLight>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCLight::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setType", (void*)&ScriptCLight::InternalSetType);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptCLight::InternalGetType);
		metaData.scriptClass->addInternalCall("Internal_setColor", (void*)&ScriptCLight::InternalSetColor);
		metaData.scriptClass->addInternalCall("Internal_getColor", (void*)&ScriptCLight::InternalGetColor);
		metaData.scriptClass->addInternalCall("Internal_setIntensity", (void*)&ScriptCLight::InternalSetIntensity);
		metaData.scriptClass->addInternalCall("Internal_getIntensity", (void*)&ScriptCLight::InternalGetIntensity);
		metaData.scriptClass->addInternalCall("Internal_setUseAutoAttenuation", (void*)&ScriptCLight::InternalSetUseAutoAttenuation);
		metaData.scriptClass->addInternalCall("Internal_getUseAutoAttenuation", (void*)&ScriptCLight::InternalGetUseAutoAttenuation);
		metaData.scriptClass->addInternalCall("Internal_setAttenuationRadius", (void*)&ScriptCLight::InternalSetAttenuationRadius);
		metaData.scriptClass->addInternalCall("Internal_getAttenuationRadius", (void*)&ScriptCLight::InternalGetAttenuationRadius);
		metaData.scriptClass->addInternalCall("Internal_setSourceRadius", (void*)&ScriptCLight::InternalSetSourceRadius);
		metaData.scriptClass->addInternalCall("Internal_getSourceRadius", (void*)&ScriptCLight::InternalGetSourceRadius);
		metaData.scriptClass->addInternalCall("Internal_setSpotAngle", (void*)&ScriptCLight::InternalSetSpotAngle);
		metaData.scriptClass->addInternalCall("Internal_getSpotAngle", (void*)&ScriptCLight::InternalGetSpotAngle);
		metaData.scriptClass->addInternalCall("Internal_setSpotFalloffAngle", (void*)&ScriptCLight::InternalSetSpotFalloffAngle);
		metaData.scriptClass->addInternalCall("Internal_getSpotFalloffAngle", (void*)&ScriptCLight::InternalGetSpotFalloffAngle);
		metaData.scriptClass->addInternalCall("Internal_setCastsShadow", (void*)&ScriptCLight::InternalSetCastsShadow);
		metaData.scriptClass->addInternalCall("Internal_getCastsShadow", (void*)&ScriptCLight::InternalGetCastsShadow);
		metaData.scriptClass->addInternalCall("Internal_setShadowBias", (void*)&ScriptCLight::InternalSetShadowBias);
		metaData.scriptClass->addInternalCall("Internal_getShadowBias", (void*)&ScriptCLight::InternalGetShadowBias);
		metaData.scriptClass->addInternalCall("Internal_getBounds", (void*)&ScriptCLight::InternalGetBounds);

	}

	void ScriptCLight::InternalSetType(ScriptCLight* thisPtr, LightType type)
	{
		thisPtr->GetHandle()->SetType(type);
	}

	LightType ScriptCLight::InternalGetType(ScriptCLight* thisPtr)
	{
		LightType tmp__output;
		tmp__output = thisPtr->GetHandle()->GetType();

		LightType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetColor(ScriptCLight* thisPtr, Color* color)
	{
		thisPtr->GetHandle()->SetColor(*color);
	}

	void ScriptCLight::InternalGetColor(ScriptCLight* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetHandle()->GetColor();

		*__output = tmp__output;
	}

	void ScriptCLight::InternalSetIntensity(ScriptCLight* thisPtr, float intensity)
	{
		thisPtr->GetHandle()->SetIntensity(intensity);
	}

	float ScriptCLight::InternalGetIntensity(ScriptCLight* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetIntensity();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetUseAutoAttenuation(ScriptCLight* thisPtr, bool enabled)
	{
		thisPtr->GetHandle()->SetUseAutoAttenuation(enabled);
	}

	bool ScriptCLight::InternalGetUseAutoAttenuation(ScriptCLight* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetUseAutoAttenuation();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetAttenuationRadius(ScriptCLight* thisPtr, float radius)
	{
		thisPtr->GetHandle()->SetAttenuationRadius(radius);
	}

	float ScriptCLight::InternalGetAttenuationRadius(ScriptCLight* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAttenuationRadius();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetSourceRadius(ScriptCLight* thisPtr, float radius)
	{
		thisPtr->GetHandle()->SetSourceRadius(radius);
	}

	float ScriptCLight::InternalGetSourceRadius(ScriptCLight* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSourceRadius();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetSpotAngle(ScriptCLight* thisPtr, Degree* spotAngle)
	{
		thisPtr->GetHandle()->SetSpotAngle(*spotAngle);
	}

	void ScriptCLight::InternalGetSpotAngle(ScriptCLight* thisPtr, Degree* __output)
	{
		Degree tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSpotAngle();

		*__output = tmp__output;
	}

	void ScriptCLight::InternalSetSpotFalloffAngle(ScriptCLight* thisPtr, Degree* spotAngle)
	{
		thisPtr->GetHandle()->SetSpotFalloffAngle(*spotAngle);
	}

	void ScriptCLight::InternalGetSpotFalloffAngle(ScriptCLight* thisPtr, Degree* __output)
	{
		Degree tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSpotFalloffAngle();

		*__output = tmp__output;
	}

	void ScriptCLight::InternalSetCastsShadow(ScriptCLight* thisPtr, bool castsShadow)
	{
		thisPtr->GetHandle()->SetCastsShadow(castsShadow);
	}

	bool ScriptCLight::InternalGetCastsShadow(ScriptCLight* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCastsShadow();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalSetShadowBias(ScriptCLight* thisPtr, float bias)
	{
		thisPtr->GetHandle()->SetShadowBias(bias);
	}

	float ScriptCLight::InternalGetShadowBias(ScriptCLight* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetShadowBias();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCLight::InternalGetBounds(ScriptCLight* thisPtr, Sphere* __output)
	{
		Sphere tmp__output;
		tmp__output = thisPtr->GetHandle()->GetBounds();

		*__output = tmp__output;
	}
}
