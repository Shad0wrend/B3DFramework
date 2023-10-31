//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"

namespace bs
{
	/** @addtogroup Text
	 *  @{
	 */

	/**	Kerning pair representing larger or smaller offset between a specific pair of characters. */
	struct B3D_SCRIPT_EXPORT(ExportAsStruct(true), DocumentationGroup(GUI_Engine)) KerningPair
	{
		u32 OtherCharId;
		float Amount;
	};

	/**	Describes a single character in a font of a specific size. */
	struct B3D_SCRIPT_EXPORT(ExportAsStruct(true), DocumentationGroup(GUI_Engine)) CharacterInformation
	{
		u32 CharId; /**< Character ID, corresponding to a Unicode key. */
		u32 Page; /**< Index of the texture the character is located on. */
		float UvX, UvY; /**< Texture coordinates of the character in the page texture. */
		float UvWidth, UvHeight; /**< Width/height of the character in texture coordinates. */
		float Width, Height; /**< Width/height of the character in pixels. */
		float XOffset, YOffset; /**< Offset for the visible portion of the character in pixels. */
		float XAdvance, YAdvance; /**< Determines how much to advance the pen after writing this character, in pixels. */

		/**
		 * Pairs that determine if certain character pairs should be closer or father together. for example "AV"
		 * combination.
		 */
		Vector<KerningPair> KerningPairs;
	};

	/**	Contains textures and data about every character for a bitmap font of a specific size. */
	struct B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(GUI_Engine)) FontBitmap : public IReflectable
	{
		/**	Returns a character description for the character with the specified Unicode key. */
		B3D_SCRIPT_EXPORT()
		const CharacterInformation& GetCharacterInformation(u32 characterId) const;

		/** Font size for which the data is contained. */
		B3D_SCRIPT_EXPORT()
		u32 Size;

		/** Y offset to the baseline on which the characters are placed, in pixels. */
		B3D_SCRIPT_EXPORT()
		float BaselineOffset;

		/** Height of a single line of the font, in pixels. */
		B3D_SCRIPT_EXPORT()
		float LineHeight;

		/** Character to use when data for a character is missing. */
		B3D_SCRIPT_EXPORT()
		CharacterInformation MissingGlyph;

		/** Width of a space in pixels. */
		B3D_SCRIPT_EXPORT()
		float SpaceWidth;

		/** Textures in which the character's pixels are stored. */
		B3D_SCRIPT_EXPORT()
		Vector<HTexture> TexturePages;

		/** All characters in the font referenced by character ID. */
		Map<u32, CharacterInformation> Characters;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	public:
		friend class FontBitmapRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;
	};

	/**
	 * Font resource containing data about textual characters and how to render text. Contains one or multiple font
	 * bitmaps, each for a specific size.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(GUI_Engine)) Font : public Resource
	{
	public:
		virtual ~Font() = default;

		/**
		 * Returns font bitmap for a specific font size.
		 *
		 * @param[in]	size	Size of the bitmap in points.
		 * @return				Bitmap object if it exists, false otherwise.
		 */
		B3D_SCRIPT_EXPORT()
		SPtr<FontBitmap> GetBitmap(u32 size) const;

		/**
		 * Finds the available font bitmap size closest to the provided size.
		 *
		 * @param[in]	size	Size of the bitmap in points.
		 * @return				Nearest available bitmap size.
		 */
		B3D_SCRIPT_EXPORT()
		i32 GetClosestSize(u32 size) const;

		/**	Creates a new font from the provided per-size font data. */
		static HFont Create(const Vector<SPtr<FontBitmap>>& fontInitData);

	public: // ***** INTERNAL ******
		using Resource::Initialize;

		/** @name Internal
		 *  @{
		 */

		/**
		 * Initializes the font with specified per-size font data.
		 *
		 * @note	Internal method. Factory methods will call this automatically for you.
		 */
		void Initialize(const Vector<SPtr<FontBitmap>>& fontData);

		/** Creates a new font as a pointer instead of a resource handle. */
		static SPtr<Font> CreatePtrInternal(const Vector<SPtr<FontBitmap>>& fontInitData);

		/** Creates a Font without initializing it. */
		static SPtr<Font> CreateEmptyInternal();

		/** @} */

	protected:
		friend class FontManager;

		Font();

		void GetCoreDependencies(Vector<CoreObject*>& dependencies) override;

	private:
		Map<u32, SPtr<FontBitmap>> mFontDataPerSize;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	public:
		friend class FontRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;
	};

	/** @} */
} // namespace bs
