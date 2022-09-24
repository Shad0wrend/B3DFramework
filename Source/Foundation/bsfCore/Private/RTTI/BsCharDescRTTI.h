//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Text/BsFontDesc.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	template<> struct RTTIPlainType<CharDesc>
	{
		enum { id = TID_CHAR_DESC }; enum { hasDynamicSize = 1 };

		static BitLength ToMemory(const CharDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.CharId, stream);
				size += rtti_write(data.Page, stream);
				size += rtti_write(data.UvX, stream);
				size += rtti_write(data.UvY, stream);
				size += rtti_write(data.UvWidth, stream);
				size += rtti_write(data.UvHeight, stream);
				size += rtti_write(data.Width, stream);
				size += rtti_write(data.Height, stream);
				size += rtti_write(data.XOffset, stream);
				size += rtti_write(data.YOffset, stream);
				size += rtti_write(data.XAdvance, stream);
				size += rtti_write(data.YAdvance, stream);
				size += rtti_write(data.KerningPairs, stream);

				return size;
			});
		}

		static BitLength FromMemory(CharDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);
			rtti_read(data.CharId, stream);
			rtti_read(data.Page, stream);
			rtti_read(data.UvX, stream);
			rtti_read(data.UvY, stream);
			rtti_read(data.UvWidth, stream);
			rtti_read(data.UvHeight, stream);
			rtti_read(data.Width, stream);
			rtti_read(data.Height, stream);
			rtti_read(data.XOffset, stream);
			rtti_read(data.YOffset, stream);
			rtti_read(data.XAdvance, stream);
			rtti_read(data.YAdvance, stream);
			rtti_read(data.KerningPairs, stream);

			return size;
		}

		static BitLength GetSize(const CharDesc& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.CharId)
				+ rtti_size(data.Page)
				+ rtti_size(data.UvX)
				+ rtti_size(data.UvY)
				+ rtti_size(data.UvWidth)
				+ rtti_size(data.UvHeight)
				+ rtti_size(data.Width)
				+ rtti_size(data.Height)
				+ rtti_size(data.XOffset)
				+ rtti_size(data.YOffset)
				+ rtti_size(data.XAdvance)
				+ rtti_size(data.YAdvance)
				+ rtti_size(data.KerningPairs);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	/** @} */
	/** @endcond */
}
