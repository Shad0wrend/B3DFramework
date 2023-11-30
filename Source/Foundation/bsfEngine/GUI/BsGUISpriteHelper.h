//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "GUI/BsGUIElement.h"
#include "2D/BsImageSprite.h"
#include "2D/BsTextSprite.h"
#include "GUI/BsGUIContent.h"

namespace bs
{
	/** @addtogroup Implementation
	 *  @{
	 */

	/** Wrapper around Sprite that helps construct a sprite for drawing a GUI element background controlled by style sheet rules. */
	class GUIBackgroundSprite
	{
	public:
		GUIBackgroundSprite();

		/**
		 * Builds the background render elements and appends them to the render elements array.
		 *
		 * @param	size				Size of the GUI element as determined by the layouting pass.
		 * @param	rules				Active style-sheet rules for the GUI element.
		 * @param	tint				Runtime color tint to apply to the sprite.
		 * @param	batchId				ID that specifies if the sprite is allowed to be batched with other sprites. Only sprites with the same batch ID can be batched.
		 * @param	outRenderElements	Array to which the generated render element will be appended to.
		 */
		void BuildRenderElements(const Size2UI& size, const GUIStyleSheetRules& rules, const Color& tint, u64 batchId, TInlineArray<GUIRenderElement, 4>& outRenderElements);

		/** Same as the other overload, but for the old deprecated GUIElementStyle type, instead of style-sheets. */
		void BuildRenderElements(const Size2UI& size, const GUIElementStyle& style, GUIElementState state, const Color& tint, u64 batchId, TInlineArray<GUIRenderElement, 4>& outRenderElements);

		/** Updates the animation start time (in seconds since application start), in case the background contains an animated sprite. */
		void SetAnimationStartTime(float time);

		/** Sets an interface that constructs the vector path used for drawing the background. */
		void SetBackgroundPathBuilder(const IGUIVectorPathBuilder* pathBuilder) { mBackgroundPathBuilder = pathBuilder; }
	private:
		ImageSprite mBackgroundSprite;
		ImageSpriteInformation mBackgroundSpriteInformation;

		const IGUIVectorPathBuilder* mBackgroundPathBuilder;
	};

	/** Wrapper around Sprite that helps construct a sprite for drawing a GUI element with text and/or image contents. */
	// TODO - Not implemented
	// TODO - Remove mBackgroundPathBuilder from GUIElementBase
	class GUIContentSprites
	{
	public:
		void Update(GUIStyleSheetRules& rules, const Size2UI& size);

	private:
		ImageSprite mBackgroundSprite;
		ImageSprite mContentImageSprite;
		TextSprite mContentTextSprite;

		ImageSpriteInformation mBackgroundSpriteInformation;
		ImageSpriteInformation mContentImageSpriteInformation;
		TextSpriteInformation mContentTextSpriteInformation;
	};

	/** Provides helper functionality that automatically extracts necessary data from a GUIElement and builds render elements for one of the GUI*Sprites types. */
	class GUISpriteHelper
	{
	public:
		/** Builds sprite elements for GUIBackgroundSprites. */
		static void BuildSpriteRenderElements(GUIElement& element, GUIElementState state, GUIBackgroundSprite& sprite);
	};

	/** @} */
} // namespace bs
