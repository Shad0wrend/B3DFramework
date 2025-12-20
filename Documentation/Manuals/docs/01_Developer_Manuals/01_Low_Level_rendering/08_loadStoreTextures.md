---
title: Textures
---

This manual covers working with textures at the render-thread level, including load-store textures and direct texture data manipulation.

# Load-store textures

We discussed textures in detail previously, but we haven't yet mentioned load-store textures. These are a type of textures that can be used in a GPU program for arbitrary reads and writes. This makes them different from normal textures, which can only be used for reading or as render targets. They are particularily useful for compute programs as they are (together with load-store buffers) the only ways to output data from such programs.

They are also known as unordered-access textures, storage textures or random write textures. In HLSL these textures have a *RW* prefix, e.g. *RWTexture2D*, and in GLSL they have an *image* prefix, e.g. *image2D*.

Creation of a load-store texture is essentially the same as for normal textures, except for the addition of the @b3d::TU_LOADSTORE usage flag.

~~~~~~~~~~~~~{.cpp}
// Creates a 2D load-store texture, 128x128 with a 4-component 32-bit floating point format
TextureCreateInformation createInformation;
createInformation.Type = TEX_TYPE_2D;
createInformation.Width = 128;
createInformation.Height = 128;
createInformation.Format = PF_RGBA32F;
createInformation.Usage = TU_LOADSTORE;

SPtr<GpuDevice> gpuDevice = ...;
SPtr<Texture> texture = gpuDevice->CreateTexture(createInformation);
~~~~~~~~~~~~~

You can then bind a load-store texture to a GPU program by calling @b3d::render::GpuParameterSet::SetStorageTexture as was described in an earlier chapter.

~~~~~~~~~~~~~{.cpp}
SPtr<GpuParameterSet> parameterSet = ...;
SPtr<Texture> texture = ...;

TextureSurface surface = TextureSurface::kComplete;
parameterSet->SetStorageTexture("myLoadStoreTex", texture, surface);
~~~~~~~~~~~~~

Load-store textures do not support sampling using sampler states, you can only read-write their pixels directly. They also do not support mip-maps, and if your texture has multiple mip-maps you must provide a @b3d::TextureSurface struct to **render::GpuParameterSet::SetStorageTexture()** in order to specify which mip-level to bind (by default it is the first).

~~~~~~~~~~~~~{.cpp}
SPtr<GpuParameterSet> parameterSet = ...;
SPtr<Texture> texture = ...;

TextureSurface surface;
surface.MipLevel = 5; // Bind 5th mip-level for load-store operations
parameterSet->SetStorageTexture("myLoadStoreTex", texture, surface);
~~~~~~~~~~~~~

Load-store textures can also be bound as normal textures, for read-only operations like sampling. Note that they cannot be bound for both operations at once. Also note that load-store textures are not supported for 3D textures, and have limited support (depending on the rendering backend) for multisampled surfaces.

# Reading and writing texture data

For reading and writing texture data on the render thread, use the @b3d::render::TextureUtility class which provides static helper methods.

## Writing data

To write pixel data to a texture subresource:

~~~~~~~~~~~~~{.cpp}
SPtr<render::Texture> texture = ...;
PixelData pixelData = ...;

// Write data to mip level 0, array layer 0
render::TextureUtility::Write(texture, pixelData);

// Write to specific mip level and array layer
render::TextureUtility::Write(texture, pixelData, 2, 0); // mip 2, layer 0

// Use staging buffer via a command buffer for non-mappable textures
SPtr<GpuCommandBuffer> commandBuffer = ...;
render::TextureUtility::Write(texture, pixelData, 0, 0, TextureWriteFlag::Normal, commandBuffer);
~~~~~~~~~~~~~

**TextureUtility::Write()** automatically chooses the optimal path:
- For directly mappable textures (TU_DYNAMIC with LINEAR tiling): Uses **Map()** + **BulkPixelConversion**
- For non-mappable textures: Uses staging buffer + **CopyBufferToTexture**

## Reading data

To read pixel data from a texture subresource:

~~~~~~~~~~~~~{.cpp}
SPtr<render::Texture> texture = ...;
PixelData destination = texture->GetProperties().AllocBuffer(0, 0);

// Blocking read - waits for GPU to finish if texture is in use
render::TextureUtility::Read(texture, destination);

// Read from specific mip level and array layer
render::TextureUtility::Read(texture, destination, 2, 0); // mip 2, layer 0
~~~~~~~~~~~~~

For non-blocking reads that integrate with your rendering pipeline:

~~~~~~~~~~~~~{.cpp}
SPtr<render::Texture> texture = ...;
SPtr<GpuCommandBuffer> commandBuffer = ...;

// Queue async read operation
TAsyncOp<SPtr<PixelData>> asyncOp = render::TextureUtility::ReadAsync(texture, *commandBuffer, 0, 0);

// ... submit command buffer and continue other work ...

// Later, check if complete and get result
if (asyncOp.HasCompleted())
{
	SPtr<PixelData> result = asyncOp.GetResult();
	// Use the pixel data
}
~~~~~~~~~~~~~

## Direct memory mapping

For textures that support direct mapping (TU_DYNAMIC textures with LINEAR tiling), you can use @b3d::render::Texture::Map for direct CPU access:

~~~~~~~~~~~~~{.cpp}
SPtr<render::Texture> texture = ...; // Must be TU_DYNAMIC

// Map returns RAII scope that auto-flushes on destruction
{
	render::GpuTextureMappedScope scope = texture->Map(0, 0, GpuMapOption::Write);
	if (scope.IsValid())
	{
		PixelData& pixelData = scope.GetPixelData();
		// Write directly to pixelData
		pixelData.SetColorAt(0, 0, Color::kRed);
	}
} // Automatically flushes when scope exits
~~~~~~~~~~~~~

## Buffer-texture copies

For explicit control over buffer-to-texture transfers, use the command buffer methods:

~~~~~~~~~~~~~{.cpp}
SPtr<GpuCommandBuffer> commandBuffer = ...;
SPtr<GpuBuffer> stagingBuffer = ...;
SPtr<render::Texture> texture = ...;

// Copy from buffer to texture
commandBuffer->CopyBufferToTexture(stagingBuffer, texture, 0, 0, 0); // buffer offset, face, mip

// Copy from texture to buffer
commandBuffer->CopyTextureToBuffer(texture, stagingBuffer, 0, 0, 0); // face, mip, buffer offset
~~~~~~~~~~~~~

## Clearing textures

To clear all pixels of a texture subresource to a specific color:

~~~~~~~~~~~~~{.cpp}
SPtr<render::Texture> texture = ...;

// Clear to black
render::TextureUtility::Clear(texture, Color::kBlack);

// Clear specific mip level and array layer
render::TextureUtility::Clear(texture, Color::kBlue, 2, 0);
~~~~~~~~~~~~~
