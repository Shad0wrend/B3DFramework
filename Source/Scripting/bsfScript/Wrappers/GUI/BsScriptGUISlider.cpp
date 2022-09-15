//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUISlider.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUISlider.h"
#include "GUI/BsGUIOptions.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

#include "Generated/BsScriptGUIElementStyle.generated.h"
#include "Generated/BsScriptHString.generated.h"
#include "Generated/BsScriptGUIContent.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUISliderH::OnChangedThunkDef ScriptGUISliderH::onChangedThunk;

	ScriptGUISliderH::ScriptGUISliderH(MonoObject* instance, GUISliderHorz* slider)
		:TScriptGUIElement(instance, slider)
	{

	}

	void ScriptGUISliderH::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUISliderH::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_SetPercent", (void*)&ScriptGUISliderH::InternalSetPercent);
		metaData.scriptClass->AddInternalCall("Internal_GetPercent", (void*)&ScriptGUISliderH::InternalGetPercent);
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUISliderH::InternalSetTint);
		metaData.scriptClass->AddInternalCall("Internal_GetValue", (void*)&ScriptGUISliderH::InternalGetValue);
		metaData.scriptClass->AddInternalCall("Internal_SetValue", (void*)&ScriptGUISliderH::InternalSetValue);
		metaData.scriptClass->AddInternalCall("Internal_SetRange", (void*)&ScriptGUISliderH::InternalSetRange);
		metaData.scriptClass->addInternalCall("Internal_GetRangeMaximum", (void*)&ScriptGUISliderH::InternalGetRangeMaximum);
		metaData.scriptClass->addInternalCall("Internal_GetRangeMinimum", (void*)&ScriptGUISliderH::InternalGetRangeMinimum);
		metaData.scriptClass->addInternalCall("Internal_SetStep", (void*)&ScriptGUISliderH::InternalSetStep);
		metaData.scriptClass->addInternalCall("Internal_GetStep", (void*)&ScriptGUISliderH::InternalGetStep);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->GetMethod("DoOnChanged", 1)->GetThunk();
	}

	void ScriptGUISliderH::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUISliderHorz* guiSlider = GUISliderHorz::Create(options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUISliderH>()) ScriptGUISliderH(instance, guiSlider);
		guiSlider->onChanged.Connect(std::bind(&ScriptGUISliderH::onChanged, nativeInstance, _1));
	}

	void ScriptGUISliderH::InternalSetPercent(ScriptGUISliderH* nativeInstance, float percent)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		slider->SetPercent(percent);
	}

	float ScriptGUISliderH::InternalGetPercent(ScriptGUISliderH* nativeInstance)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->GetPercent();
	}

	float ScriptGUISliderH::InternalGetValue(ScriptGUISliderH* nativeInstance)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->GetValue();
	}

	void ScriptGUISliderH::InternalSetValue(ScriptGUISliderH* nativeInstance, float percent)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->SetValue(percent);
	}

	void ScriptGUISliderH::InternalSetRange(ScriptGUISliderH* nativeInstance, float min, float max)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->SetRange(min, max);
	}

	float ScriptGUISliderH::InternalGetRangeMaximum(ScriptGUISliderH* nativeInstance)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->GetRangeMaximum();
	}

	float ScriptGUISliderH::InternalGetRangeMinimum(ScriptGUISliderH* nativeInstance)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->GetRangeMinimum();
	}

	void ScriptGUISliderH::InternalSetStep(ScriptGUISliderH* nativeInstance, float step)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->SetStep(step);
	}

	float ScriptGUISliderH::InternalGetStep(ScriptGUISliderH* nativeInstance)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		return slider->GetStep();
	}

	void ScriptGUISliderH::InternalSetTint(ScriptGUISliderH* nativeInstance, Color* color)
	{
		GUISliderHorz* slider = (GUISliderHorz*)nativeInstance->GetGUIElement();
		slider->SetTint(*color);
	}

	void ScriptGUISliderH::OnChanged(float percent)
	{
		MonoUtil::invokeThunk(onChangedThunk, getManagedInstance(), percent);
	}

	ScriptGUISliderV::OnChangedThunkDef ScriptGUISliderV::onChangedThunk;

	ScriptGUISliderV::ScriptGUISliderV(MonoObject* instance, GUISliderVert* slider)
		:TScriptGUIElement(instance, slider)
	{

	}

	void ScriptGUISliderV::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUISliderV::InternalCreateInstance);
		metaData.scriptClass->addInternalCall("Internal_SetPercent", (void*)&ScriptGUISliderV::InternalSetPercent);
		metaData.scriptClass->addInternalCall("Internal_GetPercent", (void*)&ScriptGUISliderV::InternalGetPercent);
		metaData.scriptClass->addInternalCall("Internal_SetTint", (void*)&ScriptGUISliderV::InternalSetTint);
		metaData.scriptClass->addInternalCall("Internal_GetValue", (void*)&ScriptGUISliderV::InternalGetValue);
		metaData.scriptClass->addInternalCall("Internal_SetValue", (void*)&ScriptGUISliderV::InternalSetValue);
		metaData.scriptClass->addInternalCall("Internal_SetRange", (void*)&ScriptGUISliderV::InternalSetRange);
		metaData.scriptClass->addInternalCall("Internal_GetRangeMaximum", (void*)&ScriptGUISliderV::InternalGetRangeMaximum);
		metaData.scriptClass->addInternalCall("Internal_GetRangeMinimum", (void*)ScriptGUISliderV::InternalGetRangeMinimum);
		metaData.scriptClass->addInternalCall("Internal_SetStep", (void*)&ScriptGUISliderV::InternalSetStep);
		metaData.scriptClass->addInternalCall("Internal_GetStep", (void*)&ScriptGUISliderV::InternalGetStep);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->GetMethod("DoOnChanged", 1)->GetThunk();
	}

	void ScriptGUISliderV::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUISliderVert* guiSlider = GUISliderVert::Create(options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUISliderV>()) ScriptGUISliderV(instance, guiSlider);
		guiSlider->onChanged.Connect(std::bind(&ScriptGUISliderV::onChanged, nativeInstance, _1));
	}

	void ScriptGUISliderV::InternalSetPercent(ScriptGUISliderV* nativeInstance, float percent)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		slider->SetPercent(percent);
	}

	float ScriptGUISliderV::InternalGetPercent(ScriptGUISliderV* nativeInstance)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->GetPercent();
	}

	float ScriptGUISliderV::InternalGetValue(ScriptGUISliderV* nativeInstance)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->GetValue();
	}

	void ScriptGUISliderV::InternalSetValue(ScriptGUISliderV* nativeInstance, float percent)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->SetValue(percent);
	}

	void ScriptGUISliderV::InternalSetRange(ScriptGUISliderV* nativeInstance, float min, float max)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->SetRange(min, max);
	}

	float ScriptGUISliderV::InternalGetRangeMaximum(ScriptGUISliderV* nativeInstance)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->GetRangeMaximum();
	}

	float ScriptGUISliderV::InternalGetRangeMinimum(ScriptGUISliderV* nativeInstance)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->GetRangeMinimum();
	}

	void ScriptGUISliderV::InternalSetStep(ScriptGUISliderV* nativeInstance, float step)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->SetStep(step);
	}

	float ScriptGUISliderV::InternalGetStep(ScriptGUISliderV* nativeInstance)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		return slider->GetStep();
	}

	void ScriptGUISliderV::InternalSetTint(ScriptGUISliderV* nativeInstance, Color* color)
	{
		GUISliderVert* slider = (GUISliderVert*)nativeInstance->GetGUIElement();
		slider->SetTint(*color);
	}

	void ScriptGUISliderV::OnChanged(float percent)
	{
		MonoUtil::invokeThunk(onChangedThunk, getManagedInstance(), percent);
	}
}
