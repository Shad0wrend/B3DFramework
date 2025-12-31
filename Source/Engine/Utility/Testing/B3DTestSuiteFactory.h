//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "Testing/B3DTestSuite.h"

namespace b3d
{
	/** @addtogroup Testing
	 *  @{
	 */

	/**
	 * Specifies which layer of tests to run. Tests are organized by layer to allow running
	 * subsets of tests without full engine initialization.
	 */
	enum class TestLayer
	{
		Utility, /**< Only Utility layer needed (no Application startup). */
		Core, /**< Requires Application startup (rendering, input, etc.). */
		Editor /**< Requires EditorApplication startup (editor systems). */
	};

	/**
	 * Factory interface for creating test suites. Allows test suites to be registered at runtime via dynamic library loading,
	 * which allows the framework test runner to run tests defined in e.g. editor layer.
	 */
	class B3D_EXPORT ITestSuiteFactory
	{
	public:
		virtual ~ITestSuiteFactory() = default;

		/** Returns the name of this test suite collection. */
		virtual const char* GetName() const = 0;

		/** Creates and returns all test suites provided by this factory. */
		virtual Vector<SPtr<TestSuite>> CreateTestSuites() = 0;

		/** Returns which layer this test suite belongs to. */
		virtual TestLayer GetLayer() const = 0;
	};

	/** @} */
} // namespace b3d
