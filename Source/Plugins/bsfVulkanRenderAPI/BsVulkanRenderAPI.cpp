//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsVulkanRenderAPI.h"
#include "CoreThread/BsCoreThread.h"
#include "Profiling/BsRenderStats.h"
#include "RenderAPI/BsGpuProgramParameterDescription.h"
#include "BsVulkanGpuDevice.h"
#include "Managers/BsVulkanTextureManager.h"
#include "Managers/BsVulkanRenderWindowManager.h"
#include "Managers/BsVulkanQueryManager.h"
#include "BsVulkanGpuCommandBuffer.h"
#include "BsVulkanGpuParameters.h"
#include "Managers/BsVulkanVertexInputManager.h"
#include "BsVulkanGpuBuffer.h"
#include "BsVulkanGpuQueue.h"
#include "BsVulkanFramebuffer.h"
#include "BsVulkanUtility.h"
#include "BsVulkanRenderPass.h"
#include "BsVulkanSubmitThread.h"
#include "BsVulkanSwapChain.h"
#include "Win32/BsWin32RenderWindow.h"

#include <vulkan/vulkan.h>


using namespace bs;
using namespace bs::ct;

const StringID& VulkanRenderAPI::GetName() const
{
	static StringID strName("VulkanRenderAPI");
	return strName;
}

void VulkanRenderAPI::Initialize()
{
	THROW_IF_NOT_CORE_THREAD;

	// TODO - Move this to CoreApplication once I get rid of VulkanRenderAPI
	GpuBackend::StartUp<VulkanGpuBackend>();

	// TODO - Currently the GpuBackend always only initializes a single device. Once we change it to support multiple, pick the device here and initialize it1
	mPrimaryGpuDevice = GpuBackend::Instance().GetDevice(0);
	mPrimaryGpuDevice->Initialize();

	RenderAPI::Initialize();
}

void VulkanRenderAPI::DestroyCore()
{
	THROW_IF_NOT_CORE_THREAD;

	GetVulkanSubmitThread().WaitUntilIdle(true);
	GetVulkanSubmitThread().RefreshCommandBufferCompletionStates();

	mPrimaryGpuDevice = nullptr;

	// TODO - Move this to CoreApplication once I get rid of VulkanRenderAPI
	GpuBackend::ShutDown();

	RenderAPI::DestroyCore();
}

void VulkanRenderAPI::BeginFrame()
{
	THROW_IF_NOT_CORE_THREAD

	GetVulkanSubmitThread().RefreshCommandBufferCompletionStates();
}

void VulkanRenderAPI::EndFrame()
{
	VulkanGpuDevice* const vulkanGpuDevice = static_cast<VulkanGpuDevice*>(mPrimaryGpuDevice.get());
	if(vulkanGpuDevice == nullptr)
		return;

	vulkanGpuDevice->SubmitTransferCommandBuffers();

	GetVulkanSubmitThread().QueueRefreshCommandBufferCompletionStates(vulkanGpuDevice);
}

void VulkanRenderAPI::SwapBuffers(const SPtr<RenderTarget>& target, u32 syncMask)
{
	THROW_IF_NOT_CORE_THREAD

	if(target == nullptr || !target->GetProperties().IsWindow)
		return;

	// Retrieve the swap chain before command buffer submit, as the submit might internally rebuild the swap chain.
	VulkanSwapChain* swapChain = nullptr;

#if B3D_PLATFORM == B3D_PLATFORM_ID_WIN32
	Win32RenderWindow* window = static_cast<Win32RenderWindow*>(target.get());
#elif B3D_PLATFORM == B3D_PLATFORM_ID_LINUX
	LinuxRenderWindow* window = static_cast<LinuxRenderWindow*>(target.get());
#elif B3D_PLATFORM == B3D_PLATFORM_ID_MACOS
	MacOSRenderWindow* window = static_cast<MacOSRenderWindow*>(target.get());
#endif

	window->SwapBuffers();
	swapChain = window->GetSwapChain();

	VulkanGpuQueue* const presentQueue = static_cast<VulkanGpuQueue*>(GetVulkanGpuBackend().GetPresentDevice()->GetQueue(GQT_GRAPHICS, 0).get());
	GetVulkanSubmitThread().QueuePresent(*presentQueue, *swapChain, syncMask);

	// Ensure the acquire operation we queued the previous frame has finished. This also means the old image was presented.
	swapChain->WaitUntilFirstImageAcquired();

	GetVulkanSubmitThread().QueueImageAcquire(*swapChain);

	B3D_INCREMENT_RENDER_STATISTIC(NumPresents);
}

void VulkanRenderAPI::ConvertProjectionMatrix(const Matrix4& matrix, Matrix4& dest)
{
	dest = matrix;

	// Flip Y axis
	dest[1][1] = -dest[1][1];

	// Convert depth range from [-1,1] to [0,1]
	dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
	dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
	dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
	dest[2][3] = (dest[2][3] + dest[3][3]) / 2;
}

GpuDataParameterBlockInformation VulkanRenderAPI::GenerateParamBlockDesc(const String& name, Vector<GpuDataParameterInformation>& params)
{
	GpuDataParameterBlockInformation block;
	block.BlockSize = 0;
	block.IsShareable = true;
	block.Name = name;
	block.Slot = 0;
	block.Set = 0;

	for(auto& param : params)
	{
		u32 size;
		if(param.Type == GPDT_STRUCT)
		{
			// Structs are always aligned and rounded up to vec4
			size = Math::DivideAndRoundUp(param.ElementSize, 16U) * 4;
			block.BlockSize = Math::DivideAndRoundUp(block.BlockSize, 4U) * 4;
		}
		else
			size = VulkanUtility::CalcInterfaceBlockElementSizeAndOffset(param.Type, param.ArraySize, block.BlockSize);

		param.ElementSize = size;
		param.ArrayElementStride = size;
		param.CpuOffset = block.BlockSize;
		param.GpuOffset = 0;
		block.BlockSize += size * param.ArraySize;
		param.ParamBlockSlot = 0;
		param.ParamBlockSet = 0;
	}

	// Constant buffer size must always be a multiple of 16
	if(block.BlockSize % 4 != 0)
		block.BlockSize += (4 - (block.BlockSize % 4));

	return block;
}

namespace bs { namespace ct {
VulkanRenderAPI& GetVulkanRenderAPI()
{
	return static_cast<VulkanRenderAPI&>(RenderAPI::Instance());
}
}} // namespace bs::ct
