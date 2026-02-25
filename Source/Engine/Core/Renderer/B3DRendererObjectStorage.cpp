//************************************ B3D Framework - Copyright 2026 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/B3DRendererObjectStorage.h"
#include "Allocators/B3DFrameAllocator.h"

namespace b3d
{
	RendererId RendererObjectStorage::AllocateRendererId()
	{
		RendererId objectId = mObjectIdAllocator.Allocate();

		RendererIdCommand command;
		command.Type = RendererIdCommandType::Allocate;
		command.ObjectId = objectId;
		mCommandBuffers[mActiveBuffer].Add(command);

		return objectId;
	}

	void RendererObjectStorage::DeallocateRendererId(RendererId objectId)
	{
		RendererIdCommand command;
		command.Type = RendererIdCommandType::Deallocate;
		command.ObjectId = objectId;
		mCommandBuffers[mActiveBuffer].Add(command);

		mObjectIdAllocator.Deallocate(objectId);
	}

	u32 RendererObjectStorage::FlushCommands(FrameAllocator& allocator, RendererIdCommand*& outCommands)
	{
		const u32 flushedBufferIndex = mActiveBuffer;
		mActiveBuffer = 1 - mActiveBuffer;
		mCommandBuffers[mActiveBuffer].clear();

		const auto& commands = mCommandBuffers[flushedBufferIndex];
		u32 commandCount = (u32)commands.size();
		if(commandCount == 0)
		{
			outCommands = nullptr;
			return 0;
		}

		outCommands = reinterpret_cast<RendererIdCommand*>(allocator.AllocateAligned(sizeof(RendererIdCommand) * commandCount, alignof(RendererIdCommand)));
		memcpy(outCommands, commands.data(), sizeof(RendererIdCommand) * commandCount);

		return commandCount;
	}
} // namespace b3d
