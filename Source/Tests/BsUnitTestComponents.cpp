//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsUnitTestComponents.h"
#include "RTTI/BsUnitTestComponentsRTTI.h"

using namespace bs;

UnitTestComponentA::UnitTestComponentA(const HSceneObject& parent)
	: Component(parent)
{}

RTTITypeBase* UnitTestComponentA::GetRttiStatic()
{
	return UnitTestComponentARTTI::Instance();
}

RTTITypeBase* UnitTestComponentA::GetRtti() const
{
	return GetRttiStatic();
}

UnitTestComponentB::UnitTestComponentB(const HSceneObject& parent)
	: Component(parent)
{}

RTTITypeBase* UnitTestComponentB::GetRttiStatic()
{
	return UnitTestComponentBRTTI::Instance();
}

RTTITypeBase* UnitTestComponentB::GetRtti() const
{
	return GetRttiStatic();
}
