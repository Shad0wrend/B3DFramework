//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGLRenderAPIFactory.h"
#include "BsGLRenderAPI.h"

using namespace b3d;
using namespace b3d::render;

constexpr const char* GLRenderAPIFactory::kSystemName;

void GLRenderAPIFactory::Create()
{
	RenderAPI::StartUp<GLRenderAPI>();
}

GLRenderAPIFactory::InitOnStart GLRenderAPIFactory::initOnStart;
