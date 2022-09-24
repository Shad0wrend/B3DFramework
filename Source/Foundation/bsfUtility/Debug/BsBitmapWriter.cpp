//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Debug/BsBitmapWriter.h"

namespace bs
{
#pragma pack(push, 2) // Align to 2byte boundary so we don't get extra 2 bytes for this struct

	struct BMP_HEADER
	{
		UINT16 BM;
		UINT32 SizeOfFile;
		UINT32 Reserve;
		UINT32 OffsetOfPixelData;
		UINT32 SizeOfHeader;
		UINT32 Width;
		UINT32 Hight;
		UINT16 NumOfColorPlane;
		UINT16 NumOfBitPerPix;
		UINT32 Compression;
		UINT32 SizeOfPixData;
		UINT32 HResolution;
		UINT32 VResolution;
		UINT32 NumOfColorInPalette;
		UINT32 ImportantColors;

	};

#pragma pack(pop)

	void BitmapWriter::RawPixelsToBmp(const UINT8* input, UINT8* output, UINT32 width, UINT32 height, UINT32 bytesPerPixel)
	{
		UINT16 bmpBytesPerPixel = 3;
		if(bytesPerPixel >= 4)
			bmpBytesPerPixel = 4;

		UINT32 padding = (width * bmpBytesPerPixel) % 4;
		if(padding != 0)
			padding = 4 - padding;

		UINT32 rowPitch = (width * bmpBytesPerPixel) + padding;
		UINT32 dataSize = height * rowPitch;

		BMP_HEADER header;
		header.BM = 0x4d42;
		header.SizeOfFile =  sizeof(header) + dataSize;
		header.Reserve = 0000;
		header.OffsetOfPixelData = 54;
		header.SizeOfHeader = 40;
		header.Width = width;
		header.Hight = height;
		header.NumOfColorPlane = 1;
		header.NumOfBitPerPix = bmpBytesPerPixel * 8;
		header.Compression = 0;
		header.SizeOfPixData = dataSize;
		header.HResolution = 2835;
		header.VResolution = 2835;
		header.NumOfColorInPalette = 0;
		header.ImportantColors = 0;

		// Write header
		memcpy(output, &header, sizeof(header));
		output += sizeof(header);

		// Write bytes
		UINT32 widthBytes = width * bytesPerPixel;
		
		// BPP matches so we can just copy directly
		if(bmpBytesPerPixel == bytesPerPixel)
		{
			for(INT32 y = height - 1; y >= 0 ; y--)
			{
				UINT8* outputPtr = output + y * rowPitch;

				memcpy(outputPtr, input, widthBytes);
				memset(outputPtr + widthBytes, 0, padding);

				input += widthBytes;
			}
		}
		else if(bmpBytesPerPixel < bytesPerPixel) // More bytes in source than supported in BMP, just truncate excess data
		{
			for(INT32 y = height - 1; y >= 0 ; y--)
			{
				UINT8* outputPtr = output + y * rowPitch;

				for(UINT32 x = 0; x < width; x++)
				{
					memcpy(outputPtr, input, bmpBytesPerPixel);
					outputPtr += bmpBytesPerPixel;
					input += bytesPerPixel;
				}

				memset(outputPtr, 0, padding);
			}
		}
		else // More bytes in BMP than in source (BMP must be 24bit minimum)
		{
			for(INT32 y = height - 1; y >= 0 ; y--)
			{
				UINT8* outputPtr = output + y * rowPitch;

				for(UINT32 x = 0; x < width; x++)
				{
					memcpy(outputPtr, input, bytesPerPixel);

					// Fill the empty bytes with the last available byte from input
					UINT32 remainingBytes = bmpBytesPerPixel - bytesPerPixel;
					while(remainingBytes > 0)
					{
						memcpy(outputPtr + (bmpBytesPerPixel - remainingBytes), input, 1);
						remainingBytes--;
					}

					outputPtr += bmpBytesPerPixel;
					input += bytesPerPixel;
				}

				memset(outputPtr, 0, padding);
			}
		}
	}

	UINT32 BitmapWriter::GetBmpSize(UINT32 width, UINT32 height, UINT32 bytesPerPixel)
	{
		UINT16 bmpBytesPerPixel = 3;
		if(bytesPerPixel >= 4)
			bmpBytesPerPixel = 4;

		UINT32 padding = (width * bmpBytesPerPixel) % 4;
		if(padding != 0)
			padding = 4 - padding;

		UINT32 rowPitch = (width * bmpBytesPerPixel) + padding;
		UINT32 dataSize = height * rowPitch;

		return sizeof(BMP_HEADER) + dataSize;
	}
}
