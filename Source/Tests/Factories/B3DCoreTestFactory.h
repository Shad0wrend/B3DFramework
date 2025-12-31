//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Testing/B3DTestSuiteFactory.h"

namespace b3d
{
	/** Factory for core layer test suites. */
	class CoreTestFactory : public ITestSuiteFactory
	{
	public:
		const char* GetName() const override { return "Core"; }
		TestLayer GetLayer() const override { return TestLayer::Core; }
		Vector<SPtr<TestSuite>> CreateTestSuites() override;
	};
} // namespace b3d
