//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIScrollBar.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "GUI/BsGUIScrollBar.h"
#include "GUI/BsGUIScrollBarHorz.h"
#include "GUI/BsGUIScrollBarVert.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIScrollBar::ScriptGUIScrollBar(MonoObject* instance)
		:TScriptGUIElement(instance, nullptr)
	{

	}

	void ScriptGUIScrollBar::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIScrollBar::InternalSetTint);
		metaData.scriptClass->AddInternalCall("Internal_GetPosition", (void*)&ScriptGUIScrollBar::InternalGetPosition);
		metaData.scriptClass->AddInternalCall("Internal_SetPosition", (void*)&ScriptGUIScrollBar::InternalSetPosition);
		metaData.scriptClass->AddInternalCall("Internal_GetHandleSize", (void*)&ScriptGUIScrollBar::InternalGetHandleSize);
		metaData.scriptClass->AddInternalCall("Internal_SetHandleSize", (void*)&ScriptGUIScrollBar::InternalSetHandleSize);
	}

	float ScriptGUIScrollBar::InternalGetPosition(ScriptGUIElementBaseTBase* nativeInstance)
	{
		GUIScrollBar* scrollBar = (GUIScrollBar*)nativeInstance->GetGuiElement();
		return scrollBar->GetScrollPos();
	}

	void ScriptGUIScrollBar::InternalSetPosition(ScriptGUIElementBaseTBase* nativeInstance, float percent)
	{
		GUIScrollBar* scrollBar = (GUIScrollBar*)nativeInstance->GetGuiElement();
		scrollBar->SetScrollPosInternal(percent);
	}

	float ScriptGUIScrollBar::InternalGetHandleSize(ScriptGUIElementBaseTBase* nativeInstance)
	{
		GUIScrollBar* scrollBar = (GUIScrollBar*)nativeInstance->GetGuiElement();
		return scrollBar->GetHandleSize();
	}

	void ScriptGUIScrollBar::InternalSetHandleSize(ScriptGUIElementBaseTBase* nativeInstance, float percent)
	{
		GUIScrollBar* scrollBar = (GUIScrollBar*)nativeInstance->GetGuiElement();
		scrollBar->SetHandleSize(percent);
	}

	void ScriptGUIScrollBar::InternalSetTint(ScriptGUIElementBaseTBase* nativeInstance, Color* color)
	{
		GUIScrollBar* scrollBar = (GUIScrollBar*)nativeInstance->GetGuiElement();
		scrollBar->SetTint(*color);
	}

	ScriptGUIScrollBarH::OnScrolledThunkDef ScriptGUIScrollBarH::onScrolledThunk;

	ScriptGUIScrollBarH::ScriptGUIScrollBarH(MonoObject* instance, GUIScrollBarHorz* scrollBar)
		:TScriptGUIElement(instance, scrollBar)
	{

	}

	void ScriptGUIScrollBarH::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUIScrollBarH::InternalCreateInstance);

		onScrolledThunk = (OnScrolledThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnScroll", 1)->GetThunk();
	}

	void ScriptGUIScrollBarH::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUIScrollBarHorz* guiScrollBar = GUIScrollBarHorz::Create(options, MonoUtil::monoToString(style));
		ScriptGUIScrollBarH* scriptScrollBar = new (bs_alloc<ScriptGUIScrollBarH>()) ScriptGUIScrollBarH(instance, guiScrollBar);

		guiScrollBar->onScrollOrResize.Connect(std::bind(&ScriptGUIScrollBarH::OnScroll, scriptScrollBar, _1, _2));
	}

	void ScriptGUIScrollBarH::OnScroll(float position, float size)
	{
		MonoUtil::invokeThunk(onScrolledThunk, getManagedInstance(), position);
	}

	ScriptGUIScrollBarV::OnScrolledThunkDef ScriptGUIScrollBarV::onScrolledThunk;

	ScriptGUIScrollBarV::ScriptGUIScrollBarV(MonoObject* instance, GUIScrollBarVert* scrollBar)
		:TScriptGUIElement(instance, scrollBar)
	{

	}

	void ScriptGUIScrollBarV::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUIScrollBarV::InternalCreateInstance);

		onScrolledThunk = (OnScrolledThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnScroll", 1)->GetThunk();
	}

	void ScriptGUIScrollBarV::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUIScrollBarVert* guiScrollBar = GUIScrollBarVert::Create(options, MonoUtil::monoToString(style));
		ScriptGUIScrollBarV* scriptScrollBar = new (bs_alloc<ScriptGUIScrollBarV>()) ScriptGUIScrollBarV(instance, guiScrollBar);

		guiScrollBar->onScrollOrResize.Connect(std::bind(&ScriptGUIScrollBarV::OnScroll, scriptScrollBar, _1, _2));
	}

	void ScriptGUIScrollBarV::OnScroll(float position, float size)
	{
		MonoUtil::invokeThunk(onScrolledThunk, getManagedInstance(), position);
	}

	ScriptGUIResizeableScrollBarH::OnScrollOrResizeThunkDef ScriptGUIResizeableScrollBarH::onScrollOrResizeThunk;

	ScriptGUIResizeableScrollBarH::ScriptGUIResizeableScrollBarH(MonoObject* instance, GUIScrollBarHorz* scrollBar)
		:TScriptGUIElement(instance, scrollBar)
	{

	}

	void ScriptGUIResizeableScrollBarH::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUIResizeableScrollBarH::InternalCreateInstance);

		onScrollOrResizeThunk = (OnScrollOrResizeThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnScrollOrResize", 2)->GetThunk();
	}

	void ScriptGUIResizeableScrollBarH::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUIScrollBarHorz* guiScrollBar = GUIScrollBarHorz::Create(true, options, MonoUtil::monoToString(style));
		ScriptGUIResizeableScrollBarH* scriptScrollBar = new (bs_alloc<ScriptGUIResizeableScrollBarH>()) ScriptGUIResizeableScrollBarH(instance, guiScrollBar);

		guiScrollBar->onScrollOrResize.Connect(std::bind(&ScriptGUIResizeableScrollBarH::OnScroll, scriptScrollBar, _1, _2));
	}

	void ScriptGUIResizeableScrollBarH::OnScroll(float position, float size)
	{
		MonoUtil::invokeThunk(onScrollOrResizeThunk, getManagedInstance(), position, size);
	}

	ScriptGUIResizeableScrollBarV::OnScrollOrResizeThunkDef ScriptGUIResizeableScrollBarV::onScrollOrResizeThunk;

	ScriptGUIResizeableScrollBarV::ScriptGUIResizeableScrollBarV(MonoObject* instance, GUIScrollBarVert* scrollBar)
		:TScriptGUIElement(instance, scrollBar)
	{

	}

	void ScriptGUIResizeableScrollBarV::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", (void*)&ScriptGUIResizeableScrollBarV::InternalCreateInstance);

		onScrollOrResizeThunk = (OnScrollOrResizeThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnScrollOrResize", 2)->GetThunk();
	}

	void ScriptGUIResizeableScrollBarV::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUIScrollBarVert* guiScrollBar = GUIScrollBarVert::Create(true, options, MonoUtil::monoToString(style));
		ScriptGUIResizeableScrollBarV* scriptScrollBar = new (bs_alloc<ScriptGUIResizeableScrollBarV>()) ScriptGUIResizeableScrollBarV(instance, guiScrollBar);

		guiScrollBar->onScrollOrResize.Connect(std::bind(&ScriptGUIResizeableScrollBarV::OnScroll, scriptScrollBar, _1, _2));
	}

	void ScriptGUIResizeableScrollBarV::OnScroll(float position, float size)
	{
		MonoUtil::invokeThunk(onScrollOrResizeThunk, getManagedInstance(), position, size);
	}
}
