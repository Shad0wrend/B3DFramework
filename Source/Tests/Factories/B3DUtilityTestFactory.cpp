//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DUtilityTestFactory.h"
#include "B3DUtilityTestSuite.h"
#include "B3DFileSystemTestSuite.h"
#include "B3DECSTestSuite.h"

namespace b3d
{
	Vector<SPtr<TestSuite>> UtilityTestFactory::CreateTestSuites()
	{
		return {
			TestSuite::Create<UtilityTestSuite>(),
			TestSuite::Create<FileSystemTestSuite>(),
			TestSuite::Create<ECSTestSuite>()
		};
	}
} // namespace b3d
