//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptInput.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Input/BsInput.h"
#include "Wrappers/BsScriptVector2I.h"
#include "BsPlayInEditor.h"

namespace bs
{
	ScriptInput::OnButtonEventThunkDef ScriptInput::OnButtonPressedThunk;
	ScriptInput::OnButtonEventThunkDef ScriptInput::OnButtonReleasedThunk;
	ScriptInput::OnCharInputEventThunkDef ScriptInput::OnCharInputThunk;
	ScriptInput::OnPointerEventThunkDef ScriptInput::OnPointerPressedThunk;
	ScriptInput::OnPointerEventThunkDef ScriptInput::OnPointerReleasedThunk;
	ScriptInput::OnPointerEventThunkDef ScriptInput::OnPointerMovedThunk;
	ScriptInput::OnPointerEventThunkDef ScriptInput::OnPointerDoubleClickThunk;

	HEvent ScriptInput::OnButtonPressedConn;
	HEvent ScriptInput::OnButtonReleasedConn;
	HEvent ScriptInput::OnCharInputConn;
	HEvent ScriptInput::OnPointerPressedConn;
	HEvent ScriptInput::OnPointerReleasedConn;
	HEvent ScriptInput::OnPointerMovedConn;
	HEvent ScriptInput::OnPointerDoubleClickConn;

	ScriptInput::ScriptInput(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptInput::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_IsButtonHeld", (void*)&ScriptInput::InternalIsButtonHeld);
		metaData.scriptClass->AddInternalCall("Internal_IsButtonDown", (void*)&ScriptInput::InternalIsButtonDown);
		metaData.scriptClass->AddInternalCall("Internal_IsButtonUp", (void*)&ScriptInput::InternalIsButtonUp);
		metaData.scriptClass->AddInternalCall("Internal_IsPointerButtonHeld", (void*)&ScriptInput::InternalIsPointerButtonHeld);
		metaData.scriptClass->AddInternalCall("Internal_IsPointerButtonDown", (void*)&ScriptInput::InternalIsPointerButtonDown);
		metaData.scriptClass->AddInternalCall("Internal_IsPointerButtonUp", (void*)&ScriptInput::InternalIsPointerButtonUp);
		metaData.scriptClass->AddInternalCall("Internal_IsPointerDoubleClicked", (void*)&ScriptInput::InternalIsPointerDoubleClicked);
		metaData.scriptClass->AddInternalCall("Internal_GetAxisValue", (void*)&ScriptInput::InternalGetAxisValue);
		metaData.scriptClass->AddInternalCall("Internal_GetPointerPosition", (void*)&ScriptInput::InternalGetPointerPosition);
		metaData.scriptClass->AddInternalCall("Internal_GetPointerDelta", (void*)&ScriptInput::InternalGetPointerDelta);

		OnButtonPressedThunk = (OnButtonEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerButtonDown", "ButtonCode,int,bool")->GetThunk();
		OnButtonReleasedThunk = (OnButtonEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerButtonUp", "ButtonCode,int,bool")->GetThunk();
		OnCharInputThunk = (OnCharInputEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerCharInput", "int,bool")->GetThunk();
		OnPointerPressedThunk = (OnPointerEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerPointerPressed", "Vector2I,Vector2I,PointerButton,bool,bool,bool,single,bool")->GetThunk();
		OnPointerReleasedThunk = (OnPointerEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerPointerReleased", "Vector2I,Vector2I,PointerButton,bool,bool,bool,single,bool")->GetThunk();
		OnPointerMovedThunk = (OnPointerEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerPointerMove", "Vector2I,Vector2I,PointerButton,bool,bool,bool,single,bool")->GetThunk();
		OnPointerDoubleClickThunk = (OnPointerEventThunkDef)metaData.scriptClass->GetMethodExact("Internal_TriggerPointerDoubleClick", "Vector2I,Vector2I,PointerButton,bool,bool,bool,single,bool")->GetThunk();
	}

	void ScriptInput::StartUp()
	{
		Input& input = Input::Instance();

		OnButtonPressedConn = input.onButtonDown.Connect(&ScriptInput::OnButtonDown);
		OnButtonReleasedConn = input.onButtonUp.Connect(&ScriptInput::OnButtonUp);
		OnCharInputConn = input.onCharInput.Connect(&ScriptInput::OnCharInput);
		OnPointerPressedConn = input.onPointerPressed.Connect(&ScriptInput::OnPointerPressed);
		OnPointerReleasedConn = input.onPointerReleased.Connect(&ScriptInput::OnPointerReleased);
		OnPointerMovedConn = input.onPointerMoved.Connect(&ScriptInput::OnPointerMoved);
		OnPointerDoubleClickConn = input.onPointerDoubleClick.Connect(&ScriptInput::OnPointerDoubleClick);
	}

	void ScriptInput::ShutDown()
	{
		OnButtonPressedConn.Disconnect();
		OnButtonReleasedConn.Disconnect();
		OnCharInputConn.Disconnect();
		OnPointerPressedConn.Disconnect();
		OnPointerReleasedConn.Disconnect();
		OnPointerMovedConn.Disconnect();
		OnPointerDoubleClickConn.Disconnect();
	}

	void ScriptInput::OnButtonDown(const ButtonEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoUtil::invokeThunk(OnButtonPressedThunk, ev.buttonCode, ev.deviceIdx, ev.isUsed());
	}

	void ScriptInput::OnButtonUp(const ButtonEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoUtil::invokeThunk(OnButtonReleasedThunk, ev.buttonCode, ev.deviceIdx, ev.isUsed());
	}

	void ScriptInput::OnCharInput(const TextInputEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoUtil::invokeThunk(OnCharInputThunk, ev.textChar, ev.isUsed());
	}

	void ScriptInput::OnPointerMoved(const PointerEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoObject* screenPos = ScriptVector2I::box(ev.screenPos);
		MonoObject* delta = ScriptVector2I::box(ev.delta);

		MonoUtil::invokeThunk(OnPointerMovedThunk, screenPos, delta,
			ev.button, ev.shift, ev.control, ev.alt, ev.mouseWheelScrollAmount, ev.isUsed());
	}

	void ScriptInput::OnPointerPressed(const PointerEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoObject* screenPos = ScriptVector2I::box(ev.screenPos);
		MonoObject* delta = ScriptVector2I::box(ev.delta);

		MonoUtil::invokeThunk(OnPointerPressedThunk, screenPos, delta,
			ev.button, ev.shift, ev.control, ev.alt, ev.mouseWheelScrollAmount, ev.isUsed());
	}

	void ScriptInput::OnPointerReleased(const PointerEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoObject* screenPos = ScriptVector2I::box(ev.screenPos);
		MonoObject* delta = ScriptVector2I::box(ev.delta);

		MonoUtil::invokeThunk(OnPointerReleasedThunk, screenPos, delta,
			ev.button, ev.shift, ev.control, ev.alt, ev.mouseWheelScrollAmount, ev.isUsed());
	}

	void ScriptInput::OnPointerDoubleClick(const PointerEvent& ev)
	{
		if (PlayInEditor::Instance().getState() != PlayInEditorState::Playing)
			return;

		MonoObject* screenPos = ScriptVector2I::box(ev.screenPos);
		MonoObject* delta = ScriptVector2I::box(ev.delta);

		MonoUtil::invokeThunk(OnPointerDoubleClickThunk, screenPos, delta,
			ev.button, ev.shift, ev.control, ev.alt, ev.mouseWheelScrollAmount, ev.isUsed());
	}

	bool ScriptInput::InternalIsButtonHeld(ButtonCode code, UINT32 deviceIdx)
	{
		return Input::Instance().isButtonHeld(code, deviceIdx);
	}

	bool ScriptInput::InternalIsButtonDown(ButtonCode code, UINT32 deviceIdx)
	{
		return Input::Instance().isButtonDown(code, deviceIdx);
	}

	bool ScriptInput::InternalIsButtonUp(ButtonCode code, UINT32 deviceIdx)
	{
		return Input::Instance().isButtonUp(code, deviceIdx);
	}

	bool ScriptInput::InternalIsPointerButtonHeld(PointerEventButton code)
	{
		return Input::Instance().isPointerButtonHeld(code);
	}

	bool ScriptInput::InternalIsPointerButtonDown(PointerEventButton code)
	{
		return Input::Instance().isPointerButtonDown(code);
	}

	bool ScriptInput::InternalIsPointerButtonUp(PointerEventButton code)
	{
		return Input::Instance().isPointerButtonUp(code);
	}

	bool ScriptInput::InternalIsPointerDoubleClicked()
	{
		return Input::Instance().isPointerDoubleClicked();
	}

	float ScriptInput::InternalGetAxisValue(UINT32 axisType, UINT32 deviceIdx)
	{
		return Input::Instance().getAxisValue(axisType, deviceIdx);
	}

	void ScriptInput::InternalGetPointerPosition(Vector2I* position)
	{
		*position = Input::Instance().getPointerPosition();
	}

	void ScriptInput::InternalGetPointerDelta(Vector2I* position)
	{
		*position = Input::Instance().getPointerDelta();
	}
}
