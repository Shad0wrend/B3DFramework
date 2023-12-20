//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUISpace.h"

using namespace bs;

GUIConstrainedSize GUIFixedSpace::CalculateConstrainedSize() const
{
	GUIConstrainedSize range;
	range.Optimal = CalculateUnconstrainedOptimalSize();
	range.Min = range.Optimal;
	range.Max = range.Optimal;

	return range;
}

GUIFixedSpace* GUIFixedSpace::Create(u32 size)
{
	return B3DNew<GUIFixedSpace>(size);
}

GUIConstrainedSize GUIFlexibleSpace::CalculateConstrainedSize() const
{
	GUIConstrainedSize range;
	range.Optimal = CalculateUnconstrainedOptimalSize();
	range.Min = range.Optimal;
	range.Max = range.Optimal;

	return range;
}

GUIFlexibleSpace* GUIFlexibleSpace::Create()
{
	return B3DNew<GUIFlexibleSpace>();
}
