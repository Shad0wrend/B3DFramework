//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DCoreTestFactory.h"
#include "B3DCoreTestSuite.h"
#include "B3DPrefabTestSuite.h"

namespace b3d
{
	Vector<SPtr<TestSuite>> CoreTestFactory::CreateTestSuites()
	{
		return {
			TestSuite::Create<CoreTestSuite>(),
			TestSuite::Create<PrefabTestSuite>()
		};
	}
} // namespace b3d
