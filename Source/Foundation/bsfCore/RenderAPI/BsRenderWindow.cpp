//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsRenderWindow.h"
#include "CoreThread/BsCoreThread.h"
#include "Managers/BsRenderWindowManager.h"
#include "RenderAPI/BsViewport.h"
#include "Platform/BsPlatform.h"
#include "Private/RTTI/BsRenderTargetRTTI.h"

namespace bs
{
	RenderWindowProperties::RenderWindowProperties(const RENDER_WINDOW_DESC& desc)
	{
		width = desc.videoMode.width;
		height = desc.videoMode.height;
		hwGamma = desc.gamma;
		vsync = desc.vsync;
		vsyncInterval = desc.vsyncInterval;
		multisampleCount = desc.multisampleCount;
		left = desc.left;
		top = desc.top;
		isFullScreen = desc.fullscreen;
		isHidden = desc.hidden;
		isModal = desc.modal;
		isWindow = true;
		requiresTextureFlipping = false;
	}

	void RenderWindow::Destroy()
	{
		RenderWindowManager::Instance().NotifyWindowDestroyed(this);

		RenderTarget::Destroy();
	}

	RenderWindow::RenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId)
		:mDesc(desc), mWindowId(windowId)
	{

	}

	void RenderWindow::Resize(UINT32 width, UINT32 height)
	{
		std::function<void(SPtr<ct::RenderWindow>, UINT32, UINT32)> resizeFunc =
			[](SPtr<ct::RenderWindow> renderWindow, UINT32 width, UINT32 height)
		{
			renderWindow->Resize(width, height);
		};

		gCoreThread().QueueCommand(std::bind(resizeFunc, GetCore(), width, height));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	void RenderWindow::Move(INT32 left, INT32 top)
	{
		std::function<void(SPtr<ct::RenderWindow>, INT32, INT32)> moveFunc =
			[](SPtr<ct::RenderWindow> renderWindow, INT32 left, INT32 top)
		{
			renderWindow->Move(left, top);
		};

		gCoreThread().QueueCommand(std::bind(moveFunc, GetCore(), left, top));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.left = syncedProps.left;
			mutableProps.top = syncedProps.top;
		}
	}

	void RenderWindow::Hide()
	{
		std::function<void(SPtr<ct::RenderWindow>)> hideFunc =
			[](SPtr<ct::RenderWindow> renderWindow)
		{
			renderWindow->SetHidden(true);
		};

		GetMutableProperties().isHidden = true;

		gCoreThread().QueueCommand(std::bind(hideFunc, GetCore()));
	}

	void RenderWindow::Show()
	{
		std::function<void(SPtr<ct::RenderWindow>)> showFunc =
			[](SPtr<ct::RenderWindow> renderWindow)
		{
			renderWindow->SetHidden(false);
		};

		GetMutableProperties().isHidden = false;

		gCoreThread().QueueCommand(std::bind(showFunc, GetCore()));
	}

	void RenderWindow::Minimize()
	{
		std::function<void(SPtr<ct::RenderWindow>)> minimizeFunc =
			[](SPtr<ct::RenderWindow> renderWindow)
		{
			renderWindow->Minimize();
		};

		GetMutableProperties().isMaximized = false;

		gCoreThread().QueueCommand(std::bind(minimizeFunc, GetCore()));
	}

	void RenderWindow::Maximize()
	{
		std::function<void(SPtr<ct::RenderWindow>)> maximizeFunc =
			[](SPtr<ct::RenderWindow> renderWindow)
		{
			renderWindow->Maximize();
		};

		GetMutableProperties().isMaximized = true;

		gCoreThread().QueueCommand(std::bind(maximizeFunc, GetCore()));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	void RenderWindow::Restore()
	{
		std::function<void(SPtr<ct::RenderWindow>)> restoreFunc =
			[](SPtr<ct::RenderWindow> renderWindow)
		{
			renderWindow->Restore();
		};

		GetMutableProperties().isMaximized = false;

		gCoreThread().QueueCommand(std::bind(restoreFunc, GetCore()));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	void RenderWindow::SetFullscreen(UINT32 width, UINT32 height, float refreshRate, UINT32 monitorIdx)
	{
		std::function<void(SPtr<ct::RenderWindow>, UINT32, UINT32, float, UINT32)> fullscreenFunc =
			[](SPtr<ct::RenderWindow> renderWindow, UINT32 width, UINT32 height, float refreshRate, UINT32 monitorIdx)
		{
			renderWindow->SetFullscreen(width, height, refreshRate, monitorIdx);
		};

		gCoreThread().QueueCommand(std::bind(fullscreenFunc, GetCore(), width, height, refreshRate, monitorIdx));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	void RenderWindow::SetFullscreen(const VideoMode& mode)
	{
		std::function<void(SPtr<ct::RenderWindow>, const VideoMode&)> fullscreenFunc =
			[](SPtr<ct::RenderWindow> renderWindow, const VideoMode& mode)
		{
			renderWindow->SetFullscreen(mode);
		};

		gCoreThread().QueueCommand(std::bind(fullscreenFunc, GetCore(), std::cref(mode)));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	void RenderWindow::SetWindowed(UINT32 width, UINT32 height)
	{
		std::function<void(SPtr<ct::RenderWindow>, UINT32, UINT32)> windowedFunc =
			[](SPtr<ct::RenderWindow> renderWindow, UINT32 width, UINT32 height)
		{
			renderWindow->SetWindowed(width, height);
		};

		gCoreThread().QueueCommand(std::bind(windowedFunc, GetCore(), width, height));
		gCoreThread().Submit(true);

		{
			ScopedSpinLock lock(GetCore()->mLock);
			const RenderWindowProperties& syncedProps = GetCore()->GetSyncedProperties();
			RenderWindowProperties& mutableProps = GetMutableProperties();

			mutableProps.width = syncedProps.width;
			mutableProps.height = syncedProps.height;
		}
	}

	SPtr<ct::RenderWindow> RenderWindow::GetCore() const
	{
		return std::static_pointer_cast<ct::RenderWindow>(mCoreSpecific);
	}

	SPtr<RenderWindow> RenderWindow::Create(RENDER_WINDOW_DESC& desc, SPtr<RenderWindow> parentWindow)
	{
		return RenderWindowManager::Instance().Create(desc, parentWindow);
	}

	RenderWindowProperties& RenderWindow::GetMutableProperties()
	{
		return const_cast<RenderWindowProperties&>(GetProperties());
	}

	const RenderWindowProperties& RenderWindow::GetProperties() const
	{
		return static_cast<const RenderWindowProperties&>(GetPropertiesInternal());
	}

	void RenderWindow::NotifyWindowEventInternal(WindowEventType type)
	{
		THROW_IF_CORE_THREAD;

		ct::RenderWindow* coreWindow = GetCore().get();
		RenderWindowProperties& syncProps = coreWindow->GetSyncedProperties();
		RenderWindowProperties& props = const_cast<RenderWindowProperties&>(GetProperties());

		switch(type)
		{
			case WindowEventType::Resized:
			{
				WindowMovedOrResizedInternal();

				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.width = props.width;
					syncProps.height = props.height;
				}

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				RenderWindowManager::Instance().NotifyMovedOrResized(coreWindow);

				break;
			}
			case WindowEventType::Moved:
			{
				WindowMovedOrResizedInternal();

				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.top = props.top;
					syncProps.left = props.left;
				}

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				RenderWindowManager::Instance().NotifyMovedOrResized(coreWindow);

				break;
			}
			case WindowEventType::FocusReceived:
			{
				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.hasFocus = true;
				}

				props.hasFocus = true;

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				RenderWindowManager::Instance().NotifyFocusReceived(coreWindow);
				break;
			}
			case WindowEventType::FocusLost:
			{
				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.hasFocus = false;
				}

				props.hasFocus = false;

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				RenderWindowManager::Instance().NotifyFocusLost(coreWindow);
				break;
			}
			case WindowEventType::Minimized:
			{
				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.isMaximized = false;
				}

				props.isMaximized = false;

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				break;
			}
			case WindowEventType::Maximized:
			{
				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.isMaximized = true;
				}

				props.isMaximized = true;

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				break;
			}
			case WindowEventType::Restored:
			{
				{
					ScopedSpinLock lock(coreWindow->mLock);
					syncProps.isMaximized = false;
				}

				props.isMaximized = false;

				ct::RenderWindowManager::Instance().NotifySyncDataDirty(coreWindow);
				break;
			}
			case WindowEventType::MouseLeft:
			{
				RenderWindowManager::Instance().NotifyMouseLeft(coreWindow);
				break;
			}
			case WindowEventType::CloseRequested:
			{
				RenderWindowManager::Instance().NotifyCloseRequested(coreWindow);
				break;
			}
		}
	}

	void RenderWindow::OnExternalResizeInternal(UINT32 width, UINT32 height)
	{
		RenderWindowProperties& props = GetMutableProperties();
		props.width = width;
		props.height = height;
		NotifyWindowEventInternal(WindowEventType::Resized);
	}

	void RenderWindow::OnExternalMoveInternal(INT32 top, INT32 left)
	{
		RenderWindowProperties& props = GetMutableProperties();
		props.top = top;
		props.left = left;
		NotifyWindowEventInternal(WindowEventType::Moved);
	}

	void RenderWindow::OnExternalFocusInternal(bool focused)
	{
		if(focused)
		{
			NotifyWindowEventInternal(WindowEventType::FocusReceived);
		}
		else
		{
			NotifyWindowEventInternal(WindowEventType::FocusLost);
		}
	}

	void RenderWindow::OnExternalMaximizedInternal(bool maximized)
	{
		if(maximized)
		{
			NotifyWindowEventInternal(WindowEventType::Maximized);
		}
		else
		{
			NotifyWindowEventInternal(WindowEventType::Restored);
		}
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* RenderWindow::GetRttiStatic()
	{
		return RenderWindowRTTI::Instance();
	}

	RTTITypeBase* RenderWindow::GetRtti() const
	{
		return RenderWindow::GetRttiStatic();
	}

	namespace ct
	{
	RenderWindow::RenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId)
		:mDesc(desc), mWindowId(windowId)
	{
		RenderWindowManager::Instance().WindowCreated(this);
	}

	RenderWindow::~RenderWindow()
	{
		RenderWindowManager::Instance().WindowDestroyed(this);
	}

	void RenderWindow::SetHidden(bool hidden)
	{
		THROW_IF_NOT_CORE_THREAD;

		RenderWindowProperties& props = const_cast<RenderWindowProperties&>(GetProperties());

		props.isHidden = hidden;
		{
			ScopedSpinLock lock(mLock);
			GetSyncedProperties().isHidden = hidden;
		}

		bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
	}

	void RenderWindow::SetActive(bool state)
	{
		THROW_IF_NOT_CORE_THREAD;
	}

	void RenderWindow::NotifyWindowEventInternal(WindowEventType type)
	{
		THROW_IF_NOT_CORE_THREAD;

		RenderWindowProperties& syncProps = GetSyncedProperties();
		RenderWindowProperties& props = const_cast<RenderWindowProperties&>(GetProperties());

		switch(type)
		{
			case WindowEventType::Resized:
			{
				WindowMovedOrResizedInternal();

				{
					ScopedSpinLock lock(mLock);
					syncProps.width = props.width;
					syncProps.height = props.height;
				}

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				bs::RenderWindowManager::Instance().NotifyMovedOrResized(this);

				break;
			}
			case WindowEventType::Moved:
			{
				WindowMovedOrResizedInternal();

				{
					ScopedSpinLock lock(mLock);
					syncProps.top = props.top;
					syncProps.left = props.left;
				}

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				bs::RenderWindowManager::Instance().NotifyMovedOrResized(this);

				break;
			}
			case WindowEventType::FocusReceived:
			{
				{
					ScopedSpinLock lock(mLock);
					syncProps.hasFocus = true;
				}

				props.hasFocus = true;

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				bs::RenderWindowManager::Instance().NotifyFocusReceived(this);
				break;
			}
			case WindowEventType::FocusLost:
			{
				{
					ScopedSpinLock lock(mLock);
					syncProps.hasFocus = false;
				}

				props.hasFocus = false;

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				bs::RenderWindowManager::Instance().NotifyFocusLost(this);
				break;
			}
			case WindowEventType::Minimized:
			{
				{
					ScopedSpinLock lock(mLock);
					syncProps.isMaximized = false;
				}

				props.isMaximized = false;

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				break;
			}
			case WindowEventType::Maximized:
			{
				{
					ScopedSpinLock lock(mLock);
					syncProps.isMaximized = true;
				}

				props.isMaximized = true;

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				break;
			}
			case WindowEventType::Restored:
			{
				{
					ScopedSpinLock lock(mLock);
					syncProps.isMaximized = false;
				}

				props.isMaximized = false;

				bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
				break;
			}
			case WindowEventType::MouseLeft:
			{
				bs::RenderWindowManager::Instance().NotifyMouseLeft(this);
				break;
			}
			case WindowEventType::CloseRequested:
			{
				bs::RenderWindowManager::Instance().NotifyCloseRequested(this);
				break;
			}
		}
	}

	const RenderWindowProperties& RenderWindow::GetProperties() const
	{
		return static_cast<const RenderWindowProperties&>(GetPropertiesInternal());
	}
	}
}
