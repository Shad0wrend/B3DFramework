//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsIReflectable.h"
#include "Math/BsMatrix4.h"
#include "Math/BsVector3.h"
#include "Math/BsVector2.h"
#include "Math/BsAABox.h"
#include "Math/BsQuaternion.h"
#include "Math/BsRay.h"
#include "CoreObject/BsCoreObject.h"
#include "Math/BsArea2.h"
#include "Math/BsConvexVolume.h"
#include "Renderer/BsRenderSettings.h"
#include "Scene/BsSceneActor.h"

namespace b3d
{
	/** @addtogroup Renderer
	 *  @{
	 */

	/** Flags for controlling Camera options. */
	enum class B3D_SCRIPT_EXPORT(DocumentationGroup(Rendering)) CameraFlag
	{
		/**
		 * If set the camera will only render when requested by the user through Camera::notifyNeedsRedraw().
		 * Otherwise the camera will render every frame (unless disabled).
		 */
		OnDemand = 1 << 0,
	};

	using CameraFlags = Flags<CameraFlag>;
	B3D_FLAGS_OPERATORS(CameraFlag)

	/** @} */

	/** @addtogroup Renderer-Internal
	 *  @{
	 */

	/**	Signals which portion of a Camera is dirty. */
	enum class CameraDirtyFlag
	{
		// First few bits reserved by ActorDirtyFlag
		RenderSettings = 1 << 4,
		Redraw = 1 << 5,
		Viewport = 1 << 31
	};

	/** @} */
	/** @addtogroup Implementation
	 *  @{
	 */

	/** Common base class for both main and render thread implementations of Camera. */
	class B3D_CORE_EXPORT CameraBase
	{
	public:
		virtual ~CameraBase() = default;

		void SetFlags(CameraFlags flag);
		CameraFlags GetFlags() const { return mCameraFlags; }

		virtual void SetHorzFov(const Radian& fovy);
		virtual const Radian& GetHorzFov() const { return mHorzFOV; }

		virtual void SetNearClipDistance(float nearDist);
		virtual float GetNearClipDistance() const { return mNearDist; }

		virtual void SetFarClipDistance(float farDist);
		virtual float GetFarClipDistance() const { return mFarDist; }

		virtual void SetAspectRatio(float ratio);
		virtual float GetAspectRatio() const;

		virtual void SetFrustumExtents(float left, float right, float top, float bottom);
		virtual void ResetFrustumExtents();
		virtual void GetFrustumExtents(float& outleft, float& outright, float& outtop, float& outbottom) const;

		virtual const Matrix4& GetProjectionMatrixRs() const;

		/** Returns the inverse of the render-system specific projection matrix. See getProjectionMatrixRS(). */
		virtual const Matrix4& GetProjectionMatrixRsInv() const;

		virtual const Matrix4& GetProjectionMatrix() const;

		/** Returns the inverse of the projection matrix. See getProjectionMatrix(). */
		virtual const Matrix4& GetProjectionMatrixInv() const;

		virtual void SetCustomProjectionMatrix(bool enable, const Matrix4& projectionMatrix = Matrix4::kIdentity);
		virtual bool IsCustomProjectionMatrixEnabled() const { return mCustomProjMatrix; }

		virtual const ConvexVolume& GetFrustum() const;

		const AABox& GetBoundingBox() const;

		virtual void SetProjectionType(ProjectionType pt);
		virtual ProjectionType GetProjectionType() const;

		virtual void SetOrthoWindow(float w, float h);

		virtual void SetOrthoWindowHeight(float h);
		virtual float GetOrthoWindowHeight() const;

		virtual void SetOrthoWindowWidth(float w);
		virtual float GetOrthoWindowWidth() const;

		void SetPriority(i32 priority)
		{
			mPriority = priority;
			MarkRenderProxyDataDirty();
		}

		i32 GetPriority() const { return mPriority; }

		void SetLayers(u64 layers)
		{
			mLayers = layers;
			MarkRenderProxyDataDirty();
		}

		u64 GetLayers() const { return mLayers; }

		void SetMsaaCount(u32 count)
		{
			mMSAA = count;
			MarkRenderProxyDataDirty();
		}

		u32 GetMsaaCount() const { return mMSAA; }

		/**
		 * Notifies a on-demand camera that it should re-draw its contents on the next frame. Ignored for a camera
		 * that isn't on-demand.
		 */
		void NotifyNeedsRedraw() { MarkRenderProxyDataDirty((ActorDirtyFlag)CameraDirtyFlag::Redraw); }

		static const float kInfiniteFarPlaneAdjust; /**< Small constant used to reduce far plane projection to avoid inaccuracies. */

	protected:
		CameraBase();

		/**	Calculate projection parameters that are used when constructing the projection matrix. */
		virtual void CalcProjectionParameters(float& left, float& right, float& bottom, float& top) const;

		/**	Recalculate frustum if dirty. */
		virtual void UpdateFrustum() const;

		/**	Recalculate frustum planes if dirty. */
		virtual void UpdateFrustumPlanes() const;

		/**	Checks if the frustum requires updating. */
		virtual bool IsFrustumOutOfDate() const;

		/**	Notify camera that the frustum requires to be updated. */
		virtual void InvalidateFrustum() const;

		/**	Returns a rectangle that defines the viewport position and size, in pixels. */
		virtual Area2I GetViewportRect() const = 0;

		/** Marks the render proxy data dirty, which lets the system know to synchronize it with the render thread at first opportunity. */
		virtual void MarkRenderProxyDataDirty(ActorDirtyFlag dirtyFlag = ActorDirtyFlag::Everything) { }

	protected:
		u64 mLayers = 0xFFFFFFFFFFFFFFFF; /**< Bitfield that can be used for filtering what objects the camera sees. */

		ProjectionType mProjType = PT_PERSPECTIVE; /**< Type of camera projection. */
		Radian mHorzFOV = Degree(90.0f); /**< Horizontal field of view represents how wide is the camera angle. */
		float mFarDist = 500.0f; /**< Clip any objects further than this. Larger value decreases depth precision at smaller depths. */
		float mNearDist = 0.05f; /**< Clip any objects close than this. Smaller value decreases depth precision at larger depths. */
		float mAspect = 1.33333333333333f; /**< Width/height viewport ratio. */
		float mOrthoHeight = 5; /**< Height in world units used for orthographic cameras. */
		i32 mPriority = 0; /**< Determines in what order will the camera be rendered. Higher priority means the camera will be rendered sooner. */
		bool mMain = false; /**< Determines does this camera render to the main render surface. */
		CameraFlags mCameraFlags; /**< Flags for controlling various behaviour. */

		bool mCustomProjMatrix = false; /**< Is custom projection matrix set. */
		u8 mMSAA = 1; /**< Number of samples to render the scene with. */

		bool mFrustumExtentsManuallySet = false; /**< Are frustum extents manually set. */

		mutable Matrix4 mProjMatrixRS = BsZero; /**< Cached render-system specific projection matrix. */
		mutable Matrix4 mProjMatrix = BsZero; /**< Cached projection matrix that determines how are 3D points projected to a 2D viewport. */
		mutable Matrix4 mProjMatrixRSInv = BsZero;
		mutable Matrix4 mProjMatrixInv = BsZero;

		mutable ConvexVolume mFrustum; /**< Main clipping planes describing cameras visible area. */
		mutable bool mRecalcFrustum : 1; /**< Should frustum be recalculated. */
		mutable bool mRecalcFrustumPlanes : 1; /**< Should frustum planes be recalculated. */
		mutable float mLeft, mRight, mTop, mBottom; /**< Frustum extents. */
		mutable AABox mBoundingBox; /**< Frustum bounding box. */
	};

	/** Templated common base class for both main and render thread implementations of Camera. */
	template <bool IsRenderProxy>
	class B3D_CORE_EXPORT TCamera : public CameraBase, public CoreVariantType<SceneActor, IsRenderProxy>
	{
		using ViewportType = CoreVariantType<Viewport, IsRenderProxy>;
		using RenderSettingsType = CoreVariantType<RenderSettings, IsRenderProxy>;
		using SceneInstanceType = CoreVariantType<SceneInstance, IsRenderProxy>;
		using Super = CoreVariantType<SceneActor, IsRenderProxy>;

	public:
		TCamera(const SPtr<SceneInstanceType>& scene);
		TCamera();
		virtual ~TCamera() = default;

		void SetTransform(const Transform& transform) override;

		SPtr<ViewportType> GetViewport() const { return mViewport; }

		virtual void SetCustomViewMatrix(bool enable, const Matrix4& viewMatrix = Matrix4::kIdentity);
		virtual bool IsCustomViewMatrixEnabled() const { return mCustomViewMatrix; }

		virtual const Matrix4& GetViewMatrix() const;

		/** Returns the inverse of the view matrix. See getViewMatrix(). */
		virtual const Matrix4& GetViewMatrixInv() const;

		/** Returns a convex volume representing the visible area of the camera, in world space. */
		virtual ConvexVolume GetWorldFrustum() const;

		void SetRenderSettings(const SPtr<RenderSettingsType>& settings)
		{
			mRenderSettings = settings;
			MarkRenderProxyDataDirty((ActorDirtyFlag)CameraDirtyFlag::RenderSettings);
		}

		const SPtr<RenderSettingsType>& GetRenderSettings() const { return mRenderSettings; }

		Vector2I WorldToScreenPoint(const Vector3& worldPoint) const;
		Vector2 WorldToNdcPoint(const Vector3& worldPoint) const;

		Vector3 WorldToViewPoint(const Vector3& worldPoint) const;
		Vector3 ScreenToWorldPoint(const Vector2I& screenPoint, float depth = 0.5f) const;

		/**
		 * Converts a point in screen space (pixels corresponding to render target attached to the camera) to a point in
		 * world space.
		 *
		 * @param[in]	screenPoint	Point to transform.
		 * @param[in]	deviceDepth	Depth to place the world point at, in normalized device coordinates.
		 * @return					3D point in world space.
		 */
		Vector3 ScreenToWorldPointDeviceDepth(const Vector2I& screenPoint, float deviceDepth = 0.5f) const;

		Vector3 ScreenToViewPoint(const Vector2I& screenPoint, float depth = 0.5f) const;
		Vector2 ScreenToNdcPoint(const Vector2I& screenPoint) const;

		Vector3 ViewToWorldPoint(const Vector3& viewPoint) const;
		Vector2I ViewToScreenPoint(const Vector3& viewPoint) const;

		Vector2 ViewToNdcPoint(const Vector3& viewPoint) const;
		Vector3 NdcToWorldPoint(const Vector2& ndcPoint, float depth = 0.5f) const;

		Vector3 NdcToViewPoint(const Vector2& ndcPoint, float depth = 0.5f) const;
		Vector2I NdcToScreenPoint(const Vector2& ndcPoint) const;

		Ray ScreenPointToRay(const Vector2I& screenPoint) const;

		Vector3 ProjectPoint(const Vector3& point) const;
		Vector3 UnprojectPoint(const Vector3& point) const;

		void MarkRenderProxyDataDirty(ActorDirtyFlag dirtyFlag = ActorDirtyFlag::Everything) override
		{
			this->MarkSceneActorRenderProxyDataDirty(dirtyFlag);
		}

	protected:
		/**
		 * Update view matrix from parent position/orientation.
		 *
		 * @note	Does nothing when custom view matrix is set.
		 */
		virtual void UpdateView() const;

		bool mCustomViewMatrix = false; /**< Is custom view matrix set. */
		mutable Matrix4 mViewMatrix = BsZero; /**< Cached view matrix that determines camera position/orientation. */
		mutable Matrix4 mViewMatrixInv = BsZero;
		mutable bool mRecalcView : 1; /**< Should view matrix be recalculated. */

		/** Viewport that describes a 2D rendering surface. */
		SPtr<ViewportType> mViewport;

		/** Settings used to control rendering for this camera. */
		SPtr<RenderSettingsType> mRenderSettings;
	};

	/** @} */

	/** @addtogroup Renderer-Internal
	 *  @{
	 */

	/**
	 * Camera determines how is world geometry projected onto a 2D surface. You may position and orient it in space, set
	 * options like aspect ratio and field or view and it outputs view and projection matrices required for rendering.
	 */
	class B3D_CORE_EXPORT Camera : public IReflectable, public CoreObject, public TCamera<false>
	{
	public:
		void SetMain(bool main);
		bool IsMain() const { return mMain; }

		/**	Creates a new camera that renders to the specified portion of the provided render target. */
		static SPtr<Camera> Create(const SPtr<SceneInstance>& scene);

		/**
		 * @name Internal
		 * @{
		 */

		void Initialize() override;
		void Destroy() override;

		/** @} */
	protected:
		struct FullSyncPacket;
		struct RedrawSyncPacket;
		struct TransformSyncPacket;

		friend class render::Camera;

		Camera() = default; // Serialization only
		Camera(const SPtr<SceneInstance>& scene);

		/** @copydoc CameraBase */
		Area2I GetViewportRect() const override;

		SPtr<render::RenderProxy> CreateRenderProxy() const override;
		void MarkSceneActorRenderProxyDataDirty(ActorDirtyFlag flag = ActorDirtyFlag::Everything) override;
		RenderProxySyncPacket* CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags) override;

		void GetCoreDependencies(Vector<CoreObject*>& dependencies) override;

		/**	Creates a new camera without initializing it. */
		static SPtr<Camera> CreateEmpty();

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CameraRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;
	};

	namespace render
	{
		/** @copydoc b3d::Camera */
		class B3D_CORE_EXPORT Camera : public RenderProxy, public TCamera<true>
		{
		public:
			~Camera();

			/** @copydoc b3d::Camera::setMain() */
			bool IsMain() const { return mMain; }

			/**	Sets an ID that can be used for uniquely identifying this object by the renderer. */
			void SetRendererId(u32 id) { mRendererId = id; }

			/**	Retrieves an ID that can be used for uniquely identifying this object by the renderer. */
			u32 GetRendererId() const { return mRendererId; }

		protected:
			friend class b3d::Camera;

			Camera(const SPtr<SceneInstance>& scene, const SPtr<RenderTarget>& target = nullptr, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
			Camera(const SPtr<SceneInstance>& scene, const SPtr<Viewport>& viewport);

			void Initialize() override;
			Area2I GetViewportRect() const override;
			void SyncFromCoreObject(const CoreSyncData& data, FrameAllocator& allocator) override;

			u32 mRendererId;
		};
	} // namespace render

	/** @} */
} // namespace b3d
