//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Win32/BsWin32RenderWindow.h"

#include "BsCoreApplication.h"
#include "Private/Win32/BsWin32Platform.h"
#include "Private/Win32/BsWin32Window.h"
#include "Win32/BsWin32VideoModeInfo.h"
#include "CoreObject/BsRenderThread.h"
#include "Profiling/BsRenderStats.h"
#include "Managers/BsRenderWindowManager.h"
#include "BsVulkanGpuDevice.h"
#include "BsVulkanSwapChain.h"
#include "BsVulkanGpuCommandBuffer.h"
#include "BsVulkanGpuBackend.h"
#include "BsVulkanGpuQueue.h"
#include "BsVulkanSubmitThread.h"
#include "Math/BsMath.h"

using namespace bs;

Win32RenderWindow::Win32RenderWindow(const RenderWindowCreateInformation& createInformation, u32 windowId, const SPtr<RenderWindow>& parentWindow)
	: RenderWindow(createInformation, windowId, parentWindow), mProperties(createInformation)
{}

Vector2I Win32RenderWindow::ScreenToWindowPosition(const Vector2I& screenPos) const
{
	POINT pos;
	pos.x = screenPos.X;
	pos.y = screenPos.Y;

	HWND hwnd = (HWND)GetPlatformWindowHandle();

	ScreenToClient(hwnd, &pos);
	return Vector2I(pos.x, pos.y);
}

Vector2I Win32RenderWindow::WindowToScreenPosition(const Vector2I& windowPos) const
{
	POINT pos;
	pos.x = windowPos.X;
	pos.y = windowPos.Y;

	HWND hwnd = (HWND)GetPlatformWindowHandle();

	ClientToScreen(hwnd, &pos);
	return Vector2I(pos.x, pos.y);
}

SPtr<ct::Win32RenderWindow> Win32RenderWindow::GetImplementationRenderProxy() const
{
	return std::static_pointer_cast<ct::Win32RenderWindow>(mRenderProxy);
}

u64 Win32RenderWindow::GetPlatformWindowHandle() const
{
	BlockUntilRenderProxyInitialized();
	return GetImplementationRenderProxy()->GetPlatformWindowHandle();
}

void Win32RenderWindow::SyncProperties()
{
	ScopedSpinLock lock(GetImplementationRenderProxy()->mLock);
	mProperties = GetImplementationRenderProxy()->mSyncedProperties;
}

SPtr<ct::RenderProxy> Win32RenderWindow::CreateRenderProxy() const
{
	SPtr<RenderWindow> parentWindow = mParentWindow.lock();
	B3D_ENSURE(B3DIsWeakUnassigned(mParentWindow) || !mParentWindow.expired()); // If parent window is assigned, it must not be expired

	RenderWindowCreateInformation createInformation = mCreateInformation;
	SPtr<ct::RenderProxy> renderProxy = B3DMakeShared<ct::Win32RenderWindow>(createInformation, mWindowId, B3DGetRenderProxy(parentWindow));
	renderProxy->SetShared(renderProxy);

	return renderProxy;
}

namespace bs {
namespace ct {
Win32RenderWindow::Win32RenderWindow(const RenderWindowCreateInformation& createInformation, u32 windowId, const SPtr<RenderWindow>& parentWindow)
	: RenderWindow(createInformation, windowId, parentWindow), mProperties(createInformation), mSyncedProperties(createInformation), mWindow(nullptr), mIsChild(false), mShowOnSwap(false), mDisplayFrequency(0)
{}

Win32RenderWindow::~Win32RenderWindow()
{
	GetVulkanSubmitThread().WaitUntilIdle();

	if(mWindow != nullptr)
	{
		B3DDelete(mWindow);
		mWindow = nullptr;
	}

	mSwapChain->Destroy();

	Platform::ResetNonClientAreas(*this);
}

void Win32RenderWindow::Initialize()
{
	RenderWindowProperties& props = mProperties;

	// Create a window
	WindowCreateInformation windowCreateInformation;
	windowCreateInformation.ShowTitleBar = mCreateInformation.ShowTitleBar;
	windowCreateInformation.ShowBorder = mCreateInformation.ShowBorder;
	windowCreateInformation.AllowResize = mCreateInformation.AllowResize;
	windowCreateInformation.EnableDoubleClick = true;
	windowCreateInformation.Fullscreen = mCreateInformation.Fullscreen;
	windowCreateInformation.Width = mCreateInformation.VideoMode.Width;
	windowCreateInformation.Height = mCreateInformation.VideoMode.Height;
	windowCreateInformation.Hidden = mCreateInformation.Hidden || mCreateInformation.HideUntilSwap;
	windowCreateInformation.Left = mCreateInformation.Left;
	windowCreateInformation.Top = mCreateInformation.Top;
	windowCreateInformation.OuterDimensions = false;
	windowCreateInformation.Title = mCreateInformation.Title;
	windowCreateInformation.ToolWindow = mCreateInformation.ToolWindow;
	windowCreateInformation.CreationParams = this;
	windowCreateInformation.Modal = mCreateInformation.Modal;
	windowCreateInformation.WndProc = &Win32Platform::Win32WndProcInternal;

#ifdef BS_STATIC_LIB
	windowDesc.module = GetModuleHandle(NULL);
#else
	windowCreateInformation.Module = GetModuleHandle("bsfVulkanRenderAPI.dll");
#endif

	if(!B3DIsWeakUnassigned(mParentWindow))
	{
		const SPtr<Win32RenderWindow> parentWindow = std::static_pointer_cast<Win32RenderWindow>(mParentWindow.lock());
		if(B3D_ENSURE(parentWindow != nullptr))
			windowCreateInformation.Parent = (HWND)parentWindow->GetPlatformWindowHandle();
	}

	const Win32VideoModeInfo& videoModeInfo = static_cast<const Win32VideoModeInfo&>(GetCoreApplication().GetPrimaryGpuDevice()->GetVideoModeInfo());
	u32 outputCount = videoModeInfo.GetOutputCount();
	if(outputCount > 0)
	{
		u32 actualMonitorIdx = std::min(mCreateInformation.VideoMode.OutputIdx, outputCount - 1);
		const Win32VideoOutputInfo& outputInfo = static_cast<const Win32VideoOutputInfo&>(videoModeInfo.GetOutputInfo(actualMonitorIdx));
		windowCreateInformation.Monitor = outputInfo.GetMonitorHandle();
	}

	// Must be set before creating a window, since wndProc will call ShowWindow if needed after creation
	if(!windowCreateInformation.External)
	{
		mShowOnSwap = mCreateInformation.HideUntilSwap && !mCreateInformation.Hidden;
		props.IsHidden = mCreateInformation.HideUntilSwap || mCreateInformation.Hidden;
	}

	mWindow = B3DNew<Win32Window>(windowCreateInformation);

	mIsChild = windowCreateInformation.Parent != nullptr;
	mDisplayFrequency = Math::RoundToI32(mCreateInformation.VideoMode.RefreshRate);

	// Update local properties
	props.IsFullScreen = mCreateInformation.Fullscreen && !mIsChild;
	props.Width = mWindow->GetWidth();
	props.Height = mWindow->GetHeight();
	props.Top = mWindow->GetTop();
	props.Left = mWindow->GetLeft();
	props.HwGamma = mCreateInformation.Gamma;
	props.MultisampleCount = 1;

	// Create Vulkan surface
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hwnd = mWindow->GetHWnd();
	surfaceCreateInfo.hinstance = windowCreateInformation.Module;

	VkInstance instance = GetVulkanGpuBackend().GetVkInstance();
	VkSurfaceKHR vkSurface;
	VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, gVulkanAllocator, &vkSurface);
	B3D_ASSERT(result == VK_SUCCESS);

	mSurface = B3DMakeShared<VulkanSurface>(vkSurface);

	SPtr<VulkanGpuDevice> presentDevice = GetVulkanGpuBackend().GetPresentDevice();
	VkPhysicalDevice physicalDevice = presentDevice->GetPhysical();

	mPresentQueueFamily = presentDevice->GetQueueFamily(GQT_GRAPHICS);

	VkBool32 supportsPresent;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, mPresentQueueFamily, vkSurface, &supportsPresent);

	if(!supportsPresent)
	{
		// Note: Not supporting present only queues at the moment
		// Note: Also present device can only return one family of graphics queue, while there could be more (some of
		// which support present)
		B3D_EXCEPT(RenderingAPIException, "Cannot find a graphics queue that also supports present operations.");
	}

	SurfaceFormat format = presentDevice->GetSurfaceFormat(vkSurface, mCreateInformation.Gamma);
	mColorFormat = format.ColorFormat;
	mColorSpace = format.ColorSpace;
	mDepthFormat = format.DepthFormat;

	// Create swap chain
	mSwapChain = presentDevice->GetResourceManager().Create<VulkanSwapChain>(mSurface, props.Width, props.Height, props.Vsync, mColorFormat, mColorSpace, mCreateInformation.DepthBuffer, mDepthFormat);

	// Make the window full screen if required
	if(!windowCreateInformation.External)
	{
		if(props.IsFullScreen)
		{
			DEVMODE displayDeviceMode;

			memset(&displayDeviceMode, 0, sizeof(displayDeviceMode));
			displayDeviceMode.dmSize = sizeof(DEVMODE);
			displayDeviceMode.dmBitsPerPel = 32;
			displayDeviceMode.dmPelsWidth = props.Width;
			displayDeviceMode.dmPelsHeight = props.Height;
			displayDeviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if(mDisplayFrequency)
			{
				displayDeviceMode.dmDisplayFrequency = mDisplayFrequency;
				displayDeviceMode.dmFields |= DM_DISPLAYFREQUENCY;

				if(ChangeDisplaySettingsEx(NULL, &displayDeviceMode, NULL, CDS_FULLSCREEN | CDS_TEST, NULL) != DISP_CHANGE_SUCCESSFUL)
				{
					B3D_EXCEPT(RenderingAPIException, "ChangeDisplaySettings with user display frequency failed.");
				}
			}

			if(ChangeDisplaySettingsEx(NULL, &displayDeviceMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
			{
				B3D_EXCEPT(RenderingAPIException, "ChangeDisplaySettings failed.");
			}
		}
	}

	{
		ScopedSpinLock lock(mLock);
		mSyncedProperties = props;
	}

	bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
	RenderWindow::Initialize();
}

void Win32RenderWindow::Move(i32 left, i32 top)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	RenderWindowProperties& props = mProperties;

	if(!props.IsFullScreen)
	{
		mWindow->Move(left, top);

		props.Top = mWindow->GetTop();
		props.Left = mWindow->GetLeft();

		{
			ScopedSpinLock lock(mLock);
			mSyncedProperties.Top = props.Top;
			mSyncedProperties.Left = props.Left;
		}

		bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
	}
}

void Win32RenderWindow::Resize(u32 width, u32 height)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	RenderWindowProperties& props = mProperties;

	if(!props.IsFullScreen)
	{
		mWindow->Resize(width, height);

		props.Width = mWindow->GetWidth();
		props.Height = mWindow->GetHeight();

		{
			ScopedSpinLock lock(mLock);
			mSyncedProperties.Width = props.Width;
			mSyncedProperties.Height = props.Height;
		}

		bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
	}
}

void Win32RenderWindow::SetActive(bool state)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	mWindow->SetActive(state);

	RenderWindow::SetActive(state);
}

void Win32RenderWindow::SetHidden(bool hidden)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	mShowOnSwap = false;
	mWindow->SetHidden(hidden);

	RenderWindow::SetHidden(hidden);
}

void Win32RenderWindow::Minimize()
{
	ASSERT_IF_NOT_RENDER_THREAD;

	mWindow->Minimize();
}

void Win32RenderWindow::Maximize()
{
	ASSERT_IF_NOT_RENDER_THREAD;

	mWindow->Maximize();
}

void Win32RenderWindow::Restore()
{
	ASSERT_IF_NOT_RENDER_THREAD;

	mWindow->Restore();
}

void Win32RenderWindow::SetFullscreen(u32 width, u32 height, float refreshRate, u32 monitorIdx)
{
	ASSERT_IF_NOT_RENDER_THREAD

	if(mIsChild)
		return;

	const Win32VideoModeInfo& videoModeInfo = static_cast<const Win32VideoModeInfo&>(GetCoreApplication().GetPrimaryGpuDevice()->GetVideoModeInfo());
	const u32 outputCount = videoModeInfo.GetOutputCount();
	if(outputCount == 0)
		return;

	RenderWindowProperties& props = mProperties;

	u32 actualMonitorIdx = std::min(monitorIdx, outputCount - 1);
	const Win32VideoOutputInfo& outputInfo = static_cast<const Win32VideoOutputInfo&>(videoModeInfo.GetOutputInfo(actualMonitorIdx));

	mDisplayFrequency = Math::RoundToI32(refreshRate);
	props.IsFullScreen = true;

	DEVMODE displayDeviceMode;

	memset(&displayDeviceMode, 0, sizeof(displayDeviceMode));
	displayDeviceMode.dmSize = sizeof(DEVMODE);
	displayDeviceMode.dmBitsPerPel = 32;
	displayDeviceMode.dmPelsWidth = width;
	displayDeviceMode.dmPelsHeight = height;
	displayDeviceMode.dmDisplayFrequency = mDisplayFrequency;
	displayDeviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

	HMONITOR hMonitor = outputInfo.GetMonitorHandle();
	MONITORINFOEX monitorInfo;

	memset(&monitorInfo, 0, sizeof(MONITORINFOEX));
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &monitorInfo);

	if(ChangeDisplaySettingsEx(monitorInfo.szDevice, &displayDeviceMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
		B3D_LOG(Error, Platform, "ChangeDisplaySettings failed");

	props.Top = monitorInfo.rcMonitor.top;
	props.Left = monitorInfo.rcMonitor.left;
	props.Width = width;
	props.Height = height;

	SetWindowLong(mWindow->GetHWnd(), GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowLong(mWindow->GetHWnd(), GWL_EXSTYLE, 0);

	SetWindowPos(mWindow->GetHWnd(), HWND_TOP, props.Left, props.Top, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

void Win32RenderWindow::SetFullscreen(const VideoMode& mode)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	SetFullscreen(mode.Width, mode.Height, mode.RefreshRate, mode.OutputIdx);
}

void Win32RenderWindow::SetWindowed(u32 width, u32 height)
{
	ASSERT_IF_NOT_RENDER_THREAD;

	RenderWindowProperties& props = mProperties;

	if(!props.IsFullScreen)
		return;

	props.IsFullScreen = false;
	props.Width = width;
	props.Height = height;

	// Drop out of fullscreen
	ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);

	u32 winWidth = width;
	u32 winHeight = height;

	RECT rect;
	SetRect(&rect, 0, 0, winWidth, winHeight);

	AdjustWindowRect(&rect, mWindow->GetStyle(), false);
	winWidth = rect.right - rect.left;
	winHeight = rect.bottom - rect.top;

	// Deal with centering when switching down to smaller resolution
	HMONITOR hMonitor = MonitorFromWindow(mWindow->GetHWnd(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo;
	memset(&monitorInfo, 0, sizeof(MONITORINFO));
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &monitorInfo);

	LONG screenw = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
	LONG screenh = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;

	i32 left = screenw > i32(winWidth) ? ((screenw - i32(winWidth)) / 2) : 0;
	i32 top = screenh > i32(winHeight) ? ((screenh - i32(winHeight)) / 2) : 0;

	SetWindowLong(mWindow->GetHWnd(), GWL_STYLE, mWindow->GetStyle() | WS_VISIBLE);
	SetWindowLong(mWindow->GetHWnd(), GWL_EXSTYLE, mWindow->GetStyleEx());

	SetWindowPos(mWindow->GetHWnd(), HWND_NOTOPMOST, left, top, winWidth, winHeight, SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOACTIVATE);

	{
		ScopedSpinLock lock(mLock);
		mSyncedProperties.Width = props.Width;
		mSyncedProperties.Height = props.Height;
	}

	bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
}

void Win32RenderWindow::SetVSync(bool enabled, u32 interval)
{
	mProperties.Vsync = enabled;
	mProperties.VsyncInterval = interval;

	if(mSwapChain != nullptr)
		mSwapChain->MarkAsInvalid();

	{
		ScopedSpinLock lock(mLock);
		mSyncedProperties.Vsync = enabled;
		mSyncedProperties.VsyncInterval = interval;
	}

	bs::RenderWindowManager::Instance().NotifySyncDataDirty(this);
}

void Win32RenderWindow::SwapBuffers()
{
	ASSERT_IF_NOT_RENDER_THREAD

	if(mShowOnSwap)
		SetHidden(false);
}

u64 Win32RenderWindow::GetPlatformWindowHandle() const
{
	return (u64)mWindow->GetHWnd();
}

void Win32RenderWindow::DoOnWindowMovedOrResized()
{
	ASSERT_IF_NOT_RENDER_THREAD

	if(!mWindow)
		return;

	mWindow->DoOnWindowMovedOrResized();

	RenderWindowProperties& props = mProperties;
	if(!props.IsFullScreen) // Fullscreen is handled directly by this object
	{
		props.Top = mWindow->GetTop();
		props.Left = mWindow->GetLeft();
		props.Width = mWindow->GetWidth();
		props.Height = mWindow->GetHeight();
	}
}

void Win32RenderWindow::SyncProperties()
{
	ScopedSpinLock lock(mLock);
	mProperties = mSyncedProperties;
}

void Win32RenderWindow::RebuildSwapChain()
{
	GetVulkanSubmitThread().WaitUntilIdle();

	SPtr<VulkanGpuDevice> presentDevice = GetVulkanGpuBackend().GetPresentDevice();
	VulkanSwapChain* oldSwapChain = mSwapChain;
	oldSwapChain->MarkAsRetired();

	mSwapChain = presentDevice->GetResourceManager().Create<VulkanSwapChain>(mSurface, mProperties.Width, mProperties.Height, mProperties.Vsync, mColorFormat, mColorSpace, mCreateInformation.DepthBuffer, mDepthFormat, oldSwapChain);
	oldSwapChain->Destroy();

	OnSwapChainDidRebuild();
}
}} // namespace bs::ct
