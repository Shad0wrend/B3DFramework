//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsDragAndDrop.h"
#include "Platform/BsPlatform.h"
#include "BsCoreApplication.h"
#include "Utility/BsTime.h"
#include "Private/RTTI/BsDragAndDropDataRTTI.h"

using namespace std::placeholders;

using namespace bs;

RTTIType* DragAndDropData::GetRttiStatic()
{
	return DragAndDropDataRTTI::Instance();
}

RTTIType* DragAndDropData::GetRtti() const
{
	return GetRttiStatic();
}

RTTIType* SceneObjectDragAndDropData::GetRttiStatic()
{
	return SceneObjectDragAndDropDataRTTI::Instance();
}

RTTIType* SceneObjectDragAndDropData::GetRtti() const
{
	return GetRttiStatic();
}

RTTIType* ResourceDragAndDropData::GetRttiStatic()
{
	return ResourceDragAndDropDataRTTI::Instance();
}

RTTIType* ResourceDragAndDropData::GetRtti() const
{
	return GetRttiStatic();
}

DragAndDrop::DragAndDrop()
{
	mMouseCaptureChangedConn = Platform::onMouseCaptureChanged.Connect(std::bind(&DragAndDrop::mCaptureChanged, this));
	Input::Instance().OnPointerReleased.Connect(std::bind(&DragAndDrop::CursorReleased, this, _1));
}

DragAndDrop::~DragAndDrop()
{
	mMouseCaptureChangedConn.Disconnect();
}

void DragAndDrop::AddDropCallback(std::function<void(bool)>&& dropCallback)
{
	mDropCallbacks.emplace_back(std::move(dropCallback));
}

void DragAndDrop::StartDrag(const SPtr<DragAndDropData>& data, std::function<void(bool)>&& dropCallback, bool needsValidDropTarget)
{
	if(IsDropInProgress())
		EndDrag(false);

	mDragData = data;
	mNeedsValidDropTarget = needsValidDropTarget;

	if(dropCallback != nullptr)
		AddDropCallback(std::move(dropCallback));

	mCaptureActive.store(false);
	mCaptureChanged.store(false);

	Platform::CaptureMouse(*GetCoreApplication().GetPrimaryWindow());
}

void DragAndDrop::Update()
{
	// This only stays active for a single frame
	if(IsDropInProgress() && mDroppedFrameIndex < Time::Instance().GetCurrentFrameIndex())
		mDropData = nullptr;

	if(IsDragInProgress())
	{
		// This generally happens when window loses focus and capture is lost (for example alt+tab)
		int captureActive = mCaptureActive.load();
		if(!captureActive && mCaptureChanged.load() &&
		   (GetTime().GetCurrentFrameIndex() > mCaptureChangeFrame.load())) // Wait one frame to ensure input (like mouse up) gets a chance to be processed
		{
			EndDrag(false);
			mCaptureChanged.store(false);
		}
	}
}

void DragAndDrop::EndDrag(bool processed)
{
	for(auto& callback : mDropCallbacks)
		callback(processed);

	mDropData = mDragData;
	mDragData = nullptr;
	mDroppedFrameIndex = Time::Instance().GetCurrentFrameIndex();

	mDropCallbacks.clear();
}

void DragAndDrop::MouseCaptureChanged()
{
	mCaptureActive.fetch_xor(1); // mCaptureActive = !mCaptureActive;
	mCaptureChanged.store(true);
	mCaptureChangeFrame.store(GetTime().GetCurrentFrameIndex());
}

void DragAndDrop::CursorReleased(const PointerEvent& event)
{
	if(!IsDragInProgress())
		return;

	if(!OnDragEnded.Empty())
	{
		DragCallbackInfo info;
		OnDragEnded(event, info);

		EndDrag(info.Processed);
	}
	else
		EndDrag(false);

	Platform::ReleaseMouseCapture();
}
