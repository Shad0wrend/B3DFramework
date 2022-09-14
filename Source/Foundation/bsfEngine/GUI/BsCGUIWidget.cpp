//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsCGUIWidget.h"
#include "Private/RTTI/BsCGUIWidgetRTTI.h"
#include "GUI/BsGUIWidget.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCCamera.h"

namespace bs
{
	CGUIWidget::CGUIWidget()
	{
		setFlag(ComponentFlag::AlwaysRun, true);
	}

	CGUIWidget::CGUIWidget(const HSceneObject& parent, const SPtr<Camera>& camera)
		:Component(parent), mCamera(camera), mParentHash((UINT32)-1)
	{
		setFlag(ComponentFlag::AlwaysRun, true);

		mInternal = GUIWidget::Create(camera);
		mOwnerTargetResizedConn = mInternal->onOwnerTargetResized.connect(
			std::bind(&CGUIWidget::ownerTargetResized, this));
		mOwnerWindowFocusChangedConn = mInternal->onOwnerWindowFocusChanged.connect(
			std::bind(&CGUIWidget::ownerWindowFocusChanged, this));
	}

	CGUIWidget::CGUIWidget(const HSceneObject& parent, const HCamera& camera)
		:CGUIWidget(parent, camera->GetCameraInternal())
	{ }

	void CGUIWidget::SetSkin(const HGUISkin& skin)
	{
		mInternal->setSkin(skin);
	}

	const GUISkin& CGUIWidget::GetSkin() const
	{
		return mInternal->getSkin();
	}

	const HGUISkin& CGUIWidget::GetSkinResource() const
	{
		return mInternal->getSkinResource();
	}

	GUIPanel* CGUIWidget::GetPanel() const
	{
		return mInternal->getPanel();
	}

	UINT8 CGUIWidget::GetDepth() const
	{
		return mInternal->getDepth();
	}

	void CGUIWidget::SetDepth(UINT8 depth)
	{
		mInternal->setDepth(depth);
	}

	bool CGUIWidget::InBounds(const Vector2I& position) const
	{
		return mInternal->inBounds(position);
	}

	const Rect2I& CGUIWidget::GetBounds() const
	{
		return mInternal->getBounds();
	}

	Viewport* CGUIWidget::GetTarget() const
	{
		return mInternal->getTarget();
	}

	SPtr<Camera> CGUIWidget::GetCamera() const
	{
		return mInternal->getCamera();
	}

	const Vector<GUIElement*>& CGUIWidget::GetElements() const
	{
		return mInternal->getElements();
	}

	void CGUIWidget::Update()
	{
		HSceneObject parent = SO();

		UINT32 curHash = parent->getTransformHash();
		if (curHash != mParentHash)
		{
			mInternal->UpdateTransformInternal(parent);
			mParentHash = curHash;
		}

		if (parent->getActive() != mInternal->getIsActive())
			mInternal->setIsActive(parent->getActive());

		mInternal->UpdateRTInternal();
	}

	void CGUIWidget::OnDestroyed()
	{
		mOwnerTargetResizedConn.disconnect();
		mOwnerWindowFocusChangedConn.disconnect();
		mInternal = nullptr;
	}

	RTTITypeBase* CGUIWidget::GetRttiStatic()
	{
		return CGUIWidgetRTTI::Instance();
	}

	RTTITypeBase* CGUIWidget::GetRtti() const
	{
		return CGUIWidget::GetRttiStatic();
	}
}
