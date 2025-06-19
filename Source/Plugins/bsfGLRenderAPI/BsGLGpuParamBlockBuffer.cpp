//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGLGpuParamBlockBuffer.h"
#include "Profiling/BsRenderStats.h"

using namespace b3d;
using namespace b3d::ct;

GLGpuParamBlockBuffer::GLGpuParamBlockBuffer(u32 size, GpuBufferUsage usage, GpuDeviceFlags deviceMask)
	: GpuBuffer(size, usage, deviceMask)
{
	B3D_ASSERT((deviceMask == GDF_DEFAULT || deviceMask == GDF_PRIMARY) && "Multiple GPUs not supported natively on OpenGL.");
}

GLGpuParamBlockBuffer::~GLGpuParamBlockBuffer()
{
	if(mBuffer)
		B3DPoolDelete(static_cast<GLHardwareBuffer*>(mBuffer));
}

void GLGpuParamBlockBuffer::Initialize()
{
	mBuffer = B3DPoolNew<GLHardwareBuffer>(GL_UNIFORM_BUFFER, mSize, mUsage);
	GpuBuffer::Initialize();
}
