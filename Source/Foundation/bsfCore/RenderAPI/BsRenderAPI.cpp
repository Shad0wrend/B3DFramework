//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsRenderAPI.h"

#include "CoreThread/BsCoreThread.h"
#include "RenderAPI/BsViewport.h"
#include "RenderAPI/BsRenderTarget.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Mesh/BsMesh.h"
#include "RenderAPI/BsGpuParams.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsRasterizerState.h"
#include "RenderAPI/BsGpuBuffer.h"
#include "RenderAPI/BsGpuPipelineState.h"

using namespace std::placeholders;

namespace bs
{
	void RenderAPI::SetGpuParams(const SPtr<GpuParams>& gpuParams)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetGpuParams, ct::RenderAPI::InstancePtr(), gpuParams->GetCore(),
			nullptr));
	}

	void RenderAPI::SetGraphicsPipeline(const SPtr<GraphicsPipelineState>& pipelineState)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetGraphicsPipeline, ct::RenderAPI::InstancePtr(),
			pipelineState->GetCore(), nullptr));
	}

	void RenderAPI::SetComputePipeline(const SPtr<ComputePipelineState>& pipelineState)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetComputePipeline, ct::RenderAPI::InstancePtr(),
							  pipelineState->GetCore(), nullptr));
	}

	void RenderAPI::SetVertexBuffers(UINT32 index, const Vector<SPtr<VertexBuffer>>& buffers)
	{
		Vector<SPtr<ct::VertexBuffer>> coreBuffers(buffers.size());
		for (UINT32 i = 0; i < (UINT32)buffers.size(); i++)
			coreBuffers[i] = buffers[i] != nullptr ? buffers[i]->GetCore() : nullptr;

		std::function<void(ct::RenderAPI*, UINT32, const Vector<SPtr<ct::VertexBuffer>>&)> resizeFunc =
			[](ct::RenderAPI* rs, UINT32 idx, const Vector<SPtr<ct::VertexBuffer>>& _buffers)
		{
			rs->SetVertexBuffers(idx, (SPtr<ct::VertexBuffer>*)_buffers.data(), (UINT32)_buffers.size());
		};

		gCoreThread().QueueCommand(std::bind(resizeFunc, ct::RenderAPI::InstancePtr(), index, coreBuffers));
	}

	void RenderAPI::SetIndexBuffer(const SPtr<IndexBuffer>& buffer)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetIndexBuffer, ct::RenderAPI::InstancePtr(), buffer->GetCore(),
			nullptr));
	}

	void RenderAPI::SetVertexDeclaration(const SPtr<VertexDeclaration>& vertexDeclaration)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetVertexDeclaration, ct::RenderAPI::InstancePtr(),
			vertexDeclaration->GetCore(), nullptr));
	}

	void RenderAPI::SetViewport(const Rect2& vp)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetViewport, ct::RenderAPI::InstancePtr(), vp, nullptr));
	}

	void RenderAPI::SetStencilRef(UINT32 value)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetStencilRef, ct::RenderAPI::InstancePtr(), value, nullptr));
	}

	void RenderAPI::SetDrawOperation(DrawOperationType op)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetDrawOperation, ct::RenderAPI::InstancePtr(), op,
			nullptr));
	}

	void RenderAPI::SetScissorRect(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetScissorRect, ct::RenderAPI::InstancePtr(), left, top, right, bottom,
			nullptr));
	}

	void RenderAPI::SetRenderTarget(const SPtr<RenderTarget>& target, UINT32 readOnlyFlags,
									RenderSurfaceMask loadMask)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SetRenderTarget,
			ct::RenderAPI::InstancePtr(), target->GetCore(), readOnlyFlags, loadMask, nullptr));
	}

	void RenderAPI::ClearRenderTarget(UINT32 buffers, const Color& color, float depth,
		UINT16 stencil, UINT8 targetMask)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::ClearRenderTarget, ct::RenderAPI::InstancePtr(), buffers, color,
			depth, stencil, targetMask, nullptr));
	}

	void RenderAPI::ClearViewport(UINT32 buffers, const Color& color, float depth, UINT16 stencil,
		UINT8 targetMask)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::ClearViewport, ct::RenderAPI::InstancePtr(), buffers, color, depth,
			stencil, targetMask, nullptr));
	}

	void RenderAPI::SwapBuffers(const SPtr<RenderTarget>& target)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::SwapBuffers, ct::RenderAPI::InstancePtr(), target->GetCore(), 1));
	}

	void RenderAPI::Draw(UINT32 vertexOffset, UINT32 vertexCount, UINT32 instanceCount)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::Draw, ct::RenderAPI::InstancePtr(), vertexOffset,
			vertexCount, instanceCount, nullptr));
	}

	void RenderAPI::DrawIndexed(UINT32 startIndex, UINT32 indexCount, UINT32 vertexOffset,
		UINT32 vertexCount, UINT32 instanceCount)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::DrawIndexed, ct::RenderAPI::InstancePtr(), startIndex, indexCount,
			vertexOffset, vertexCount, instanceCount, nullptr));
	}

	void RenderAPI::DispatchCompute(UINT32 numGroupsX, UINT32 numGroupsY, UINT32 numGroupsZ)
	{
		gCoreThread().QueueCommand(std::bind(&ct::RenderAPI::DispatchCompute, ct::RenderAPI::InstancePtr(), numGroupsX,
			numGroupsY, numGroupsZ, nullptr));
	}

	const VideoModeInfo& RenderAPI::GetVideoModeInfo()
	{
		return ct::RenderAPI::Instance().GetVideoModeInfo();
	}

	void RenderAPI::ConvertProjectionMatrix(const Matrix4& matrix, Matrix4& dest)
	{
		ct::RenderAPI::Instance().ConvertProjectionMatrix(matrix, dest);
	}

	namespace ct
	{
	RenderAPI::RenderAPI()
		: mCurrentCapabilities(nullptr), mNumDevices(0)
	{
	}

	RenderAPI::~RenderAPI()
	{
		// Base classes need to call virtual destroy_internal method instead of a destructor

		bs_deleteN(mCurrentCapabilities, mNumDevices);
		mCurrentCapabilities = nullptr;
	}

	SPtr<bs::RenderWindow> RenderAPI::Initialize(const RENDER_WINDOW_DESC& primaryWindowDesc)
	{
		gCoreThread().QueueCommand(std::bind((void(RenderAPI::*)())&PooledThread::Initialize, this),
			CTQF_InternalQueue | CTQF_BlockUntilComplete);

		RENDER_WINDOW_DESC windowDesc = primaryWindowDesc;
		SPtr<bs::RenderWindow> renderWindow = bs::RenderWindow::Create(windowDesc, nullptr);

		// Make sure render window initialization is submitted to the internal queue
		gCoreThread().SubmitAll();

		gCoreThread().QueueCommand(std::bind(&RenderAPI::InitializeWithWindow, this, renderWindow->GetCore()),
			CTQF_InternalQueue | CTQF_BlockUntilComplete);

		return renderWindow;
	}

	void RenderAPI::Initialize()
	{
		// Do nothing
	}

	void RenderAPI::InitializeWithWindow(const SPtr<RenderWindow>& primaryWindow)
	{
		THROW_IF_NOT_CORE_THREAD;
	}

	void RenderAPI::Destroy()
	{
		gCoreThread().QueueCommand(std::bind(&RenderAPI::DestroyCore, this));
		gCoreThread().SubmitAll(true);
	}

	void RenderAPI::DestroyCore()
	{
		mActiveRenderTarget = nullptr;
	}

	const RenderAPICapabilities& RenderAPI::GetCapabilities(UINT32 deviceIdx) const
	{
		if(deviceIdx >= mNumDevices)
		{
			BS_LOG(Warning, RenderBackend, "Invalid device index provided: {0}. Valid range is: [0, {1}).",
				deviceIdx, mNumDevices);
			return mCurrentCapabilities[0];
		}

		return mCurrentCapabilities[deviceIdx];
	}

	UINT32 RenderAPI::VertexCountToPrimCount(DrawOperationType type, UINT32 elementCount)
	{
		UINT32 primCount = 0;
		switch (type)
		{
		case DOT_POINT_LIST:
			primCount = elementCount;
			break;

		case DOT_LINE_LIST:
			primCount = elementCount / 2;
			break;

		case DOT_LINE_STRIP:
			primCount = elementCount - 1;
			break;

		case DOT_TRIANGLE_LIST:
			primCount = elementCount / 3;
			break;

		case DOT_TRIANGLE_STRIP:
			primCount = elementCount - 2;
			break;

		case DOT_TRIANGLE_FAN:
			primCount = elementCount - 2;
			break;
		}

		return primCount;
	}
	}
}
