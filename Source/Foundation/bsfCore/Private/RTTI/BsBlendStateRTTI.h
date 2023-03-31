//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "RenderAPI/BsGpuPipelineState.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	template <>
	struct RTTIPlainType<BlendStateInformation>
	{
		enum
		{
			id = TID_BlendStateInformation
		};

		enum
		{
			hasDynamicSize = 1
		};

		static BitLength ToMemory(const BlendStateInformation& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return B3DRTTIWriteWithSizeHeader(stream, data, compress, [&data, &stream]()
											   { return stream.WriteBytes(data); });
		}

		static BitLength FromMemory(BlendStateInformation& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			B3DRTTIReadSizeHeader(stream, compress, size);
			stream.ReadBytes(data);

			return size;
		}

		static BitLength GetSize(const BlendStateInformation& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = sizeof(data);
			B3DRTTIAddHeaderSize(dataSize, compress);

			return dataSize;
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
