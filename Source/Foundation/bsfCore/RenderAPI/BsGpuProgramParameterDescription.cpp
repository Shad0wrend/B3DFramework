//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGpuProgramParameterDescription.h"
#include "Private/RTTI/BsGpuProgramRTTI.h"

using namespace bs;

constexpr u32 RTTIPlainType<GpuDataParameterBlockInformation>::kVersion;
constexpr u32 RTTIPlainType<GpuObjectParameterInformation>::kVersion;
constexpr u32 RTTIPlainType<GpuDataParameterInformation>::kVersion;

RTTITypeBase* GpuProgramParameterDescription::GetRttiStatic()
{
	return GpuProgramParameterDescriptionRTTI::Instance();
}

RTTITypeBase* GpuProgramParameterDescription::GetRtti() const
{
	return GpuProgramParameterDescription::GetRttiStatic();
}
