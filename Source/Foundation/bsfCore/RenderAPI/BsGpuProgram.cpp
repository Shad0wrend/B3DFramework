//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsRenderAPICapabilities.h"
#include "RenderAPI/BsRenderAPI.h"
#include "RenderAPI/BsGpuParams.h"
#include "RenderAPI/BsGpuParamDesc.h"
#include "Managers/BsGpuProgramManager.h"
#include "Private/RTTI/BsGpuProgramRTTI.h"

namespace bs
{
	GpuProgramBytecode::~GpuProgramBytecode()
	{
		if(instructions.data)
			bs_free(instructions.data);
	}

	RTTITypeBase* GpuProgramBytecode::GetRttiStatic()
	{
		return GpuProgramBytecodeRTTI::Instance();
	}

	RTTITypeBase* GpuProgramBytecode::GetRtti() const
	{
		return GpuProgramBytecode::GetRttiStatic();
	}

	GpuProgram::GpuProgram(const GPU_PROGRAM_DESC& desc)
		: mNeedsAdjacencyInfo(desc.requiresAdjacency), mLanguage(desc.language), mType(desc.type)
		, mEntryPoint(desc.entryPoint), mSource(desc.source)
	{

	}

	bool GpuProgram::IsCompiled() const
	{
		return getCore()->isCompiled();
	}

	String GpuProgram::GetCompileErrorMessage() const
	{
		return getCore()->getCompileErrorMessage();
	}

	SPtr<GpuParamDesc> GpuProgram::GetParamDesc() const
	{
		return getCore()->getParamDesc();
	}

	SPtr<ct::GpuProgram> GpuProgram::GetCore() const
	{
		return std::static_pointer_cast<ct::GpuProgram>(mCoreSpecific);
	}

	SPtr<ct::CoreObject> GpuProgram::CreateCore() const
	{
		GPU_PROGRAM_DESC desc;
		desc.source = mSource;
		desc.entryPoint = mEntryPoint;
		desc.language = mLanguage;
		desc.type = mType;
		desc.requiresAdjacency = mNeedsAdjacencyInfo;
		desc.bytecode = mBytecode;

		return ct::GpuProgramManager::Instance().createInternal(desc);
	}

	SPtr<GpuProgram> GpuProgram::Create(const GPU_PROGRAM_DESC& desc)
	{
		return GpuProgramManager::Instance().create(desc);
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/
	RTTITypeBase* GpuProgram::GetRttiStatic()
	{
		return GpuProgramRTTI::Instance();
	}

	RTTITypeBase* GpuProgram::GetRtti() const
	{
		return GpuProgram::GetRttiStatic();
	}

	namespace ct
	{
	GpuProgram::GpuProgram(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask)
		: mNeedsAdjacencyInfo(desc.requiresAdjacency), mType(desc.type), mEntryPoint(desc.entryPoint), mSource(desc.source)
		, mBytecode(desc.bytecode)
	{
		mParametersDesc = bs_shared_ptr_new<GpuParamDesc>();
	}

	GpuProgram::~GpuProgram()
	{ }

	bool GpuProgram::IsSupported() const
	{
		return true;
	}

	SPtr<GpuProgram> GpuProgram::Create(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask)
	{
		return GpuProgramManager::Instance().create(desc, deviceMask);
	}

	SPtr<GpuProgramBytecode> GpuProgram::CompileBytecode(const GPU_PROGRAM_DESC& desc)
	{
		return GpuProgramManager::Instance().compileBytecode(desc);
	}
	}
}
