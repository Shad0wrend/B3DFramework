//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Renderer/BsCamera.h"
#include "Scene/BsComponent.h"

namespace bs
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * @copydoc	Camera
	 * 			
	 * Wraps a Camera as a Component.
	 */
	class BS_CORE_EXPORT BS_SCRIPT_EXPORT(m:Rendering,n:Camera) CCamera : public Component
	{
	public:
		CCamera(const HSceneObject& parent);
		virtual ~CCamera() = default;

		/** @copydoc Camera::setFlags */
		BS_SCRIPT_EXPORT(n:Flags,pr:setter)
		void SetFlags(CameraFlags flags) { mInternal->setFlags(flags); }

		/** @copydoc Camera::getFlags */
		BS_SCRIPT_EXPORT(n:Flags,pr:getter)
		CameraFlags GetFlags() const { return mInternal->getFlags(); }
		
		/** @copydoc Camera::getViewport */
		BS_SCRIPT_EXPORT(n:Viewport,pr:getter)
		SPtr<Viewport> GetViewport() const { return mInternal->getViewport(); }

		/** @copydoc Camera::setHorzFOV */
		BS_SCRIPT_EXPORT(n:FieldOfView,pr:setter,range:[1,360],slider,order:-1)
		virtual void SetHorzFOV(const Radian& fovy) { mInternal->setHorzFOV(fovy); }

		/** @copydoc Camera::getHorzFOV */
		BS_SCRIPT_EXPORT(n:FieldOfView,pr:getter,range:[1,360],slider,order:-1)
		virtual const Radian& GetHorzFOV() const { return mInternal->getHorzFOV(); }

		/** @copydoc Camera::setNearClipDistance */
		BS_SCRIPT_EXPORT(n:NearClipPlane,pr:setter)
		virtual void SetNearClipDistance(float nearDist) { mInternal->setNearClipDistance(nearDist); }

		/** @copydoc Camera::getNearClipDistance */
		BS_SCRIPT_EXPORT(n:NearClipPlane,pr:getter)
		virtual float GetNearClipDistance() const { return mInternal->getNearClipDistance(); }

		/** @copydoc Camera::setFarClipDistance */
		BS_SCRIPT_EXPORT(n:FarClipPlane,pr:setter)
		virtual void SetFarClipDistance(float farDist) { mInternal->setFarClipDistance(farDist); }

		/** @copydoc Camera::getFarClipDistance */
		BS_SCRIPT_EXPORT(n:FarClipPlane,pr:getter)
		virtual float GetFarClipDistance() const { return mInternal->getFarClipDistance(); }

		/** @copydoc Camera::setAspectRatio */
		BS_SCRIPT_EXPORT(n:AspectRatio,pr:setter)
		virtual void SetAspectRatio(float ratio) { mInternal->setAspectRatio(ratio); }

		/** @copydoc Camera::getAspectRatio */
		BS_SCRIPT_EXPORT(n:AspectRatio,pr:getter)
		virtual float GetAspectRatio() const { return mInternal->getAspectRatio(); }

		/** @copydoc Camera::setFrustumExtents */
		virtual void SetFrustumExtents(float left, float right, float top, float bottom)
		{
			mInternal->setFrustumExtents(left, right, top, bottom);
		}

		/** @copydoc Camera::resetFrustumExtents */
		virtual void ResetFrustumExtents() { mInternal->resetFrustumExtents(); }

		/** @copydoc Camera::getFrustumExtents */
		virtual void GetFrustumExtents(float& outleft, float& outright, float& outtop, float& outbottom) const
		{
			mInternal->getFrustumExtents(outleft, outright, outtop, outbottom);
		}

		/** @copydoc Camera::getProjectionMatrixRS */
		BS_SCRIPT_EXPORT(n:ProjMatrix,pr:getter)
		virtual const Matrix4& GetProjectionMatrixRS() const { return mInternal->getProjectionMatrixRS(); }

		/** @copydoc Camera::getProjectionMatrix */
		virtual const Matrix4& GetProjectionMatrix() const { return mInternal->getProjectionMatrix(); }

		/** @copydoc Camera::getViewMatrix */
		BS_SCRIPT_EXPORT(n:ViewMatrix,pr:getter)
		virtual const Matrix4& GetViewMatrix() const { updateView(); return mInternal->getViewMatrix(); }

		/** @copydoc Camera::setCustomViewMatrix */
		virtual void SetCustomViewMatrix(bool enable, const Matrix4& viewMatrix = Matrix4::IDENTITY)
		{
			mInternal->setCustomViewMatrix(enable, viewMatrix);
		}

		/** @copydoc Camera::isCustomViewMatrixEnabled */
		virtual bool IsCustomViewMatrixEnabled() const { return mInternal->isCustomViewMatrixEnabled(); }
		
		/** @copydoc Camera::setCustomProjectionMatrix */
		virtual void SetCustomProjectionMatrix(bool enable, const Matrix4& projectionMatrix = Matrix4::IDENTITY)
		{
			mInternal->setCustomProjectionMatrix(enable, projectionMatrix);
		}

		/** @copydoc Camera::isCustomProjectionMatrixEnabled */
		virtual bool IsCustomProjectionMatrixEnabled() const { return mInternal->isCustomProjectionMatrixEnabled(); }

		/** @copydoc Camera::getFrustum */
		virtual const ConvexVolume& GetFrustum() const { return mInternal->getFrustum(); }

		/** @copydoc Camera::getWorldFrustum */
		virtual ConvexVolume GetWorldFrustum() const;

		/** @copydoc Camera::getBoundingBox */
		const AABox& GetBoundingBox() const { return mInternal->getBoundingBox(); }

		/** @copydoc Camera::setProjectionType */
		BS_SCRIPT_EXPORT(n:ProjectionType,pr:setter,order:-2)
		virtual void SetProjectionType(ProjectionType pt) { mInternal->setProjectionType(pt); }

		/** @copydoc Camera::getProjectionType */
		BS_SCRIPT_EXPORT(n:ProjectionType,pr:getter,order:-2)
		virtual ProjectionType GetProjectionType() const { return mInternal->getProjectionType(); }

		/** @copydoc Camera::setOrthoWindow */
		virtual void SetOrthoWindow(float w, float h) { mInternal->setOrthoWindow(w, h); }

		/** @copydoc Camera::setOrthoWindowHeight */
		BS_SCRIPT_EXPORT(n:OrthoHeight,pr:setter,order:-1)
		virtual void SetOrthoWindowHeight(float h) { mInternal->setOrthoWindowHeight(h); }

		/** @copydoc Camera::getOrthoWindowHeight */
		BS_SCRIPT_EXPORT(n:OrthoHeight,pr:getter,order:-1)
		virtual float GetOrthoWindowHeight() const { return mInternal->getOrthoWindowHeight(); }

		/** @copydoc Camera::setOrthoWindowWidth */
		BS_SCRIPT_EXPORT(n:OrthoWidth,pr:setter,hide)
		virtual void SetOrthoWindowWidth(float w) { mInternal->setOrthoWindowWidth(w); }

		/** @copydoc Camera::getOrthoWindowWidth */
		BS_SCRIPT_EXPORT(n:OrthoWidth,pr:getter,hide)
		virtual float GetOrthoWindowWidth() const { return mInternal->getOrthoWindowWidth(); }

		/** @copydoc Camera::setPriority */
		BS_SCRIPT_EXPORT(n:Priority,pr:setter)
		void SetPriority(INT32 priority) { mInternal->setPriority(priority); }

		/** @copydoc Camera::getPriority */
		BS_SCRIPT_EXPORT(n:Priority,pr:getter)
		INT32 GetPriority() const { return mInternal->getPriority(); }

		/** @copydoc Camera::setLayers */
		BS_SCRIPT_EXPORT(n:Layers,pr:setter,layerMask)
		void SetLayers(UINT64 layers) { mInternal->setLayers(layers); }

		/** @copydoc Camera::getLayers */
		BS_SCRIPT_EXPORT(n:Layers,pr:getter,layerMask)
		UINT64 GetLayers() const { return mInternal->getLayers(); }

		/** @copydoc Camera::setMSAACount */
		BS_SCRIPT_EXPORT(n:SampleCount,pr:setter)
		void SetMSAACount(UINT32 count) { mInternal->setMSAACount(count); }

		/** @copydoc Camera::getMSAACount */
		BS_SCRIPT_EXPORT(n:SampleCount,pr:getter)
		UINT32 GetMSAACount() const { return mInternal->getMSAACount(); }

		/** @copydoc Camera::setRenderSettings() */
		BS_SCRIPT_EXPORT(n:RenderSettings,pr:setter,applyOnDirty)
		void SetRenderSettings(const SPtr<RenderSettings>& settings) { mInternal->setRenderSettings(settings); }

		/** @copydoc Camera::getRenderSettings() */
		BS_SCRIPT_EXPORT(n:RenderSettings,pr:getter,applyOnDirty)
		const SPtr<RenderSettings>& GetRenderSettings() const { return mInternal->getRenderSettings(); }

		/** @copydoc Camera::notifyNeedsRedraw() */
		BS_SCRIPT_EXPORT()
		void NotifyNeedsRedraw() { mInternal->notifyNeedsRedraw(); }
		
		/** @copydoc Camera::worldToScreenPoint */
		BS_SCRIPT_EXPORT()
		Vector2I WorldToScreenPoint(const Vector3& worldPoint) const { updateView(); return mInternal->worldToScreenPoint(worldPoint); }

		/** @copydoc Camera::worldToNdcPoint */
		BS_SCRIPT_EXPORT()
		Vector2 WorldToNdcPoint(const Vector3& worldPoint) const { updateView(); return mInternal->worldToNdcPoint(worldPoint); }

		/** @copydoc Camera::worldToViewPoint */
		BS_SCRIPT_EXPORT()
		Vector3 WorldToViewPoint(const Vector3& worldPoint) const { updateView(); return mInternal->worldToViewPoint(worldPoint); }

		/** @copydoc Camera::screenToWorldPoint */
		BS_SCRIPT_EXPORT()
		Vector3 ScreenToWorldPoint(const Vector2I& screenPoint, float depth = 0.5f) const { updateView(); return mInternal->screenToWorldPoint(screenPoint, depth); }

		/** @copydoc Camera::screenToViewPoint */
		BS_SCRIPT_EXPORT()
		Vector3 ScreenToViewPoint(const Vector2I& screenPoint, float depth = 0.5f) const { return mInternal->screenToViewPoint(screenPoint, depth); }

		/** @copydoc Camera::screenToNdcPoint */
		BS_SCRIPT_EXPORT()
		Vector2 ScreenToNdcPoint(const Vector2I& screenPoint) const { return mInternal->screenToNdcPoint(screenPoint); }

		/** @copydoc Camera::viewToWorldPoint */
		BS_SCRIPT_EXPORT()
		Vector3 ViewToWorldPoint(const Vector3& viewPoint) const { updateView(); return mInternal->viewToWorldPoint(viewPoint); }

		/** @copydoc Camera::viewToScreenPoint */
		BS_SCRIPT_EXPORT()
		Vector2I ViewToScreenPoint(const Vector3& viewPoint) const { return mInternal->viewToScreenPoint(viewPoint); }

		/** @copydoc Camera::viewToNdcPoint */
		BS_SCRIPT_EXPORT()
		Vector2 ViewToNdcPoint(const Vector3& viewPoint) const { return mInternal->viewToNdcPoint(viewPoint); }

		/** @copydoc Camera::ndcToWorldPoint */
		BS_SCRIPT_EXPORT()
		Vector3 NdcToWorldPoint(const Vector2& ndcPoint, float depth = 0.5f) const { updateView(); return mInternal->ndcToWorldPoint(ndcPoint, depth); }

		/** @copydoc Camera::ndcToViewPoint */
		BS_SCRIPT_EXPORT()
		Vector3 NdcToViewPoint(const Vector2& ndcPoint, float depth = 0.5f) const { return mInternal->ndcToViewPoint(ndcPoint, depth); }

		/** @copydoc Camera::ndcToScreenPoint */
		BS_SCRIPT_EXPORT()
		Vector2I NdcToScreenPoint(const Vector2& ndcPoint) const { return mInternal->ndcToScreenPoint(ndcPoint); }

		/** @copydoc Camera::screenPointToRay */
		BS_SCRIPT_EXPORT()
		Ray ScreenPointToRay(const Vector2I& screenPoint) const { updateView(); return mInternal->screenPointToRay(screenPoint); }

		/** @copydoc Camera::projectPoint */
		BS_SCRIPT_EXPORT()
		Vector3 ProjectPoint(const Vector3& point) const { return mInternal->projectPoint(point); }

		/** @copydoc Camera::unprojectPoint */
		BS_SCRIPT_EXPORT()
		Vector3 UnprojectPoint(const Vector3& point) const { return mInternal->unprojectPoint(point); }

		/** @copydoc Camera::setMain */	
		BS_SCRIPT_EXPORT(n:Main,pr:setter)
		void SetMain(bool main);

		/** @copydoc Camera::isMain */
		BS_SCRIPT_EXPORT(n:Main,pr:getter)
		bool IsMain() const { return mInternal->isMain(); }

		/** @name Internal
		 *  @{
		 */

		/** Returns the internal camera that is used for majority of operations by this component. */
		SPtr<Camera> _getCamera() const { updateView(); return mInternal; }

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

		/** @copydoc Component::_instantiate */
		void _instantiate() override;

		/** @copydoc Component::onInitialized */
		void OnInitialized() override;

		/** @copydoc Component::onDestroyed */
		void OnDestroyed() override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CCameraRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CCamera();// Serialization only
	 };

	 /** @} */
}
