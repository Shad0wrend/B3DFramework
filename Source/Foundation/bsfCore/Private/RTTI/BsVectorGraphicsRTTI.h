//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsMathRTTI.h"
#include "VectorGraphics/BsVectorGraphics.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	template <>
	struct RTTIPlainType<VectorPathCommand>
	{
		enum
		{
			id = TID_VectorPathPaint
		};

		enum
		{
			hasDynamicSize = 1
		};

		template<class T, class Processor>
		static void RTTIEnumerateFields(T&& object, Processor& processor)
		{
			processor(object.Type);
			
			switch(object.Type)
			{
			case VectorPathCommandType::Fill:
				processor(object.Fill.StateIndex);
				break;
			case VectorPathCommandType::Stroke:
				processor(object.Stroke.StateIndex);
				break;
			case VectorPathCommandType::SetDrawCursor:
				processor(object.SetDrawCursor.Position);
				break;
			case VectorPathCommandType::SetPathWinding:
				processor(object.SetPathWinding.Winding);
				break;
			case VectorPathCommandType::DrawLineTo:
				processor(object.DrawLineTo.Target);
				break;
			case VectorPathCommandType::DrawArcTo:
				processor(object.DrawArcTo.MiddlePoint);
				processor(object.DrawArcTo.EndPoint);
				processor(object.DrawArcTo.Radius);
				break;
			case VectorPathCommandType::DrawCubicBezierTo:
				processor(object.DrawCubicBezierTo.ControlPoint1);
				processor(object.DrawCubicBezierTo.ControlPoint1);
				processor(object.DrawCubicBezierTo.EndPoint);
				break;
			case VectorPathCommandType::DrawQuadraticBezierTo:
				processor(object.DrawQuadraticBezierTo.ControlPoint);
				processor(object.DrawQuadraticBezierTo.EndPoint);
				break;
			case VectorPathCommandType::DrawRectangle:
				processor(object.DrawRectangle.Area);
				break;
			case VectorPathCommandType::DrawRoundedRectangle:
				processor(object.DrawRoundedRectangle.Area);
				processor(object.DrawRoundedRectangle.RadiusTopLeft);
				processor(object.DrawRoundedRectangle.RadiusTopRight);
				processor(object.DrawRoundedRectangle.RadiusBottomLeft);
				processor(object.DrawRoundedRectangle.RadiusBottomRight);
				break;
			case VectorPathCommandType::DrawEllipse:
				processor(object.DrawEllipse.Origin);
				processor(object.DrawEllipse.Radius);
				break;
			case VectorPathCommandType::DrawArc:
				processor(object.DrawArc.Origin);
				processor(object.DrawArc.Radius);
				processor(object.DrawArc.StartAngle);
				processor(object.DrawArc.EndAngle);
				processor(object.DrawArc.Direction);
				break;
			}
		}

		static BitLength ToMemory(const VectorPathCommand& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr u8 kVersion = 0;

			return B3DRTTIWriteWithSizeHeader(stream, data, compress, [&data, &stream]()
											   {
				BitLength size = 0;
				size += B3DRTTIWrite(kVersion, stream);

				RTTIWriteProcessor writeProcessor(stream);
				RTTIEnumerateFields(data, writeProcessor);

				size += writeProcessor.GetSize();
				return size; });
		}

		static BitLength FromMemory(VectorPathCommand& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			B3DRTTIReadSizeHeader(stream, compress, size);

			uint8_t version;
			B3DRTTIRead(version, stream);
			B3D_ASSERT(version == 0);

			RTTIReadProcessor readProcessor(stream);
			RTTIEnumerateFields(data, readProcessor);

			return size;
		}

		static BitLength GetSize(const VectorPathCommand& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = sizeof(uint8_t);

			RTTISizeProcessor sizeProcessor;
			RTTIEnumerateFields(data, sizeProcessor);

			dataSize += sizeProcessor.GetSize();

			B3DRTTIAddHeaderSize(dataSize, compress);
			return dataSize;
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
