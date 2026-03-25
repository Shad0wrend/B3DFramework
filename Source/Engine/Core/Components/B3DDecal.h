//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "Scene/B3DComponent.h"
#include "Math/B3DTransform.h"
#include "CoreObject/B3DCoreObject.h"
#include "Renderer/B3DRendererId.h"
#include "Renderer/B3DRendererObjectStorage.h"

namespace b3d
{
	class DecalObjectStorageBase;
	struct DecalFullUpdateChannel;
	struct DecalTransformUpdateChannel;

	/** @addtogroup Implementation
	 *  @{
	 */

	/** Shared decal property data used by both main and render thread variants. */
	template <bool IsRenderProxy>
	struct B3D_EXPORT TDecalData
	{
		using MaterialType = CoreVariantHandleType<Material, IsRenderProxy>;

		MaterialType Material;
		Vector2 Size = Vector2::kOne;
		float MaxDistance = 10.0f;
		u64 Layer = 1;
		u32 LayerMask = 0xFFFFFFFF;
	};

	/** CRTP getter interface providing shared read access for decal data to both Decal and render::DecalProxy. */
	template <typename Derived, bool IsRenderProxy>
	class TDecalGetters
	{
		using MaterialType = CoreVariantHandleType<Material, IsRenderProxy>;

	public:
		/** Width and height of the decal. */
		B3D_SCRIPT_EXPORT(ExportName(Size), Property(Getter))
		Vector2 GetSize() const { return GetData().Size; }

		/** Determines the material to use when rendering the decal. */
		B3D_SCRIPT_EXPORT(ExportName(Material), Property(Getter))
		const MaterialType& GetMaterial() const { return GetData().Material; }

		/** Determines the maximum distance (from its origin) at which the decal is displayed. */
		B3D_SCRIPT_EXPORT(ExportName(MaxDistance), Property(Getter))
		float GetMaxDistance() const { return GetData().MaxDistance; }

		/**
		 * Bitfield that allows you to mask on which objects will the decal be projected onto. Only objects with the
		 * matching layers will be projected onto. Note that decal layer mask only supports 32-bits and objects with
		 * layers in bits >= 32 will always be projected onto.
		 */
		B3D_SCRIPT_EXPORT(ExportName(LayerMask), Property(Getter))
		u32 GetLayerMask() const { return GetData().LayerMask; }

		/**
		 * Determines the layer that controls whether a system is considered visible in a specific camera. Layer must match
		 * camera layer bitfield in order for the camera to render the decal.
		 */
		B3D_SCRIPT_EXPORT(ExportName(Layer), Property(Getter))
		u64 GetLayer() const { return GetData().Layer; }

	private:
		const TDecalData<IsRenderProxy>& GetData() const
		{
			return static_cast<const Derived*>(this)->GetDecalData();
		}
	};

	/** Computes world-space bounds for a decal. */
	Bounds ComputeDecalBounds(const Vector2& size, float maxDistance, const Matrix4& worldTransform);

	/** Common code used both by main and render thread variants of Decal. */
	template <bool IsRenderProxy>
	class B3D_EXPORT TDecal : public CoreVariantType<CoreObject, IsRenderProxy>,
		public TDecalData<IsRenderProxy>,
		public TDecalGetters<TDecal<IsRenderProxy>, IsRenderProxy>
	{
	public:
		using typename TDecalData<IsRenderProxy>::MaterialType;
		using SceneInstanceType = CoreVariantType<SceneInstance, IsRenderProxy>;
		using Super = CoreVariantType<CoreObject, IsRenderProxy>;

		friend class TDecalGetters<TDecal<IsRenderProxy>, IsRenderProxy>;

		TDecal();
		virtual ~TDecal() = default;

		/** @copydoc TDecalGetters::GetSize */
		B3D_SCRIPT_EXPORT(ExportName(Size), Property(Setter))
		void SetSize(const Vector2& size)
		{
			this->Size = Vector2::Max(Vector2::kZero, size);
			MarkRenderProxyDataDirty();
			UpdateBounds();
		}

		/** @copydoc TDecalGetters::GetMaterial */
		B3D_SCRIPT_EXPORT(ExportName(Material), Property(Setter))
		void SetMaterial(const MaterialType& material)
		{
			this->Material = material;
			MarkRenderProxyDataDirty();
		}

		/** @copydoc TDecalGetters::GetMaxDistance */
		B3D_SCRIPT_EXPORT(ExportName(MaxDistance), Property(Setter))
		void SetMaxDistance(float distance)
		{
			this->MaxDistance = Math::Max(0.0f, distance);
			MarkRenderProxyDataDirty();
			UpdateBounds();
		}

		/** @copydoc TDecalGetters::GetLayerMask */
		B3D_SCRIPT_EXPORT(ExportName(LayerMask), Property(Setter))
		void SetLayerMask(u32 mask)
		{
			this->LayerMask = mask;
			MarkRenderProxyDataDirty();
		}

		/** @copydoc TDecalGetters::GetLayer */
		B3D_SCRIPT_EXPORT(ExportName(Layer), Property(Setter))
		void SetLayer(u64 layer);

		/**	Gets world bounds of this object. */
		Bounds GetBounds() const { return mBounds; }

		/**	Returns the transform matrix that is applied to the object when its being rendered. */
		Matrix4 GetWorldTransformMatrix() const { return mWorldTransformMatrix; }

		/**
		 * Returns the transform matrix that is applied to the object when its being rendered. This transform matrix does
		 * not include scale values.
		 */
		Matrix4 GetWorldTransformMatrixWithoutScale() const { return mWorldTransformMatrixWithoutScale; }

		/** Returns the decal data struct. */
		const TDecalData<IsRenderProxy>& GetDecalData() const { return *this; }

	protected:
		/** Updates the internal bounds for the decal. Call this whenever a property affecting the bounds changes. */
		void UpdateBounds();

		/** @copydoc CoreObject::MarkRenderProxyDataDirty */
		void MarkRenderProxyDataDirty(ComponentDirtyFlag flag = ComponentDirtyFlag::Everything);

		/** @copydoc CoreObject::MarkDependenciesDirty */
		void MarkCoreObjectDependenciesDirty();

		Matrix4 mWorldTransformMatrix = kIdentityTag;
		Matrix4 mWorldTransformMatrixWithoutScale = kIdentityTag;
		Bounds mBounds = Bounds::kEmpty;
	};

	/** @} */

	/** @addtogroup Renderer-Internal
	 *  @{
	 */

	namespace ecs
	{
		class ECSDecalRTTI;

		/** ECS fragment storing decal visual data (material, size, layer, etc.). */
		struct B3D_EXPORT Decal : TDecalData<false>, IReflectable
		{
			struct FullSyncPacket;
			struct TransformSyncPacket;
			struct FullSyncPacketECS;
			struct TransformSyncPacketECS;

			friend class ECSDecalRTTI;
			static RTTIType* GetRttiStatic();
			RTTIType* GetRtti() const override;
		};

		/** ECS fragment storing the persistent render object ID for a decal. */
		struct DecalId
		{
			RendererId Id = kInvalidRendererId;
		};
	} // namespace ecs

	/** @} */

	/** @addtogroup Components
	 *  @{
	 */

	/** Wraps Decal as a Component. */
	class B3D_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Rendering)) Decal : public Component, public TDecalGetters<Decal, false>, public CoreObject
	{
		friend class TDecalGetters<Decal, false>;
	public:
		Decal(const HSceneObject& parent);

		/** @copydoc TDecalGetters::GetSize */
		B3D_SCRIPT_EXPORT(ExportName(Size), Property(Setter))
		void SetSize(const Vector2& size);

		/** @copydoc TDecalGetters::GetMaterial */
		B3D_SCRIPT_EXPORT(ExportName(Material), Property(Setter))
		void SetMaterial(const HMaterial& material);

		/** @copydoc TDecalGetters::GetMaxDistance */
		B3D_SCRIPT_EXPORT(ExportName(MaxDistance), Property(Setter))
		void SetMaxDistance(float distance);

		/** @copydoc TDecalGetters::GetLayerMask */
		B3D_SCRIPT_EXPORT(ExportName(LayerMask), Property(Setter))
		void SetLayerMask(u32 mask);

		/** @copydoc TDecalGetters::GetLayer */
		B3D_SCRIPT_EXPORT(ExportName(Layer), Property(Setter))
		void SetLayer(u64 layer);

		/**	Gets world bounds of this object. */
		Bounds GetBounds() const { return mBounds; }

		/**	Returns the transform matrix that is applied to the object when its being rendered. */
		Matrix4 GetWorldTransformMatrix() const { return mWorldTransformMatrix; }

		/**
		 * Returns the transform matrix that is applied to the object when its being rendered. This transform matrix does
		 * not include scale values.
		 */
		Matrix4 GetWorldTransformMatrixWithoutScale() const { return mWorldTransformMatrixWithoutScale; }

	protected:
		SPtr<render::RenderProxy> CreateRenderProxy() const override;
		void GetCoreDependencies(Vector<CoreObject*>& dependencies) override;
		RenderProxySyncPacket* CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags) override;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		void Initialize() override;
		void OnEnabled() override;
		void OnDisabled() override;
		void OnDestroyed() override;
		void OnSceneChanged(SceneInstance* oldScene, ecs::Entity oldEntity) override;
		void OnTransformChanged(TransformChangedFlags flags) override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class DecalRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;

	protected:
		Decal(); // Serialization only

	private:
		/** Returns a reference to the decal data for the CRTP getter interface. */
		const TDecalData<false>& GetDecalData() const;

		/** Returns a mutable reference to the ECS decal fragment. */
		ecs::Decal& GetFragment();

		/** Returns a const reference to the ECS decal fragment. */
		const ecs::Decal& GetFragment() const;

		/** @copydoc CoreObject::MarkRenderProxyDataDirty */
		void MarkRenderProxyDataDirty(ComponentDirtyFlag flag = ComponentDirtyFlag::Everything);

		/** Updates the internal bounds for the decal. Call this whenever a property affecting the bounds changes. */
		void UpdateBounds();

		Matrix4 mWorldTransformMatrix = kIdentityTag;
		Matrix4 mWorldTransformMatrixWithoutScale = kIdentityTag;
		Bounds mBounds = Bounds::kEmpty;
	};

	namespace render
	{
		/** Render thread counterpart of a b3d::Decal */
		class B3D_EXPORT Decal : public TDecal<true>
		{
		public:
			~Decal();

			/**	Sets an ID that can be used for uniquely identifying this object by the renderer. */
			void SetRendererId(u32 id) { mRendererId = id; }

			/**	Retrieves an ID that can be used for uniquely identifying this object by the renderer. */
			u32 GetRendererId() const { return mRendererId; }

			/** Returns the world space transform for the decal. */
			const Transform& GetWorldTransform() const { return mTransform; }

			/** Returns width and height of the decal, scaled by decal's transform. */
			Vector2 GetWorldSize() const
			{
				return Vector2(Size.X * mTransform.GetScale().X, Size.Y * mTransform.GetScale().Y);
			}

			/** Maximum distance (from its origin) at which the decal is displayed, scaled by decal's transform. */
			float GetWorldMaxDistance() const { return MaxDistance * mTransform.GetScale().Z; }

		protected:
			friend class b3d::Decal;
			friend struct ecs::Decal;

			Decal(const SPtr<SceneInstance>& scene);

			void Initialize() override;
			void SyncFromCoreObject(const CoreSyncData& data, FrameAllocator& allocator) override;

			u32 mRendererId = 0;
			Transform mTransform;
			bool mActive = true;
			SPtr<SceneInstance> mSceneInstance;
		};
	} // namespace render

	namespace render
	{
		/** Render-thread representation of a decal, stored in packed arrays in DecalObjectStorageBase. */
		class B3D_EXPORT DecalProxy : public TDecalGetters<DecalProxy, true>
		{
			friend class TDecalGetters<DecalProxy, true>;
			friend struct b3d::DecalFullUpdateChannel;
			friend struct b3d::DecalTransformUpdateChannel;

		public:
			/** Returns the world space transform matrix for the decal. */
			const Matrix4& GetWorldTransformMatrix() const { return mWorldTransformMatrix; }

			/** Returns the world space transform matrix without scale for the decal. */
			const Matrix4& GetWorldTransformMatrixWithoutScale() const { return mWorldTransformMatrixWithoutScale; }

			/** Returns world space bounds that encompass the decal's area of influence. */
			Bounds GetBounds() const { return mBounds; }

			/** Sets the renderer-assigned packed slot ID. */
			void SetRendererId(PackedRendererId id) { mRendererId = id; }

			/** Returns the renderer-assigned packed slot ID. */
			PackedRendererId GetRendererId() const { return mRendererId; }

		protected:
			const TDecalData<true>& GetDecalData() const { return mData; }

			TDecalData<true> mData;
			Matrix4 mWorldTransformMatrix = kIdentityTag;
			Matrix4 mWorldTransformMatrixWithoutScale = kIdentityTag;
			Bounds mBounds = Bounds::kEmpty;
			PackedRendererId mRendererId = kInvalidPackedRendererId;
		};
	} // namespace render

	/**
	 * Contains render thread representation of decal objects, stored in packed arrays accessible by PackedRendererId.
	 * Implements main -> render thread synchronization logic for decal objects.
	 */
	class B3D_EXPORT DecalObjectStorageBase : public RendererObjectStorage
	{
	public:
		void* SyncRead(ecs::Registry& registry, FrameAllocator& allocator) override;
		void SyncWrite(void* batchData, FrameAllocator& allocator) override;

		/** Returns the decal proxy at the given slot. */
		render::DecalProxy& GetDecalProxy(PackedRendererId slotId) { return mDecalProxies[slotId]; }

		/** Returns the decal proxy at the given slot (const). */
		const render::DecalProxy& GetDecalProxy(PackedRendererId slotId) const { return mDecalProxies[slotId]; }

		/** Returns the total number of decals. */
		u32 GetDecalCount() const { return (u32)mDecalProxies.size(); }

		/**
		 * Called once per frame for each new decal being added, or a decal whose data needs to be rebuilt
		 * after a significant update (in which case DestroyRenderState will be called first).
		 */
		virtual void CreateRenderState(TArrayView<const PackedRendererId> slotIds) = 0;

		/**
		 * Called once per frame for each decal that is being removed, or a decal that needs to be rebuilt
		 * after a significant update (in which case CreateRenderState will be called right after).
		 */
		virtual void DestroyRenderState(TArrayView<const PackedRendererId> slotIds) = 0;

		/**
		 * Called once per frame for each decal that needs to be updated after a minor update (e.g. one that doesn't
		 * require a full render state rebuild, such as a transform change).
		 */
		virtual void UpdateRenderState(TArrayView<const PackedRendererId> slotIds) = 0;

	protected:
		Vector<render::DecalProxy> mDecalProxies;
	};

	/** @} */
} // namespace b3d
