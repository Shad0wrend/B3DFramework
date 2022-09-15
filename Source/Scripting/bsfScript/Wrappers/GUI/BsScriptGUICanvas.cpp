//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUICanvas.h"
#include "BsScriptMeta.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUICanvas.h"
#include "GUI/BsGUIOptions.h"

#include "Generated/BsScriptFont.generated.h"
#include "Generated/BsScriptSpriteTexture.generated.h"

namespace bs
{
	ScriptGUICanvas::ScriptGUICanvas(MonoObject* instance, GUICanvas* canvas)
		:TScriptGUIElement(instance, canvas)
	{

	}

	void ScriptGUICanvas::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUICanvas::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_DrawLine", (void*)&ScriptGUICanvas::InternalDrawLine);
		metaData.scriptClass->AddInternalCall("Internal_DrawPolyLine", (void*)&ScriptGUICanvas::InternalDrawPolyLine);
		metaData.scriptClass->AddInternalCall("Internal_DrawTexture", (void*)&ScriptGUICanvas::InternalDrawTexture);
		metaData.scriptClass->AddInternalCall("Internal_DrawTriangleStrip", (void*)&ScriptGUICanvas::InternalDrawTriangleStrip);
		metaData.scriptClass->AddInternalCall("Internal_DrawTriangleList", (void*)&ScriptGUICanvas::InternalDrawTriangleList);
		metaData.scriptClass->AddInternalCall("Internal_DrawText", (void*)&ScriptGUICanvas::InternalDrawText);
		metaData.scriptClass->addInternalCall("Internal_Clear", (void*)&ScriptGUICanvas::InternalClear);
	}

	void ScriptGUICanvas::InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUICanvas* guiCanvas = GUICanvas::Create(options, MonoUtil::monoToString(style));

		new (bs_alloc<ScriptGUICanvas>()) ScriptGUICanvas(instance, guiCanvas);
	}

	void ScriptGUICanvas::InternalDrawLine(ScriptGUICanvas* nativeInstance, Vector2I* a, Vector2I* b, Color* color,
		UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();
		canvas->drawLine(*a, *b, *color, depth);
	}

	void ScriptGUICanvas::InternalDrawPolyLine(ScriptGUICanvas* nativeInstance, MonoArray* vertices, Color* color,
		UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();

		ScriptArray verticesArray(vertices);
		UINT32 size = verticesArray.size();

		Vector<Vector2I> nativeVertices(size);
		memcpy(nativeVertices.data(), verticesArray.getRaw<Vector2I>(), sizeof(Vector2I) * size);

		canvas->drawPolyLine(nativeVertices, *color, depth);
	}

	void ScriptGUICanvas::InternalDrawTexture(ScriptGUICanvas* nativeInstance, ScriptSpriteTexture* texture, Rect2I* area,
		TextureScaleMode scaleMode, Color* color, UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();

		HSpriteTexture nativeTexture;
		if (texture != nullptr)
			nativeTexture = texture->GetHandle();

		canvas->drawTexture(nativeTexture, *area, scaleMode, *color, depth);
	}
	
	void ScriptGUICanvas::InternalDrawTriangleStrip(ScriptGUICanvas* nativeInstance, MonoArray* vertices, Color* color,
		UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();

		ScriptArray verticesArray(vertices);
		UINT32 size = verticesArray.size();

		Vector<Vector2I> nativeVertices(size);
		memcpy(nativeVertices.data(), verticesArray.getRaw<Vector2I>(), sizeof(Vector2I) * size);

		canvas->drawTriangleStrip(nativeVertices, *color, depth);
	}

	void ScriptGUICanvas::InternalDrawTriangleList(ScriptGUICanvas* nativeInstance, MonoArray* vertices, Color* color,
		UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();

		ScriptArray verticesArray(vertices);
		UINT32 size = verticesArray.size();

		Vector<Vector2I> nativeVertices(size);
		memcpy(nativeVertices.data(), verticesArray.getRaw<Vector2I>(), sizeof(Vector2I) * size);

		canvas->drawTriangleList(nativeVertices, *color, depth);
	}

	void ScriptGUICanvas::InternalDrawText(ScriptGUICanvas* nativeInstance, MonoString* text, Vector2I* position,
		ScriptFont* font, UINT32 size, Color* color, UINT8 depth)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();
		String nativeText = MonoUtil::monoToString(text);

		HFont nativeFont;
		if (font != nullptr)
			nativeFont = font->GetHandle();

		canvas->drawText(nativeText, *position, nativeFont, size, *color, depth);
	}

	void ScriptGUICanvas::InternalClear(ScriptGUICanvas* nativeInstance)
	{
		GUICanvas* canvas = (GUICanvas*)nativeInstance->GetGUIElement();
		canvas->clear();
	}
}
