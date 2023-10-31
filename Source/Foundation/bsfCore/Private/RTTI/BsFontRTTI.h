//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStdRTTI.h"
#include "Text/BsFont.h"
#include "Image/BsTexture.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	template <>
	struct RTTIPlainType<CharacterInformation>
	{
		enum
		{
			id = TID_CharacterInformation
		};

		enum
		{
			hasDynamicSize = 1
		};

		static BitLength ToMemory(const CharacterInformation& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return B3DRTTIWriteWithSizeHeader(stream, data, compress, [&data, &stream]()
											   {
				BitLength size = 0;
				size += B3DRTTIWrite(data.CharId, stream);
				size += B3DRTTIWrite(data.Page, stream);
				size += B3DRTTIWrite(data.UvX, stream);
				size += B3DRTTIWrite(data.UvY, stream);
				size += B3DRTTIWrite(data.UvWidth, stream);
				size += B3DRTTIWrite(data.UvHeight, stream);
				size += B3DRTTIWrite(data.Width, stream);
				size += B3DRTTIWrite(data.Height, stream);
				size += B3DRTTIWrite(data.XOffset, stream);
				size += B3DRTTIWrite(data.YOffset, stream);
				size += B3DRTTIWrite(data.XAdvance, stream);
				size += B3DRTTIWrite(data.YAdvance, stream);
				size += B3DRTTIWrite(data.KerningPairs, stream);

				return size; });
		}

		static BitLength FromMemory(CharacterInformation& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			B3DRTTIReadSizeHeader(stream, compress, size);
			B3DRTTIRead(data.CharId, stream);
			B3DRTTIRead(data.Page, stream);
			B3DRTTIRead(data.UvX, stream);
			B3DRTTIRead(data.UvY, stream);
			B3DRTTIRead(data.UvWidth, stream);
			B3DRTTIRead(data.UvHeight, stream);
			B3DRTTIRead(data.Width, stream);
			B3DRTTIRead(data.Height, stream);
			B3DRTTIRead(data.XOffset, stream);
			B3DRTTIRead(data.YOffset, stream);
			B3DRTTIRead(data.XAdvance, stream);
			B3DRTTIRead(data.YAdvance, stream);
			B3DRTTIRead(data.KerningPairs, stream);

			return size;
		}

		static BitLength GetSize(const CharacterInformation& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = B3DRTTISize(data.CharId) + B3DRTTISize(data.Page) + B3DRTTISize(data.UvX) + B3DRTTISize(data.UvY) + B3DRTTISize(data.UvWidth) + B3DRTTISize(data.UvHeight) + B3DRTTISize(data.Width) + B3DRTTISize(data.Height) + B3DRTTISize(data.XOffset) + B3DRTTISize(data.YOffset) + B3DRTTISize(data.XAdvance) + B3DRTTISize(data.YAdvance) + B3DRTTISize(data.KerningPairs);

			B3DRTTIAddHeaderSize(dataSize, compress);
			return dataSize;
		}
	};

	class B3D_CORE_EXPORT FontBitmapRTTI : public RTTIType<FontBitmap, IReflectable, FontBitmapRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(Size, 0)
			B3D_RTTI_MEMBER_PLAIN(BaselineOffset, 1)
			B3D_RTTI_MEMBER_PLAIN(LineHeight, 2)
			B3D_RTTI_MEMBER_PLAIN(MissingGlyph, 3)
			B3D_RTTI_MEMBER_PLAIN(SpaceWidth, 4)
			B3D_RTTI_MEMBER_REFL_ARRAY(TexturePages, 5)
			B3D_RTTI_MEMBER_PLAIN(Characters, 6)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName()
		{
			static String name = "FontData";
			return name;
		}

		u32 GetRttiId()
		{
			return TID_FontBitmap;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return B3DMakeShared<FontBitmap>();
		}
	};

	class B3D_CORE_EXPORT FontRTTI : public RTTIType<Font, Resource, FontRTTI>
	{
	private:
		FontBitmap& GetBitmap(Font* obj, u32 idx)
		{
			if(idx >= obj->mFontDataPerSize.size())
				B3D_EXCEPT(InternalErrorException, "Index out of range: " + ToString(idx) + ". Valid range: 0 .. " + ToString((int)obj->mFontDataPerSize.size()));

			auto iter = obj->mFontDataPerSize.begin();
			for(u32 i = 0; i < idx; i++, ++iter)
			{}

			return *iter->second;
		}

		void SetBitmap(Font* obj, u32 idx, FontBitmap& value)
		{
			mFontDataPerSize[idx] = B3DMakeShared<FontBitmap>();
			*mFontDataPerSize[idx] = value;
		}

		u32 GetNumBitmaps(Font* obj)
		{
			return (u32)obj->mFontDataPerSize.size();
		}

		void SetNumBitmaps(Font* obj, u32 size)
		{
			mFontDataPerSize.resize(size);
		}

	public:
		FontRTTI()
		{
			AddReflectableArrayField("mBitmaps", 0, &FontRTTI::GetBitmap, &FontRTTI::GetNumBitmaps, &FontRTTI::SetBitmap, &FontRTTI::SetNumBitmaps);
		}

		const String& GetRttiName() override
		{
			static String name = "Font";
			return name;
		}

		u32 GetRttiId() override
		{
			return TID_Font;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return Font::CreateEmptyInternal();
		}

	protected:
		void OnDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			Font* font = static_cast<Font*>(obj);
			font->Initialize(mFontDataPerSize);
		}

		Vector<SPtr<FontBitmap>> mFontDataPerSize;
	};

	/** @} */
	/** @endcond */
} // namespace bs
