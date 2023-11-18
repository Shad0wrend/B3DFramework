//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGUIVectorPaths.h"
#include "StyleSheet/BsGUIStyleSheet.h"
#include "VectorGraphics/BsVectorGraphics.h"

using namespace bs;

HVectorPath GUIBackgroundVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	HVectorPath path = VectorPath::Create(Size2((float)size.Width, (float)size.Height));

	const Rect2 fillArea = Rect2(0.0f, 0.0f, (float)size.Width, (float)size.Height);

	path->DrawRoundedRectangle(fillArea, (float)styleSheetRule.BorderTopLeftRadius, (float)styleSheetRule.BorderTopRightRadius, (float)styleSheetRule. BorderBottomLeftRadius, (float)styleSheetRule.BorderTopRightRadius)
		.ClosePath()
		.SetFillPaint(styleSheetRule.BackgroundColor)
		.DrawFill();

	// TODO - Not supporting separate border styles at the moment. See nvgRoundedRectVarying for implementation. Also ideally support elliptical corners
	const bool drawBorder = styleSheetRule.BorderLeft.Width > 0 && styleSheetRule.BorderLeft.Style != GUIBorderElementStyle::None;
	if(drawBorder)
	{
		path->SetStrokePaint(styleSheetRule.BorderLeft.Color)
			.SetStrokeWidth((float)styleSheetRule.BorderLeft.Width)
			.DrawStroke();
	}

	return path;
}

HVectorPath GUICheckmarkVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	HVectorPath path = VectorPath::Create(Size2(512.0f, 512.0f));

	// TODO: Each GUI element will create its own path, while in most cases only one of these will be needed for the entire UI
	path->SetDrawCursor(Vector2(17.47f, 250.9f))
		.DrawCubicBezierTo(Vector2(88.82f, 328.1f), Vector2(158.0f, 397.6f), Vector2(224.5f, 485.5f))
		.DrawCubicBezierTo(Vector2(296.8f, 341.7f), Vector2(370.8f, 197.4f), Vector2(492.9f, 41.13f))
		.DrawLineTo(Vector2(460.0f, 26.06f))
		.DrawCubicBezierTo(Vector2(356.9f, 135.4f), Vector2(276.8f, 238.9f), Vector2(207.2f, 361.9f))
		.DrawCubicBezierTo(Vector2(158.8f, 318.3f), Vector2(80.58f, 256.6f), Vector2(32.82f, 224.9f))
		.ClosePath()
		.SetFillPaint(styleSheetRule.Color)
		.DrawFill();

	return path;
}

HVectorPath GUITabBackgroundVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	HVectorPath path = VectorPath::Create(Size2(135.0f, 27.0f));

	path->SetDrawCursor(Vector2(22.231762f, 4.4818461e-4f))
		.DrawCubicBezierTo(Vector2(15.114941f, 0.00594818f), Vector2(7.1471837f, 26.91633f), Vector2(0.23176174f, 27.000448f))
		.DrawCubicBezierTo(Vector2(-6.6836603f, 27.084548f), Vector2(143.51984f, 27.090248f), Vector2(135.23176f, 27.000448f))
		.DrawCubicBezierTo(Vector2(126.94367f, 26.910658f), Vector2(120.66325f, -0.04179682f), Vector2(113.23176f, 4.4818461e-4f))
		.DrawCubicBezierTo(Vector2(105.80027f, 0.04269818f), Vector2(29.348582f, -0.00505182f), Vector2(22.231762f, 4.4818461e-4f))
		.ClosePath()
		.SetFillPaint(styleSheetRule.BackgroundColor)
		.DrawFill();

	const bool drawBorder = styleSheetRule.BorderLeft.Width > 0 && styleSheetRule.BorderLeft.Style != GUIBorderElementStyle::None;
	if(drawBorder)
	{
		path->SetStrokePaint(styleSheetRule.BorderLeft.Color)
			.SetStrokeWidth((float)styleSheetRule.BorderLeft.Width)
			.DrawStroke();
	}

	return path;
}

HVectorPath GUIDropDownArrowVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	HVectorPath path = VectorPath::Create(Size2(100.0f, 100.0f));

	path->SetDrawCursor(Vector2(0.0f, 0.0f))
		.DrawLineTo(Vector2(50.0f, 100.0f))
		.DrawLineTo(Vector2(100.0f, 0.0f))
		.ClosePath()
		.SetFillPaint(styleSheetRule.BackgroundColor)
		.DrawFill();

	const bool drawBorder = styleSheetRule.BorderLeft.Width > 0 && styleSheetRule.BorderLeft.Style != GUIBorderElementStyle::None;
	if(drawBorder)
	{
		path->SetStrokePaint(styleSheetRule.BorderLeft.Color)
			.SetStrokeWidth((float)styleSheetRule.BorderLeft.Width)
			.DrawStroke();
	}

	return path;
}

HVectorPath GUIScrollArrowVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	HVectorPath path = VectorPath::Create(Size2(100.0f, 75.0f));

	path->SetDrawCursor(Vector2(0.0f, 0.0f))
		.DrawLineTo(Vector2(50.724423f, 75.0f))
		.DrawLineTo(Vector2(100.0f, 0.0f))
		.DrawCubicBezierTo(Vector2(69.114599f, 25.917529f), Vector2(36.035557f, 28.608797f), Vector2(0.0f, 0.0f))
		.ClosePath()
		.SetFillPaint(styleSheetRule.Color)
		.DrawFill();

	const bool drawBorder = styleSheetRule.BorderLeft.Width > 0 && styleSheetRule.BorderLeft.Style != GUIBorderElementStyle::None;
	if(drawBorder)
	{
		path->SetStrokePaint(styleSheetRule.BorderLeft.Color)
			.SetStrokeWidth((float)styleSheetRule.BorderLeft.Width)
			.DrawStroke();
	}

	return path;
}

HVectorPath GUIScrollHandleVectorPathBuilder::BuildPath(const Size2UI& size, const GUIStyleSheetRules& styleSheetRule) const
{
	constexpr u32 kReferenceRasterSize = 13; // Reference size of the handle in pixels, both width and height
	constexpr float kReferenceCanvasSize = 100.0f; // Reference size of the vector path canvas

	float canvasUnitsPerPixel = kReferenceCanvasSize / (float)kReferenceRasterSize;

	// Adjust the height so it expands
	const u32 shaftRasterHeight = (u32)Math::Max(0, ((i32)size.Height - (i32)kReferenceRasterSize)); // Height not including the caps
	float shaftCanvasHeight = (float)shaftRasterHeight * canvasUnitsPerPixel;

	const Size2 canvasSize(kReferenceCanvasSize, kReferenceCanvasSize + shaftCanvasHeight);

	HVectorPath path = VectorPath::Create(canvasSize);

	path->SetDrawCursor(Vector2(0.0, 50.0f))
		.DrawCubicBezierTo(Vector2(0.0f, 22.32227f), Vector2(22.605928f, 0.0f), Vector2(50.0f, 0.0f))
		.DrawCubicBezierTo(Vector2(77.677738f, 0.0f), Vector2(100.0f, 22.32225f), Vector2(100.0f, 50.0f))
		.DrawLineTo(Vector2(100.0f, 50.0f + shaftCanvasHeight))
		.DrawCubicBezierTo(Vector2(100.0f, 77.78187f), Vector2(77.677738f, 99.78705f), Vector2(50.0f, 100.0f))
		.DrawCubicBezierTo(Vector2(22.60593f, 100.0f), Vector2(0.0f, 77.78187f), Vector2(0.0f, 50.0f))
		.ClosePath()
		.SetFillPaint(styleSheetRule.Color)
		.DrawFill();

	const bool drawBorder = styleSheetRule.BorderLeft.Width > 0 && styleSheetRule.BorderLeft.Style != GUIBorderElementStyle::None;
	if(drawBorder)
	{
		path->SetStrokePaint(styleSheetRule.BorderLeft.Color)
			.SetStrokeWidth((float)styleSheetRule.BorderLeft.Width)
			.DrawStroke();
	}

	return path;
}
