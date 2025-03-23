//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Image/BsSpriteImage.h"
#include "VectorGraphics/BsVectorGraphics.h"
#include "VectorGraphics/BsVectorSpriteAtlas.h"

namespace bs
{
	class GUIVectorSpriteAtlasAllocation;
	/** @addtogroup Image
	 *  @{
	 */

	/** Descriptor structure used for initialization of a SpriteVectorPath. */
	struct B3D_SCRIPT_EXPORT(ExportAsStruct(true), DocumentationGroup(Rendering)) SpriteVectorPathCreateInformation : SpriteImageInformation
	{
		SpriteVectorPathCreateInformation() = default;
		SpriteVectorPathCreateInformation(const SpriteImageInformation& spriteImageInformation, const HVectorPath& vectorPath, const Size2I& defaultSize)
			: SpriteImageInformation(spriteImageInformation), VectorPath(vectorPath), DefaultSize(defaultSize)
		{ }

		HVectorPath VectorPath; /**< Vector path to render on the sprite. */
		Size2I DefaultSize; /**< Size of the unscaled rasterized path, in pixels. Actual rendered size might be different depending on DPI scale or other scale factors. */
		VectorGraphicsRasterizationScaling ScalingMode = VectorGraphicsRasterizationScaling::StretchToFit; /**< How to scale the path canvas onto the rasterized destination. */
	};

	/** Provides information about a particular sprite vector path image allocated within a texture atlas. */
	template<bool IsRenderProxy>
	struct TSpriteVectorPathAllocation : CoreVariantType<SpriteImageAllocation, IsRenderProxy>
	{
		using SpriteImageType = CoreVariantType<SpriteImage, IsRenderProxy>;
		using TextureType = CoreVariantHandleType<Texture, IsRenderProxy>;

		using CoreVariantType<SpriteImageAllocation, IsRenderProxy>::SpriteImageAllocation;

	private:
		friend class SpriteVectorPath;

	};

	/** @copydoc TSpriteVectorPathAllocation. */
	struct SpriteVectorPathAllocation : TSpriteVectorPathAllocation<false>
	{
		using TSpriteVectorPathAllocation::TSpriteVectorPathAllocation;

		SpriteVectorPathAllocation(const WeakSPtr<SpriteImageType>& owner, const SPtr<GUIVectorSpriteAtlasAllocation>& vectorSpriteAtlasAllocation)
			:TSpriteVectorPathAllocation(owner, vectorSpriteAtlasAllocation ? vectorSpriteAtlasAllocation->AtlasTexture : nullptr, vectorSpriteAtlasAllocation ? vectorSpriteAtlasAllocation->UVRange : Area2(0.0f, 0.0f, 1.0f, 1.0f)), mVectorSpriteAtlasAllocation(vectorSpriteAtlasAllocation)
		{ }


	private:
		/** Allocation handle in the vector path atlas. */
		SPtr<GUIVectorSpriteAtlasAllocation> mVectorSpriteAtlasAllocation; // Note: This is kept on the main thread only, but is deallocated with a one frame delay, so its important the render proxy doesn't attempt to use this allocation any later. We might want a better system down the line.
	};

	/** @} */

	namespace ct
	{
		/** @addtogroup RenderThread
		 *  @{
		 */

		/**
		 * Render proxy counterpart of a bs::SpriteVectorPathCreateInformation.
		 *
		 * @note	Render thread.
		 */
		struct SpriteVectorPathCreateInformation : SpriteImageInformation
		{
			SpriteVectorPathCreateInformation() = default;
			SpriteVectorPathCreateInformation(const SpriteImageInformation& spriteImageInformation, const SPtr<Texture>& atlasTexture)
				: SpriteImageInformation(spriteImageInformation), AtlasTexture(atlasTexture)
			{ }

			/** Texture used as the atlas. */
			SPtr<Texture> AtlasTexture;
		};

		/** @} */
	} // namespace ct

	/** @addtogroup Image
	 *  @{
	 */

	/** Implementation of SpriteImage that renders a vector path. */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Rendering)) SpriteVectorPath : public CoreVariantType<SpriteImage, false>
	{
	public:
		SPtr<SpriteImageAllocation> FindOrAllocateImageToFitArea(const Size2I& size) override;
		SPtr<SpriteImageAllocation> FindOrAllocateScaledImage(float scale) override;

		/**	Creates a new sprite vector path. */
		B3D_SCRIPT_EXPORT(ExtensionConstructorForType(SpriteVectorPath))
		static HSpriteVectorPath Create(const HVectorPath& vectorPath, const Size2I& defaultSize);

		/**	Creates a new sprite vector path. */
		B3D_SCRIPT_EXPORT(ExtensionConstructorForType(SpriteVectorPath))
		static HSpriteVectorPath Create(const SpriteVectorPathCreateInformation& createInformation);

		/** Creates a new SpriteVectorPath without a resource handle. Use Create() for normal use. */
		static SPtr<SpriteVectorPath> CreateShared(const HVectorPath& vectorPath, const Size2I& defaultSize);

		/** Creates a new SpriteVectorPath without a resource handle. Use Create() for normal use. */
		static SPtr<SpriteVectorPath> CreateShared(const SpriteVectorPathCreateInformation& createInformation);

	private:
		friend class SpriteVectorPathRTTI;
		friend class ct::SpriteVectorPath;
		struct SyncPacket;

		SpriteVectorPath(const SpriteVectorPathCreateInformation& createInformation);

		/** Allocates a sprite image using the provided size. */
		SPtr<SpriteVectorPathAllocation> AllocateImage(const Size2I& size);

		void Initialize() override;
		SPtr<ct::RenderProxy> CreateRenderProxy() const override;
		RenderProxySyncPacket* CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags) override;
		void GetCoreDependencies(Vector<CoreObject*>& dependencies) override;

		HVectorPath mVectorPath;
		Size2I mDefaultSize{BsZero};
		VectorGraphicsRasterizationScaling mScalingMode = VectorGraphicsRasterizationScaling::StretchToFit;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/

		/**	Creates a new empty and uninitialized sprite vector path. */
		static SPtr<SpriteVectorPath> CreateEmpty();

	public:
		friend class SpriteVectorPathRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;
	};

	/** @} */

	namespace ct
	{
		/** @addtogroup RenderThread
		 *  @{
		 */

		/** @copydoc TSpriteVectorPathAllocation. */
		struct SpriteVectorPathAllocation : TSpriteVectorPathAllocation<true>
		{
			using TSpriteVectorPathAllocation::TSpriteVectorPathAllocation;
		};

		/**
		 * Render proxy counterpart of a bs::SpriteVectorPath.
		 *
		 * @note	Render thread.
		 */
		class B3D_CORE_EXPORT SpriteVectorPath : public CoreVariantType<SpriteImage, true>
		{
		public:
		private:
			friend class bs::SpriteVectorPath;

			SpriteVectorPath(const SpriteVectorPathCreateInformation& createInformation);

			void SyncFromCoreObject(const CoreSyncData& data, FrameAllocator& allocator) override;
		};

		/** @} */
	} // namespace ct
} // namespace bs
