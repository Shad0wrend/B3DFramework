//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Renderer/BsCamera.h"
#include "Scene/BsComponent.h"

namespace b3d
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * Camera determines how is world geometry projected onto a 2D surface. You may position and orient it in space, set
	 * options like aspect ratio and field or view and it outputs view and projection matrices required for rendering.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Rendering)) CCamera : public Component
	{
	public:
		CCamera(const HSceneObject& parent);
		virtual ~CCamera() = default;

		/** Determines flags used for controlling the camera behaviour. */
		B3D_SCRIPT_EXPORT(ExportName(Flags), Property(Setter))
		void SetFlags(CameraFlags flags) { mInternal->SetFlags(flags); }

		/** @copydoc SetFlags */
		B3D_SCRIPT_EXPORT(ExportName(Flags), Property(Getter))
		CameraFlags GetFlags() const { return mInternal->GetFlags(); }

		/**	Returns the viewport used by the camera. */
		B3D_SCRIPT_EXPORT(ExportName(Viewport), Property(Getter))
		SPtr<Viewport> GetViewport() const { return mInternal->GetViewport(); }

		/**
		 * Determines the camera horizontal field of view. This determines how wide the camera viewing angle is along the
		 * horizontal axis. Vertical FOV is calculated from the horizontal FOV and the aspect ratio.
		 */
		B3D_SCRIPT_EXPORT(ExportName(FieldOfView), Property(Setter), UIValueRange([ 1, 360 ]), UI(AsSlider), UIOrder(-1))
		void SetHorizontalFOV(const Radian& fovy) { mInternal->SetHorzFov(fovy); }

		/** @copydoc SetHorizontalFOV */
		B3D_SCRIPT_EXPORT(ExportName(FieldOfView), Property(Getter), UIValueRange([ 1, 360 ]), UI(AsSlider), UIOrder(-1))
		const Radian& GetHorizontalFOV() const { return mInternal->GetHorzFov(); }

		/**
		 * Determines the distance from the frustum to the near clipping plane. Anything closer than the near clipping plane will
		 * not be rendered. Decreasing this value decreases depth buffer precision.
		 */
		B3D_SCRIPT_EXPORT(ExportName(NearClipPlane), Property(Setter))
		void SetNearClipDistance(float nearDist) { mInternal->SetNearClipDistance(nearDist); }

		/** @copydoc SetNearClipDistance */
		B3D_SCRIPT_EXPORT(ExportName(NearClipPlane), Property(Getter))
		float GetNearClipDistance() const { return mInternal->GetNearClipDistance(); }

		/**
		 * Determines the distance from the frustum to the far clipping plane. Anything farther than the far clipping plane will
		 * not be rendered. Increasing this value decreases depth buffer precision.
		 */
		B3D_SCRIPT_EXPORT(ExportName(FarClipPlane), Property(Setter))
		void SetFarClipDistance(float farDist) { mInternal->SetFarClipDistance(farDist); }

		/** @copydoc SetFarClipDistance */
		B3D_SCRIPT_EXPORT(ExportName(FarClipPlane), Property(Getter))
		float GetFarClipDistance() const { return mInternal->GetFarClipDistance(); }

		/**	Determines the current viewport aspect ratio (width / height). */
		B3D_SCRIPT_EXPORT(ExportName(AspectRatio), Property(Setter))
		void SetAspectRatio(float ratio) { mInternal->SetAspectRatio(ratio); }

		/** @copydoc SetAspectRatio */
		B3D_SCRIPT_EXPORT(ExportName(AspectRatio), Property(Getter))
		float GetAspectRatio() const { return mInternal->GetAspectRatio(); }

		/**
		 * Manually set the extents of the frustum that will be used when calculating the projection matrix. This will
		 * prevents extents for being automatically calculated from aspect and near plane so it is up to the caller to keep
		 * these values accurate.
		 *
		 * @param left		The position where the left clip plane intersect the near clip plane, in view space.
		 * @param right		The position where the right clip plane intersect the near clip plane, in view space.
		 * @param top		The position where the top clip plane intersect the near clip plane, in view space.
		 * @param bottom	The position where the bottom clip plane intersect the near clip plane, in view space.
		 */
		void SetFrustumExtents(float left, float right, float top, float bottom)
		{
			mInternal->SetFrustumExtents(left, right, top, bottom);
		}

		/**
		 * Resets frustum extents so they are automatically derived from other values. This is only relevant if you have
		 * previously set custom extents.
		 */
		void ResetFrustumExtents() { mInternal->ResetFrustumExtents(); }

		/** Returns the extents of the frustum in view space at the near plane. */
		void GetFrustumExtents(float& outleft, float& outright, float& outtop, float& outbottom) const
		{
			mInternal->GetFrustumExtents(outleft, outright, outtop, outbottom);
		}

		/**
		 * Returns the standard projection matrix that determines how are 3D points projected to two dimensions. The layout
		 * of this matrix depends on currently used GPU backend.
		 */
		B3D_SCRIPT_EXPORT(ExportName(ProjMatrix), Property(Getter))
		const Matrix4& GetProjectionMatrix() const { return mInternal->GetProjectionMatrixRs(); }

		/**
		 * Returns the standard projection matrix that determines how are 3D points projected to two dimensions. Returned
		 * matrix is standard following right-hand rules and depth range of [-1, 1]. Note that currently used GPU backend
		 * might expect different rules, in which case use GetProjectionMatrix() to retrieve an adjusted matrix.
		 */
		const Matrix4& GetUnadjustedProjectionMatrix() const { return mInternal->GetProjectionMatrix(); }

		/** Gets the camera view matrix. Used for positioning/orienting the camera. */
		B3D_SCRIPT_EXPORT(ExportName(ViewMatrix), Property(Getter))
		const Matrix4& GetViewMatrix() const
		{
			UpdateView();
			return mInternal->GetViewMatrix();
		}

		/**
		 * Sets whether the camera should use the custom view matrix. When this is enabled camera will no longer calculate
		 * its view matrix based on position/orientation and caller will be resonsible to keep the view matrix up to date.
		 */
		void SetCustomViewMatrix(bool enable, const Matrix4& viewMatrix = Matrix4::kIdentity)
		{
			mInternal->SetCustomViewMatrix(enable, viewMatrix);
		}

		/** Returns true if a custom view matrix is used. */
		bool IsCustomViewMatrixEnabled() const { return mInternal->IsCustomViewMatrixEnabled(); }

		/**
		 * Sets whether the camera should use the custom projection matrix. When this is enabled camera will no longer
		 * calculate its projection matrix based on field of view, aspect and other parameters and caller will be resonsible
		 * to keep the projection matrix up to date.
		 */
		void SetCustomProjectionMatrix(bool enable, const Matrix4& projectionMatrix = Matrix4::kIdentity)
		{
			mInternal->SetCustomProjectionMatrix(enable, projectionMatrix);
		}

		/** Returns the extents of the frustum in view space at the near plane. */
		bool IsCustomProjectionMatrixEnabled() const { return mInternal->IsCustomProjectionMatrixEnabled(); }

		/** Returns a convex volume representing the visible area of the camera, in local space. */
		const ConvexVolume& GetFrustum() const { return mInternal->GetFrustum(); }

		/** Returns a convex volume representing the visible area of the camera, in world space. */
		ConvexVolume GetWorldFrustum() const;

		/**	Returns the bounding of the frustum. */
		const AABox& GetBoundingBox() const { return mInternal->GetBoundingBox(); }

		/**
		 * Determines the type of projection used by the camera. Projection type controls how is 3D geometry projected onto a
		 * 2D plane.
		 */
		B3D_SCRIPT_EXPORT(ExportName(ProjectionType), Property(Setter), UIOrder(-2))
		virtual void SetProjectionType(ProjectionType pt) { mInternal->SetProjectionType(pt); }

		/** @copydoc SetProjectionType */
		B3D_SCRIPT_EXPORT(ExportName(ProjectionType), Property(Getter), UIOrder(-2))
		virtual ProjectionType GetProjectionType() const { return mInternal->GetProjectionType(); }

		/**
		 * Sets the orthographic window height, for use with orthographic rendering only.
		 *
		 * @param	w	Width of the window in world units.
		 * @param	h	Height of the window in world units.
		 *
		 * @note
		 * Calling this method will recalculate the aspect ratio, use SetOrthoWindowHeight() or SetOrthoWindowWidth() alone
		 * if you wish to preserve the aspect ratio but just fit one or other dimension to a particular size.
		 */
		virtual void SetOrthographicSize(float w, float h) { mInternal->SetOrthoWindow(w, h); }

		/**
		 * Determines the type of projection used by the camera. Projection type controls how is 3D geometry projected onto a
		 * 2D plane.
		 */
		B3D_SCRIPT_EXPORT(ExportName(OrthoHeight), Property(Setter), UIOrder(-1))
		virtual void SetOrthographicHeight(float h) { mInternal->SetOrthoWindowHeight(h); }

		/** @copydoc SetOrthoWindowHeight */
		B3D_SCRIPT_EXPORT(ExportName(OrthoHeight), Property(Getter), UIOrder(-1))
		virtual float GetOrthographicHeight() const { return mInternal->GetOrthoWindowHeight(); }

		/**
		 * Determines the orthographic window width, for use with orthographic rendering only. The height of the window
		 * will be calculated from the aspect ratio. Value is specified in world units.
		 */
		B3D_SCRIPT_EXPORT(ExportName(OrthoWidth), Property(Setter), UI(Hide))
		virtual void SetOrthographicWidth(float w) { mInternal->SetOrthoWindowWidth(w); }

		/** @copydoc GetOrthoWindowWidth */
		B3D_SCRIPT_EXPORT(ExportName(OrthoWidth), Property(Getter), UI(Hide))
		virtual float GetOrthographicWidth() const { return mInternal->GetOrthoWindowWidth(); }

		/**
		 * Determines a priority that determines in which orders the cameras are rendered. This only applies to cameras rendering
		 * to the same render target. Higher value means the camera will be rendered sooner.
		 */
		B3D_SCRIPT_EXPORT(ExportName(Priority), Property(Setter))
		void SetPriority(i32 priority) { mInternal->SetPriority(priority); }

		/** @copydoc SetPriority */
		B3D_SCRIPT_EXPORT(ExportName(Priority), Property(Getter))
		i32 GetPriority() const { return mInternal->GetPriority(); }

		/**	Determines layer bitfield that is used when determining which object should the camera render. */
		B3D_SCRIPT_EXPORT(ExportName(Layers), Property(Setter), UI(AsLayerMask))
		void SetLayers(u64 layers) { mInternal->SetLayers(layers); }

		/** @copydoc SetLayers */
		B3D_SCRIPT_EXPORT(ExportName(Layers), Property(Getter), UI(AsLayerMask))
		u64 GetLayers() const { return mInternal->GetLayers(); }

		/**
		 * Determines number of samples to use when rendering to this camera. Values larger than 1 will enable MSAA
		 * rendering.
		 */
		B3D_SCRIPT_EXPORT(ExportName(SampleCount), Property(Setter))
		void SetMSAACount(u32 count) { mInternal->SetMsaaCount(count); }

		/** @copydoc SetMSAACount */
		B3D_SCRIPT_EXPORT(ExportName(SampleCount), Property(Getter))
		u32 GetMSAACount() const { return mInternal->GetMsaaCount(); }

		/**
		 * Settings that control rendering for this view. They determine how will the renderer process this view, which
		 * effects will be enabled, and what properties will those effects use.
		 */
		B3D_SCRIPT_EXPORT(ExportName(RenderSettings), Property(Setter), ApplyOnDirty(true))
		void SetRenderSettings(const SPtr<RenderSettings>& settings) { mInternal->SetRenderSettings(settings); }

		/** @copydoc SetRenderSettings() */
		B3D_SCRIPT_EXPORT(ExportName(RenderSettings), Property(Getter), ApplyOnDirty(true))
		const SPtr<RenderSettings>& GetRenderSettings() const { return mInternal->GetRenderSettings(); }

		/** @copydoc Camera::NotifyNeedsRedraw() */
		B3D_SCRIPT_EXPORT()
		void NotifyNeedsRedraw() { mInternal->NotifyNeedsRedraw(); }

		/**
		 * Converts a point in world space to screen coordinates.
		 *
		 * @param	worldPoint		3D point in world space.
		 * @return					2D point on the render target attached to the camera's viewport, in pixels.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2I WorldToScreenPoint(const Vector3& worldPoint) const
		{
			UpdateView();
			return mInternal->WorldToScreenPoint(worldPoint);
		}

		/**
		 * Converts a point in world space to normalized device coordinates.
		 *
		 * @param	worldPoint		3D point in world space.
		 * @return					2D point in normalized device coordinates ([-1, 1] range), relative to the camera's viewport.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2 WorldToNDCPoint(const Vector3& worldPoint) const
		{
			UpdateView();
			return mInternal->WorldToNdcPoint(worldPoint);
		}

		/**
		 * Converts a point in world space to view space coordinates.
		 *
		 * @param	worldPoint		3D point in world space.
		 * @return					3D point relative to the camera's coordinate system.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 WorldToViewPoint(const Vector3& worldPoint) const
		{
			UpdateView();
			return mInternal->WorldToViewPoint(worldPoint);
		}

		/**
		 * Converts a point in screen space to a point in world space.
		 *
		 * @param	screenPoint		2D point on the render target attached to the camera's viewport, in pixels.
		 * @param	depth			Depth to place the world point at, in world coordinates. The depth is applied to the
		 *							vector going from camera origin to the point on the near plane.
		 * @return					3D point in world space.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 ScreenToWorldPoint(const Vector2I& screenPoint, float depth = 0.5f) const
		{
			UpdateView();
			return mInternal->ScreenToWorldPoint(screenPoint, depth);
		}

		/**
		 * Converts a point in screen space to a point in view space.
		 *
		 * @param	screenPoint	2D point on the render target attached to the camera's viewport, in pixels.
		 * @param	depth		Depth to place the world point at, in device depth. The depth is applied to the
		 *						vector going from camera origin to the point on the near plane.
		 * @return				3D point relative to the camera's coordinate system.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 ScreenToViewPoint(const Vector2I& screenPoint, float depth = 0.5f) const { return mInternal->ScreenToViewPoint(screenPoint, depth); }

		/**
		 * Converts a point in screen space to normalized device coordinates.
		 *
		 * @param	screenPoint		2D point on the render target attached to the camera's viewport, in pixels.
		 * @return					2D point in normalized device coordinates ([-1, 1] range), relative to
		 *							the camera's viewport.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2 ScreenToNDCPoint(const Vector2I& screenPoint) const { return mInternal->ScreenToNdcPoint(screenPoint); }

		/**
		 * Converts a point in view space to world space.
		 *
		 * @param	viewPoint		3D point relative to the camera's coordinate system.
		 * @return					3D point in world space.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 ViewToWorldPoint(const Vector3& viewPoint) const
		{
			UpdateView();
			return mInternal->ViewToWorldPoint(viewPoint);
		}

		/**
		 * Converts a point in view space to screen space.
		 *
		 * @param	viewPoint		3D point relative to the camera's coordinate system.
		 * @return					2D point on the render target attached to the camera's viewport, in pixels.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2I ViewToScreenPoint(const Vector3& viewPoint) const { return mInternal->ViewToScreenPoint(viewPoint); }

		/**
		 * Converts a point in view space to normalized device coordinates.
		 *
		 * @param	viewPoint		3D point relative to the camera's coordinate system.
		 * @return					2D point in normalized device coordinates ([-1, 1] range), relative to
		 *							the camera's viewport.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2 ViewToNDCPoint(const Vector3& viewPoint) const { return mInternal->ViewToNdcPoint(viewPoint); }

		/**
		 * Converts a point in normalized device coordinates to world space.
		 *
		 * @param	ndcPoint	2D point in normalized device coordinates ([-1, 1] range), relative to
		 *						the camera's viewport.
		 * @param	depth		Depth to place the world point at. The depth is applied to the
		 *						vector going from camera origin to the point on the near plane.
		 * @return				3D point in world space.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 NDCToWorldPoint(const Vector2& ndcPoint, float depth = 0.5f) const
		{
			UpdateView();
			return mInternal->NdcToWorldPoint(ndcPoint, depth);
		}

		/**
		 * Converts a point in normalized device coordinates to view space.
		 *
		 * @param	ndcPoint	2D point in normalized device coordinates ([-1, 1] range), relative to
		 *						the camera's viewport.
		 * @param	depth		Depth to place the world point at. The depth is applied to the
		 *						vector going from camera origin to the point on the near plane.
		 * @return				3D point relative to the camera's coordinate system.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 NDCToViewPoint(const Vector2& ndcPoint, float depth = 0.5f) const { return mInternal->NdcToViewPoint(ndcPoint, depth); }

		/**
		 * Converts a point in normalized device coordinates to screen space.
		 *
		 * @param	ndcPoint	2D point in normalized device coordinates ([-1, 1] range), relative to
		 *						the camera's viewport.
		 * @return				2D point on the render target attached to the camera's viewport, in pixels.
		 */
		B3D_SCRIPT_EXPORT()
		Vector2I NDCToScreenPoint(const Vector2& ndcPoint) const { return mInternal->NdcToScreenPoint(ndcPoint); }

		/**
		 * Converts a point in screen space to a ray in world space.
		 *
		 * @param	screenPoint		2D point on the render target attached to the camera's viewport, in pixels.
		 * @return					Ray in world space, originating at the selected point on the camera near plane.
		 */
		B3D_SCRIPT_EXPORT()
		Ray ScreenPointToRay(const Vector2I& screenPoint) const
		{
			UpdateView();
			return mInternal->ScreenPointToRay(screenPoint);
		}

		/**
		 * Projects a point in view space to normalized device coordinates. Similar to viewToNdcPoint() but preserves
		 * the depth component.
		 *
		 * @param	point			3D point relative to the camera's coordinate system.
		 * @return					3D point in normalized device coordinates ([-1, 1] range), relative to the
		 *							camera's viewport. Z value range depends on active render API.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 ProjectPoint(const Vector3& point) const { return mInternal->ProjectPoint(point); }

		/**
		 * Un-projects a point in normalized device space to view space.
		 *
		 * @param	point			3D point in normalized device coordinates ([-1, 1] range), relative to the
		 *							camera's viewport. Z value range depends on active render API.
		 * @return					3D point relative to the camera's coordinate system.
		 */
		B3D_SCRIPT_EXPORT()
		Vector3 UnprojectPoint(const Vector3& point) const { return mInternal->UnprojectPoint(point); }

		/**
		 * Determines whether this is the main application camera. Main camera controls the final render surface that is
		 * displayed to the user.
		 */
		B3D_SCRIPT_EXPORT(ExportName(Main), Property(Setter))
		void SetMain(bool main);

		/** @copydoc SetMain */
		B3D_SCRIPT_EXPORT(ExportName(Main), Property(Getter))
		bool IsMain() const { return mInternal->IsMain(); }

		/** @name Internal
		 *  @{
		 */

		/** Returns the internal camera that is used for majority of operations by this component. */
		SPtr<Camera> GetCameraInternal() const
		{
			UpdateView();
			return mInternal;
		}

		/** @} */

	protected:
		/** Checks if the world transform of the camera changed, and if needed updates the view matrix. */
		void UpdateView() const;

		mutable SPtr<Camera> mInternal;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		void Initialize() override;
		void OnBeginPlay() override;
		void OnDestroyed() override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CCameraRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;

	protected:
		CCamera(); // Serialization only
	};

	/** @} */
} // namespace b3d
