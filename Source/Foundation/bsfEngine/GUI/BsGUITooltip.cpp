//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUITooltip.h"
#include "GUI/BsGUIPanel.h"
#include "Renderer/BsCamera.h"
#include "RenderAPI/BsViewport.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIHelper.h"
#include "GUI/BsGUISkin.h"
#include "Resources/BsBuiltinResources.h"
#include "GUI/BsDropDownAreaPlacement.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUITooltip::TOOLTIP_WIDTH = 200;
	const UINT32 GUITooltip::CURSOR_SIZE = 16;

	String GUITooltip::GetFrameStyleName()
	{
		return "TooltipFrame";
	}

	GUITooltip::GUITooltip(const HSceneObject& parent, const GUIWidget& overlaidWidget, const Vector2I& position,
		const String& text)
		:CGUIWidget(parent, overlaidWidget.GetCamera())
	{
		SetDepth(0); // Needs to be in front of everything
		SetSkin(overlaidWidget.GetSkinResource());
		
		SPtr<Camera> camera = overlaidWidget.GetCamera();
		SPtr<Viewport> viewport = camera->GetViewport();

		Rect2I availableBounds = viewport->GetPixelArea();
		
		const GUISkin& skin = GetSkin();
		const GUIElementStyle* multiLineLabelStyle = skin.GetStyle(BuiltinResources::MultiLineLabelStyle);
		const GUIElementStyle* backgroundStyle = skin.GetStyle(GetFrameStyleName());

		Vector2I size(TOOLTIP_WIDTH, 25);
		if (multiLineLabelStyle != nullptr)
		{
			GUIDimensions dimensions = GUIDimensions::Create(GUIOptions(GUIOption::FixedWidth(TOOLTIP_WIDTH)));
			size = GUIHelper::CalcOptimalContentsSize(text, *multiLineLabelStyle, dimensions);
		}

		INT32 contentOffsetX = 0;
		INT32 contentOffsetY = 0;
		if(backgroundStyle != nullptr)
		{
			size.x += backgroundStyle->margins.left + backgroundStyle->margins.right;
			size.y += backgroundStyle->margins.top + backgroundStyle->margins.bottom;

			contentOffsetX = backgroundStyle->margins.left;
			contentOffsetY = backgroundStyle->margins.top;
		}

		// Content area
		GUIPanel* contentPanel = GetPanel()->AddNewElement<GUIPanel>();
		contentPanel->SetWidth((UINT32)size.x);
		contentPanel->SetHeight((UINT32)size.y);
		contentPanel->SetDepthRange(-1);

		// Background frame
		GUIPanel* backgroundPanel = GetPanel()->AddNewElement<GUIPanel>();
		backgroundPanel->SetWidth((UINT32)size.x);
		backgroundPanel->SetHeight((UINT32)size.y);
		backgroundPanel->SetDepthRange(0);

		GUILayout* backgroundLayout = backgroundPanel->AddNewElement<GUILayoutX>();

		GUITexture* backgroundFrame = GUITexture::Create(TextureScaleMode::StretchToFit, GetFrameStyleName());
		backgroundLayout->AddElement(backgroundFrame);

		GUILayout* contentLayout = contentPanel->AddNewElement<GUILayoutY>();
		contentLayout->AddNewElement<GUILabel>(HString(text),
			GUIOptions(GUIOption::FixedWidth(TOOLTIP_WIDTH), GUIOption::FlexibleHeight()),
			BuiltinResources::MultiLineLabelStyle);

		Rect2I positionBounds;
		positionBounds.x = position.x;
		positionBounds.y = position.y;
		positionBounds.width = CURSOR_SIZE;
		positionBounds.height = CURSOR_SIZE;

		DropDownAreaPlacement::HorzDir horzDir;
		DropDownAreaPlacement::VertDir vertDir;
		DropDownAreaPlacement placement = DropDownAreaPlacement::AroundBounds(positionBounds);
		Rect2I placementBounds = placement.GetOptimalBounds((UINT32)size.x, (UINT32)size.y, availableBounds, horzDir, vertDir);

		backgroundPanel->SetPosition(placementBounds.x, placementBounds.y);
		contentPanel->SetPosition(placementBounds.x + contentOffsetX, placementBounds.y + contentOffsetY);
	}
}
