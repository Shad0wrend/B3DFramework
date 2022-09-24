//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsRenderTexture.h"
#include "Error/BsException.h"
#include "Image/BsTexture.h"
#include "Managers/BsTextureManager.h"
#include "Resources/BsResources.h"
#include "CoreThread/BsCoreThread.h"
#include <Private/RTTI/BsRenderTargetRTTI.h>

namespace bs
{
	RenderTextureProperties::RenderTextureProperties(const RENDER_TEXTURE_DESC& desc, bool requiresFlipping)
	{
		UINT32 firstIdx = (UINT32)-1;
		bool requiresHwGamma = false;
		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			HTexture texture = desc.ColorSurfaces[i].Texture;

			if (!texture.IsLoaded())
				continue;

			if (firstIdx == (UINT32)-1)
				firstIdx = i;

			requiresHwGamma |= texture->GetProperties().IsHardwareGammaEnabled();
		}

		if (firstIdx == (UINT32)-1)
		{
			HTexture texture = desc.DepthStencilSurface.Texture;
			if (texture.IsLoaded())
			{
				const TextureProperties& texProps = texture->GetProperties();
				Construct(&texProps, desc.DepthStencilSurface.NumFaces, desc.DepthStencilSurface.MipLevel,
						  requiresFlipping, false);
			}
		}
		else
		{
			HTexture texture = desc.ColorSurfaces[firstIdx].Texture;

			const TextureProperties& texProps = texture->GetProperties();
			Construct(&texProps, desc.ColorSurfaces[firstIdx].NumFaces, desc.ColorSurfaces[firstIdx].MipLevel,
					  requiresFlipping, requiresHwGamma);
		}
	}

	RenderTextureProperties::RenderTextureProperties(const ct::RENDER_TEXTURE_DESC& desc, bool requiresFlipping)
	{
		UINT32 firstIdx = (UINT32)-1;
		bool requiresHwGamma = false;
		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			SPtr<ct::Texture> texture = desc.ColorSurfaces[i].Texture;

			if (texture == nullptr)
				continue;

			if (firstIdx == (UINT32)-1)
				firstIdx = i;

			requiresHwGamma |= texture->GetProperties().IsHardwareGammaEnabled();
		}

		if(firstIdx == (UINT32)-1)
		{
			SPtr<ct::Texture> texture = desc.DepthStencilSurface.Texture;
			if(texture != nullptr)
			{
				const TextureProperties& texProps = texture->GetProperties();
				Construct(&texProps, desc.DepthStencilSurface.NumFaces, desc.DepthStencilSurface.MipLevel,
						  requiresFlipping, false);
			}
		}
		else
		{
			SPtr<ct::Texture> texture = desc.ColorSurfaces[firstIdx].Texture;

			const TextureProperties& texProps = texture->GetProperties();
			Construct(&texProps, desc.ColorSurfaces[firstIdx].NumFaces, desc.ColorSurfaces[firstIdx].MipLevel,
					  requiresFlipping, requiresHwGamma);
		}
	}

	void RenderTextureProperties::Construct(const TextureProperties* textureProps, UINT32 numSlices,
											UINT32 mipLevel, bool requiresFlipping, bool hwGamma)
	{
		if (textureProps != nullptr)
		{
			PixelUtil::GetSizeForMipLevel(textureProps->GetWidth(), textureProps->GetHeight(), textureProps->GetDepth(),
										  mipLevel, Width, Height, numSlices);

			numSlices *= numSlices;
			MultisampleCount = textureProps->GetNumSamples();
		}

		IsWindow = false;
		RequiresTextureFlipping = requiresFlipping;
		this->HwGamma = hwGamma;
	}

	SPtr<RenderTexture> RenderTexture::Create(const TEXTURE_DESC& desc,
		bool createDepth, PixelFormat depthStencilFormat)
	{
		return TextureManager::Instance().CreateRenderTexture(desc, createDepth, depthStencilFormat);
	}

	SPtr<RenderTexture> RenderTexture::Create(const RENDER_TEXTURE_DESC& desc)
	{
		return TextureManager::Instance().CreateRenderTexture(desc);
	}

	SPtr<ct::RenderTexture> RenderTexture::GetCore() const
	{
		return std::static_pointer_cast<ct::RenderTexture>(mCoreSpecific);
	}

	RenderTexture::RenderTexture(const RENDER_TEXTURE_DESC& desc)
		:mDesc(desc)
	{
		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			if (desc.ColorSurfaces[i].Texture != nullptr)
				mBindableColorTex[i] = desc.ColorSurfaces[i].Texture;
		}

		if (desc.DepthStencilSurface.Texture != nullptr)
			mBindableDepthStencilTex = desc.DepthStencilSurface.Texture;
	}

	SPtr<ct::CoreObject> RenderTexture::CreateCore() const
	{
		ct::RENDER_TEXTURE_DESC coreDesc;

		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			ct::RENDER_SURFACE_DESC surfaceDesc;
			if (mDesc.ColorSurfaces[i].Texture.IsLoaded())
				surfaceDesc.Texture = mDesc.ColorSurfaces[i].Texture->GetCore();

			surfaceDesc.Face = mDesc.ColorSurfaces[i].Face;
			surfaceDesc.NumFaces = mDesc.ColorSurfaces[i].NumFaces;
			surfaceDesc.MipLevel = mDesc.ColorSurfaces[i].MipLevel;

			coreDesc.ColorSurfaces[i] = surfaceDesc;
		}

		if (mDesc.DepthStencilSurface.Texture.IsLoaded())
			coreDesc.DepthStencilSurface.Texture = mDesc.DepthStencilSurface.Texture->GetCore();

		coreDesc.DepthStencilSurface.Face = mDesc.DepthStencilSurface.Face;
		coreDesc.DepthStencilSurface.NumFaces = mDesc.DepthStencilSurface.NumFaces;
		coreDesc.DepthStencilSurface.MipLevel = mDesc.DepthStencilSurface.MipLevel;

		return ct::TextureManager::Instance().CreateRenderTextureInternal(coreDesc);
	}

	CoreSyncData RenderTexture::SyncToCore(FrameAlloc* allocator)
	{
		UINT32 size = sizeof(RenderTextureProperties);
		UINT8* buffer = allocator->Alloc(size);

		RenderTextureProperties& props = const_cast<RenderTextureProperties&>(GetProperties());

		memcpy(buffer, (void*)&props, size);
		return CoreSyncData(buffer, size);
	}

	const RenderTextureProperties& RenderTexture::GetProperties() const
	{
		return static_cast<const RenderTextureProperties&>(GetPropertiesInternal());
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* RenderTexture::GetRttiStatic()
	{
		return RenderTextureRTTI::Instance();
	}

	RTTITypeBase* RenderTexture::GetRtti() const
	{
		return RenderTexture::GetRttiStatic();
	}

	namespace ct
	{
	RenderTexture::RenderTexture(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx)
		:mDesc(desc)
	{ }

	void RenderTexture::Initialize()
	{
		RenderTarget::Initialize();

		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			if (mDesc.ColorSurfaces[i].Texture != nullptr)
			{
				SPtr<Texture> texture = mDesc.ColorSurfaces[i].Texture;

				if ((texture->GetProperties().GetUsage() & TU_RENDERTARGET) == 0)
					BS_EXCEPT(InvalidParametersException, "Provided texture is not created with render target usage.");

				mColorSurfaces[i] = texture->RequestView(mDesc.ColorSurfaces[i].MipLevel, 1,
					mDesc.ColorSurfaces[i].Face, mDesc.ColorSurfaces[i].NumFaces, GVU_RENDERTARGET);
			}
		}

		if (mDesc.DepthStencilSurface.Texture != nullptr)
		{
			SPtr<Texture> texture = mDesc.DepthStencilSurface.Texture;

			if ((texture->GetProperties().GetUsage() & TU_DEPTHSTENCIL) == 0)
				BS_EXCEPT(InvalidParametersException, "Provided texture is not created with depth stencil usage.");

			mDepthStencilSurface = texture->RequestView(mDesc.DepthStencilSurface.MipLevel, 1,
				mDesc.DepthStencilSurface.Face, mDesc.DepthStencilSurface.NumFaces, GVU_DEPTHSTENCIL);
		}

		ThrowIfBuffersDontMatch();
	}

	SPtr<RenderTexture> RenderTexture::Create(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx)
	{
		return TextureManager::Instance().CreateRenderTexture(desc, deviceIdx);
	}

	void RenderTexture::SyncToCore(const CoreSyncData& data)
	{
		RenderTextureProperties& props = const_cast<RenderTextureProperties&>(GetProperties());
		props = data.GetData<RenderTextureProperties>();
	}

	const RenderTextureProperties& RenderTexture::GetProperties() const
	{
		return static_cast<const RenderTextureProperties&>(GetPropertiesInternal());
	}

	void RenderTexture::ThrowIfBuffersDontMatch() const
	{
		UINT32 firstSurfaceIdx = (UINT32)-1;
		for (UINT32 i = 0; i < BS_MAX_MULTIPLE_RENDER_TARGETS; i++)
		{
			if (mColorSurfaces[i] == nullptr)
				continue;

			if (firstSurfaceIdx == (UINT32)-1)
			{
				firstSurfaceIdx = i;
				continue;
			}

			const TextureProperties& curTexProps = mDesc.ColorSurfaces[i].Texture->GetProperties();
			const TextureProperties& firstTexProps = mDesc.ColorSurfaces[firstSurfaceIdx].Texture->GetProperties();

			UINT32 curMsCount = curTexProps.GetNumSamples();
			UINT32 firstMsCount = firstTexProps.GetNumSamples();

			UINT32 curNumSlices = mColorSurfaces[i]->GetNumArraySlices();
			UINT32 firstNumSlices = mColorSurfaces[firstSurfaceIdx]->GetNumArraySlices();

			if (curMsCount == 0)
				curMsCount = 1;

			if (firstMsCount == 0)
				firstMsCount = 1;

			if (curTexProps.GetWidth() != firstTexProps.GetWidth() ||
				curTexProps.GetHeight() != firstTexProps.GetHeight() ||
				curTexProps.GetDepth() != firstTexProps.GetDepth() ||
				curMsCount != firstMsCount ||
				curNumSlices != firstNumSlices)
			{
				String errorInfo = "\nWidth: " + toString(curTexProps.GetWidth()) + "/" + toString(firstTexProps.GetWidth());
				errorInfo += "\nHeight: " + toString(curTexProps.GetHeight()) + "/" + toString(firstTexProps.GetHeight());
				errorInfo += "\nDepth: " + toString(curTexProps.GetDepth()) + "/" + toString(firstTexProps.GetDepth());
				errorInfo += "\nNum. slices: " + toString(curNumSlices) + "/" + toString(firstNumSlices);
				errorInfo += "\nMultisample Count: " + toString(curMsCount) + "/" + toString(firstMsCount);

				BS_EXCEPT(InvalidParametersException, "Provided color textures don't match!" + errorInfo);
			}
		}

		if (firstSurfaceIdx != (UINT32)-1)
		{
			const TextureProperties& firstTexProps = mDesc.ColorSurfaces[firstSurfaceIdx].Texture->GetProperties();
			SPtr<TextureView> firstSurfaceView = mColorSurfaces[firstSurfaceIdx];

			UINT32 numSlices;
			if (firstTexProps.GetTextureType() == TEX_TYPE_3D)
				numSlices = firstTexProps.GetDepth();
			else
				numSlices = firstTexProps.GetNumFaces();

			if ((firstSurfaceView->GetFirstArraySlice() + firstSurfaceView->GetNumArraySlices()) > numSlices)
			{
				BS_EXCEPT(InvalidParametersException, "Provided number of faces is out of range. Face: " +
					toString(firstSurfaceView->GetFirstArraySlice() + firstSurfaceView->GetNumArraySlices()) + ". Max num faces: " + toString(numSlices));
			}

			if (firstSurfaceView->GetMostDetailedMip() > firstTexProps.GetNumMipmaps())
			{
				BS_EXCEPT(InvalidParametersException, "Provided number of mip maps is out of range. Mip level: " +
					toString(firstSurfaceView->GetMostDetailedMip()) + ". Max num mipmaps: " + toString(firstTexProps.GetNumMipmaps()));
			}

			if (mDepthStencilSurface == nullptr)
				return;

			const TextureProperties& depthTexProps = mDesc.DepthStencilSurface.Texture->GetProperties();
			UINT32 depthMsCount = depthTexProps.GetNumSamples();
			UINT32 colorMsCount = firstTexProps.GetNumSamples();

			if (depthMsCount == 0)
				depthMsCount = 1;

			if (colorMsCount == 0)
				colorMsCount = 1;

			if (depthTexProps.GetWidth() != firstTexProps.GetWidth() ||
				depthTexProps.GetHeight() != firstTexProps.GetHeight() ||
				depthMsCount != colorMsCount)
			{
				String errorInfo = "\nWidth: " + toString(depthTexProps.GetWidth()) + "/" + toString(firstTexProps.GetWidth());
				errorInfo += "\nHeight: " + toString(depthTexProps.GetHeight()) + "/" + toString(firstTexProps.GetHeight());
				errorInfo += "\nMultisample Count: " + toString(depthMsCount) + "/" + toString(colorMsCount);

				BS_EXCEPT(InvalidParametersException, "Provided texture and depth stencil buffer don't match!" + errorInfo);
			}
		}
	}
	}
}
