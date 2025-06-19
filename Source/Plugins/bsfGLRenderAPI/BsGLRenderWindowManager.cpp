//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGLRenderWindowManager.h"
#include "BsGLRenderAPI.h"
#include "BsGLSupport.h"
#include "Threading/BsAsyncOp.h"

using namespace b3d;

GLRenderWindowManager::GLRenderWindowManager(render::GLRenderAPI* renderSystem)
	: mRenderSystem(renderSystem)
{
	B3D_ASSERT(mRenderSystem != nullptr);
}

SPtr<RenderWindow> GLRenderWindowManager::CreateImpl(RENDER_WINDOW_DESC& desc, u32 windowId, const SPtr<RenderWindow>& parentWindow)
{
	render::GLSupport* glSupport = mRenderSystem->GetGlSupport();

	// Create the window
	return glSupport->NewWindow(desc, windowId, parentWindow);
}
