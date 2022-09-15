//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptCursor.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Platform/BsCursor.h"
#include "Generated/BsScriptPixelData.generated.h"

namespace bs
{
	ScriptCursor::ScriptCursor(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptCursor::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetScreenPosition", (void*)&ScriptCursor::InternalGetScreenPosition);
		metaData.scriptClass->AddInternalCall("Internal_SetScreenPosition", (void*)&ScriptCursor::InternalSetScreenPosition);
		metaData.scriptClass->AddInternalCall("Internal_Hide", (void*)&ScriptCursor::InternalHide);
		metaData.scriptClass->AddInternalCall("Internal_Show", (void*)&ScriptCursor::InternalShow);
		metaData.scriptClass->AddInternalCall("Internal_ClipToRect", (void*)&ScriptCursor::InternalClipToRect);
		metaData.scriptClass->AddInternalCall("Internal_ClipDisable", (void*)&ScriptCursor::InternalClipDisable);
		metaData.scriptClass->AddInternalCall("Internal_SetCursor", (void*)&ScriptCursor::InternalSetCursor);
		metaData.scriptClass->AddInternalCall("Internal_SetCursorStr", (void*)&ScriptCursor::InternalSetCursorStr);
		metaData.scriptClass->AddInternalCall("Internal_SetCursorIcon", (void*)&ScriptCursor::InternalSetCursorIcon);
		metaData.scriptClass->AddInternalCall("Internal_SetCursorIconStr", (void*)&ScriptCursor::InternalSetCursorIconStr);
		metaData.scriptClass->AddInternalCall("Internal_ClearCursorIcon", (void*)&ScriptCursor::InternalClearCursorIcon);
		metaData.scriptClass->AddInternalCall("Internal_ClearCursorIconStr", (void*)&ScriptCursor::InternalClearCursorIconStr);
	}

	void ScriptCursor::InternalGetScreenPosition(Vector2I* value)
	{
		Cursor::Instance().GetScreenPosition();
	}

	void ScriptCursor::InternalSetScreenPosition(Vector2I* value)
	{
		Cursor::Instance().SetScreenPosition(*value);
	}

	void ScriptCursor::InternalHide()
	{
		Cursor::Instance().Hide();
	}

	void ScriptCursor::InternalShow()
	{
		Cursor::Instance().show();
	}

	void ScriptCursor::InternalClipToRect(Rect2I* value)
	{
		Cursor::Instance().clipToRect(*value);
	}

	void ScriptCursor::InternalClipDisable()
	{
		Cursor::Instance().clipDisable();
	}

	void ScriptCursor::InternalSetCursorStr(MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);
		Cursor::Instance().setCursor(nameStr);
	}

	void ScriptCursor::InternalSetCursor(CursorType cursor)
	{
		Cursor::Instance().setCursor(cursor);
	}

	void ScriptCursor::InternalSetCursorIconStr(MonoString* name, MonoObject* iconData, Vector2I* hotspot)
	{
		String nameStr = MonoUtil::monoToString(name);

		ScriptPixelData* scriptPixelData = ScriptPixelData::toNative(iconData);

		if (scriptPixelData != nullptr)
		{
			SPtr<PixelData> pixelData = scriptPixelData->GetInternal();
			Cursor::Instance().setCursorIcon(nameStr, *pixelData, *hotspot);
		}
		else
			Cursor::Instance().clearCursorIcon(nameStr);
	}

	void ScriptCursor::InternalSetCursorIcon(CursorType cursor, MonoObject* iconData, Vector2I* hotspot)
	{
		ScriptPixelData* scriptPixelData = ScriptPixelData::toNative(iconData);

		if (scriptPixelData != nullptr)
		{
			SPtr<PixelData> pixelData = scriptPixelData->GetInternal();
			Cursor::Instance().setCursorIcon(cursor, *pixelData, *hotspot);
		}
		else
			Cursor::Instance().clearCursorIcon(cursor);
	}

	void ScriptCursor::InternalClearCursorIconStr(MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);
		Cursor::Instance().clearCursorIcon(nameStr);
	}

	void ScriptCursor::InternalClearCursorIcon(CursorType cursor)
	{
		Cursor::Instance().clearCursorIcon(cursor);
	}
}
