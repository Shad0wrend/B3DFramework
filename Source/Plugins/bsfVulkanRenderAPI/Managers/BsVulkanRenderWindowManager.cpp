//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsVulkanRenderWindowManager.h"

#if B3D_PLATFORM == B3D_PLATFORM_ID_WIN32
#	include "Win32/BsWin32RenderWindow.h"
#elif B3D_PLATFORM == B3D_PLATFORM_ID_LINUX
#	include "Linux/BsLinuxRenderWindow.h"
#elif B3D_PLATFORM == B3D_PLATFORM_ID_MACOS
#	include "MacOS/BsMacOSRenderWindow.h"
#endif

using namespace bs;

SPtr<RenderWindow> VulkanRenderWindowManager::CreateImplementation(const RenderWindowCreateInformation& createInformation, u32 windowId, const SPtr<RenderWindow>& parentWindow)
{
	// Create the window
#if B3D_PLATFORM == B3D_PLATFORM_ID_WIN32
	Win32RenderWindow* renderWindow = new(B3DAllocate<Win32RenderWindow>()) Win32RenderWindow(createInformation, windowId, parentWindow);
	return B3DMakeSharedFromExisting<Win32RenderWindow>(renderWindow);
#elif B3D_PLATFORM == B3D_PLATFORM_ID_LINUX
	LinuxRenderWindow* renderWindow = new(B3DAllocate<LinuxRenderWindow>()) LinuxRenderWindow(createInformation, windowId, parentWindow);
	return B3DMakeSharedFromExisting<LinuxRenderWindow>(renderWindow);
#elif B3D_PLATFORM == B3D_PLATFORM_ID_MACOS
	MacOSRenderWindow* renderWindow = new(B3DAllocate<MacOSRenderWindow>()) MacOSRenderWindow(createInformation, windowId, parentWindow);
	return B3DMakeSharedFromExisting<MacOSRenderWindow>(renderWindow);
#endif
}
