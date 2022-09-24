//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsIconUtility.h"
#include "Image/BsPixelData.h"
#include "Image/BsColor.h"
#include "Error/BsException.h"

#define MSDOS_SIGNATURE 0x5A4D
#define PE_SIGNATURE 0x00004550
#define PE_32BIT_SIGNATURE 0x10B
#define PE_64BIT_SIGNATURE 0x20B
#define PE_NUM_DIRECTORY_ENTRIES 16
#define PE_SECTION_UNINITIALIZED_DATA 0x00000080
#define	PE_IMAGE_DIRECTORY_ENTRY_RESOURCE 2
#define PE_IMAGE_RT_ICON 3

namespace bs
{
	/**	MS-DOS header found at the beggining in a PE format file. */
	struct MSDOSHeader
	{
		UINT16 Signature;
		UINT16 LastSize;
		UINT16 NumBlocks;
		UINT16 NumReloc;
		UINT16 HdrSize;
		UINT16 MinAlloc;
		UINT16 MaxAlloc;
		UINT16 Ss;
		UINT16 Sp;
		UINT16 Checksum;
		UINT16 Ip;
		UINT16 Cs;
		UINT16 RelocPos;
		UINT16 NumOverlay;
		UINT16 Reserved1[4];
		UINT16 OemId;
		UINT16 OemInfo;
		UINT16 Reserved2[10];
		UINT32 Lfanew;
	};

	/**	COFF header found in a PE format file. */
	struct COFFHeader
	{
		UINT16 Machine;
		UINT16 NumSections;
		UINT32 TimeDateStamp;
		UINT32 PtrSymbolTable;
		UINT32 NumSymbols;
		UINT16 SizeOptHeader;
		UINT16 Characteristics;
	};

	/**	Contains address and size of data areas in a PE image. */
	struct PEDataDirectory
	{
		UINT32 VirtualAddress;
		UINT32 Size;
	};

	/**	Optional header in a 32-bit PE format file. */
	struct PEOptionalHeader32
	{
		UINT16 Signature;
		UINT8 MajorLinkerVersion;
		UINT8 MinorLinkerVersion;
		UINT32 SizeCode;
		UINT32 SizeInitializedData;
		UINT32 SizeUninitializedData;
		UINT32 AddressEntryPoint;
		UINT32 BaseCode;
		UINT32 BaseData;
		UINT32 BaseImage;
		UINT32 AlignmentSection;
		UINT32 AlignmentFile;
		UINT16 MajorOsVersion;
		UINT16 MinorOsVersion;
		UINT16 MajorImageVersion;
		UINT16 MinorImageVersion;
		UINT16 MajorSubsystemVersion;
		UINT16 MinorSubsystemVersion;
		UINT32 Reserved;
		UINT32 SizeImage;
		UINT32 SizeHeaders;
		UINT32 Checksum;
		UINT16 Subsystem;
		UINT16 Characteristics;
		UINT32 SizeStackReserve;
		UINT32 SizeStackCommit;
		UINT32 SizeHeapReserve;
		UINT32 SizeHeapCommit;
		UINT32 LoaderFlags;
		UINT32 NumRvaAndSizes;
		PEDataDirectory DataDirectory[16];
	};

	/**	Optional header in a 64-bit PE format file. */
	struct PEOptionalHeader64
	{
		UINT16 Signature;
		UINT8 MajorLinkerVersion;
		UINT8 MinorLinkerVersion;
		UINT32 SizeCode;
		UINT32 SizeInitializedData;
		UINT32 SizeUninitializedData;
		UINT32 AddressEntryPoint;
		UINT32 BaseCode;
		UINT64 BaseImage;
		UINT32 AlignmentSection;
		UINT32 AlignmentFile;
		UINT16 MajorOsVersion;
		UINT16 MinorOsVersion;
		UINT16 MajorImageVersion;
		UINT16 MinorImageVersion;
		UINT16 MajorSubsystemVersion;
		UINT16 MinorSubsystemVersion;
		UINT32 Reserved;
		UINT32 SizeImage;
		UINT32 SizeHeaders;
		UINT32 Checksum;
		UINT16 Subsystem;
		UINT16 Characteristics;
		UINT64 SizeStackReserve;
		UINT64 SizeStackCommit;
		UINT64 SizeHeapReserve;
		UINT64 SizeHeapCommit;
		UINT32 LoaderFlags;
		UINT32 NumRvaAndSizes;
		PEDataDirectory DataDirectory[16];
	};

	/**	A section header in a PE format file. */
	struct PESectionHeader
	{
		char Name[8];
		UINT32 VirtualSize;
		UINT32 RelativeVirtualAddress;
		UINT32 PhysicalSize;
		UINT32 PhysicalAddress;
		UINT8 Deprecated[12];
		UINT32 Flags;
	};

	/**	A resource table header within a .rsrc section in a PE format file. */
	struct PEImageResourceDirectory
	{
		UINT32 Flags;
		UINT32 TimeDateStamp;
		UINT16 MajorVersion;
		UINT16 MinorVersion;
		UINT16 NumNamedEntries;
		UINT16 NumIdEntries;
	};

	/**	A single entry in a resource table within a .rsrc section in a PE format file. */
	struct PEImageResourceEntry
	{
		UINT32 Type;
		UINT32 OffsetDirectory : 31;
		UINT32 IsDirectory : 1;
	};

	/** An entry in a resource table referencing resource data. Found within a .rsrc section in a PE format file. */
	struct PEImageResourceEntryData
	{
		UINT32 OffsetData;
		UINT32 Size;
		UINT32 CodePage;
		UINT32 ResourceHandle;
	};

	/**	Header used in icon file format. */
	struct IconHeader
	{
		UINT32 Size;
		INT32 Width;
		INT32 Height;
		UINT16 Planes;
		UINT16 BitCount;
		UINT32 Compression;
		UINT32 SizeImage;
		INT32 XPelsPerMeter;
		INT32 YPelsPerMeter;
		UINT32 ClrUsed;
		UINT32 ClrImportant;
	};

	void IconUtility::UpdateIconExe(const Path& path, const Map<UINT32, SPtr<PixelData>>& pixelsPerSize)
	{
		// A PE file is structured as such:
		//  - MSDOS Header
		//  - PE Signature
		//  - COFF Header
		//  - PE Optional Header
		//  - One or multiple sections
		//   - .code
		//   - .data
		//   - ...
		//   - .rsrc
		//    - icon/cursor/etc data

		std::fstream stream;
		stream.open(path.ToPlatformString().c_str(), std::ios::in | std::ios::out | std::ios::binary);

		// First check magic number to ensure file is even an executable
		UINT16 magicNum;
		stream.read((char*)&magicNum, sizeof(magicNum));
		if (magicNum != MSDOS_SIGNATURE)
			BS_EXCEPT(InvalidStateException, "Provided file is not a valid executable.");

		// Read the MSDOS header and skip over it
		stream.seekg(0);

		MSDOSHeader msdosHeader;
		stream.read((char*)&msdosHeader, sizeof(MSDOSHeader));

		// Read PE signature
		stream.seekg(msdosHeader.Lfanew);

		UINT32 peSignature;
		stream.read((char*)&peSignature, sizeof(peSignature));

		if (peSignature != PE_SIGNATURE)
			BS_EXCEPT(InvalidStateException, "Provided file is not in PE format.");

		// Read COFF header
		COFFHeader coffHeader;
		stream.read((char*)&coffHeader, sizeof(COFFHeader));

		if (coffHeader.SizeOptHeader == 0) // .exe files always have an optional header
			BS_EXCEPT(InvalidStateException, "Provided file is not a valid executable.");

		UINT32 numSectionHeaders = coffHeader.NumSections;

		// Read optional header
		auto optionalHeaderPos = stream.tellg();

		UINT16 optionalHeaderSignature;
		stream.read((char*)&optionalHeaderSignature, sizeof(optionalHeaderSignature));

		PEDataDirectory* dataDirectory = nullptr;
		stream.seekg(optionalHeaderPos);
		if (optionalHeaderSignature == PE_32BIT_SIGNATURE)
		{
			PEOptionalHeader32 optionalHeader;
			stream.read((char*)&optionalHeader, sizeof(optionalHeader));

			dataDirectory = optionalHeader.DataDirectory + PE_IMAGE_DIRECTORY_ENTRY_RESOURCE;
		}
		else if (optionalHeaderSignature == PE_64BIT_SIGNATURE)
		{
			PEOptionalHeader64 optionalHeader;
			stream.read((char*)&optionalHeader, sizeof(optionalHeader));

			dataDirectory = optionalHeader.DataDirectory + PE_IMAGE_DIRECTORY_ENTRY_RESOURCE;
		}
		else
			BS_EXCEPT(InvalidStateException, "Unrecognized PE format.");

		// Read section headers
		auto sectionHeaderPos = optionalHeaderPos + (std::ifstream::pos_type)coffHeader.SizeOptHeader;
		stream.seekg(sectionHeaderPos);

		PESectionHeader* sectionHeaders = bs_stack_alloc<PESectionHeader>(numSectionHeaders);
		stream.read((char*)sectionHeaders, sizeof(PESectionHeader) * numSectionHeaders);

		// Look for .rsrc section header
		std::function<void(PEImageResourceDirectory*, PEImageResourceDirectory*, UINT8*, UINT32)> setIconData =
			[&](PEImageResourceDirectory* base, PEImageResourceDirectory* current, UINT8* imageData, UINT32 sectionAddress)
		{
			UINT32 numEntries = current->NumIdEntries; // Not supporting name entries
			PEImageResourceEntry* entries = (PEImageResourceEntry*)(current + 1);

			for (UINT32 i = 0; i < numEntries; i++)
			{
				// Only at root does the type identify resource type
				if (base == current && entries[i].Type != PE_IMAGE_RT_ICON)
					continue;

				if (entries[i].IsDirectory)
				{
					PEImageResourceDirectory* child = (PEImageResourceDirectory*)(((UINT8*)base) + entries[i].OffsetDirectory);
					setIconData(base, child, imageData, sectionAddress);
				}
				else
				{
					PEImageResourceEntryData* data = (PEImageResourceEntryData*)(((UINT8*)base) + entries[i].OffsetDirectory);

					UINT8* iconData = imageData + (data->OffsetData - sectionAddress);
					UpdateIconData(iconData, pixelsPerSize);
				}
			}
		};

		for (UINT32 i = 0; i < numSectionHeaders; i++)
		{
			if (sectionHeaders[i].Flags & PE_SECTION_UNINITIALIZED_DATA)
				continue;

			if (strcmp(sectionHeaders[i].Name, ".rsrc") == 0)
			{
				UINT32 imageSize = sectionHeaders[i].PhysicalSize;
				UINT8* imageData = (UINT8*)bs_stack_alloc(imageSize);

				stream.seekg(sectionHeaders[i].PhysicalAddress);
				stream.read((char*)imageData, imageSize);

				UINT32 resourceDirOffset = dataDirectory->VirtualAddress - sectionHeaders[i].RelativeVirtualAddress;
				PEImageResourceDirectory* resourceDirectory = (PEImageResourceDirectory*)&imageData[resourceDirOffset];

				setIconData(resourceDirectory, resourceDirectory, imageData, sectionHeaders[i].RelativeVirtualAddress);
				stream.seekp(sectionHeaders[i].PhysicalAddress);
				stream.write((char*)imageData, imageSize);

				bs_stack_free(imageData);
			}
		}

		bs_stack_free(sectionHeaders);
		stream.close();
	}

	void IconUtility::UpdateIconData(UINT8* iconData, const Map<UINT32, SPtr<PixelData>>& pixelsPerSize)
	{
		IconHeader* iconHeader = (IconHeader*)iconData;

		if (iconHeader->Size != sizeof(IconHeader) || iconHeader->Compression != 0
			|| iconHeader->Planes != 1 || iconHeader->BitCount != 32)
		{
			// Unsupported format
			return;
		}

		UINT8* iconPixels = iconData + sizeof(IconHeader);
		UINT32 width = iconHeader->Width;
		UINT32 height = iconHeader->Height / 2;

		auto iterFind = pixelsPerSize.find(width);
		if (iterFind == pixelsPerSize.end() || iterFind->second->GetWidth() != width
			|| iterFind->second->GetHeight() != height)
		{
			// No icon of this size provided
			return;
		}

		// Write colors
		SPtr<PixelData> srcPixels = iterFind->second;
		UINT32* colorData = (UINT32*)iconPixels;

		UINT32 idx = 0;
		for (INT32 y = (INT32)height - 1; y >= 0; y--)
		{
			for (UINT32 x = 0; x < width; x++)
				colorData[idx++] = srcPixels->GetColorAt(x, y).GetAsBgra();
		}

		// Write AND mask
		UINT32 colorDataSize = width * height * sizeof(UINT32);
		UINT8* maskData = iconPixels + colorDataSize;

		UINT32 numPackedPixels = width / 8; // One per bit in byte

		for (INT32 y = (INT32)height - 1; y >= 0; y--)
		{
			UINT8 mask = 0;
			for (UINT32 packedX = 0; packedX < numPackedPixels; packedX++)
			{
				for (UINT32 pixelIdx = 0; pixelIdx < 8; pixelIdx++)
				{
					UINT32 x = packedX * 8 + pixelIdx;
					Color color = srcPixels->GetColorAt(x, y);
					if (color.A < 0.25f)
						mask |= 1 << (7 - pixelIdx);
				}

				*maskData = mask;
				maskData++;
			}
		}
	}
}
