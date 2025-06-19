//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsD3D11RenderWindowManager.h"
#include "BsD3D11RenderAPI.h"
#include "BsD3D11RenderWindow.h"
#include "Threading/BsAsyncOp.h"

using namespace b3d;

D3D11RenderWindowManager::D3D11RenderWindowManager(ct::D3D11RenderAPI* renderSystem)
	: mRenderSystem(renderSystem)
{
	B3D_ASSERT(mRenderSystem != nullptr);
}

SPtr<RenderWindow> D3D11RenderWindowManager::CreateImpl(RENDER_WINDOW_DESC& desc, u32 windowId, const SPtr<RenderWindow>& parentWindow)
{
	ct::RenderAPI* rs = ct::RenderAPI::InstancePtr();
	ct::D3D11RenderAPI* d3d11rs = static_cast<ct::D3D11RenderAPI*>(rs);

	if(parentWindow != nullptr)
	{
		u64 hWnd;
		parentWindow->GetCustomAttribute("WINDOW", &hWnd);
		desc.PlatformSpecific["parentWindowHandle"] = ToString(hWnd);
	}

	// Create the window
	D3D11RenderWindow* renderWindow = new(B3DAllocate<D3D11RenderWindow>()) D3D11RenderWindow(desc, windowId);
	return B3DMakeCoreFromExisting<D3D11RenderWindow>(renderWindow);
}
