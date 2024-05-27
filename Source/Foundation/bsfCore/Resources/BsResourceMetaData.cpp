//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Resources/BsResourceMetaData.h"
#include "Private/RTTI/BsResourceMetaDataRTTI.h"

using namespace bs;

RTTIType* ResourceMetaData::GetRttiStatic()
{
	return ResourceMetaDataRTTI::Instance();
}

RTTIType* ResourceMetaData::GetRtti() const
{
	return ResourceMetaData::GetRttiStatic();
}
