//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Image/BsSpriteImage.h"
#include "VectorGraphics/BsVectorGraphics.h"

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
		SpriteVectorPathCreateInformation(const SpriteImageInformation& spriteImageInformation, const HVectorPath& vectorPath, const Size2UI& size)
			: SpriteImageInformation(spriteImageInformation), VectorPath(vectorPath), Size(size)
		{ }

		HVectorPath VectorPath; /**< Vector path to render on the sprite. */
		Size2UI Size; /**< Size of the rasterized path, in pixels. */
		VectorGraphicsRasterizationScaling ScalingMode = VectorGraphicsRasterizationScaling::StretchToFit; /**< How to scale the path canvas onto the rasterized destination. */
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
		/**	Sets the vector path to render. */
		B3D_SCRIPT_EXPORT()
		void SetVectorPath(const HVectorPath& vectorPath);

		/**	Creates a new sprite vector path. */
		B3D_SCRIPT_EXPORT(ExtensionConstructorForType(SpriteVectorPath))
		static HSpriteVectorPath Create(const HVectorPath& vectorPath, const Size2UI& size);

		/**	Creates a new sprite vector path. */
		B3D_SCRIPT_EXPORT(ExtensionConstructorForType(SpriteVectorPath))
		static HSpriteVectorPath Create(const SpriteVectorPathCreateInformation& createInformation);

		/** Creates a new SpriteVectorPath without a resource handle. Use Create() for normal use. */
		static SPtr<SpriteVectorPath> CreateShared(const HVectorPath& vectorPath, const Size2UI& size);

		/** Creates a new SpriteVectorPath without a resource handle. Use Create() for normal use. */
		static SPtr<SpriteVectorPath> CreateShared(const SpriteVectorPathCreateInformation& createInformation);

	private:
		friend class SpriteVectorPathRTTI;
		friend class ct::SpriteVectorPath;
		struct SyncPacket;

		SpriteVectorPath(const SpriteVectorPathCreateInformation& createInformation);

		void Initialize() override;
		SPtr<ct::RenderProxy> CreateRenderProxy() const override;
		RenderProxySyncPacket* CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags) override;
		void GetCoreDependencies(Vector<CoreObject*>& dependencies) override;

		/** Renders the vector path into the atlas texture. Should be called any time after the vector path changes. */
		void RenderVectorPath();

		HVectorPath mVectorPath;
		Size2UI mSize;
		VectorGraphicsRasterizationScaling mScalingMode = VectorGraphicsRasterizationScaling::StretchToFit;
		SPtr<GUIVectorSpriteAtlasAllocation> mSpriteAtlasAllocation;

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
