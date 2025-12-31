//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Testing/B3DConsoleTestOutput.h"
#include "Testing/B3DJSONTestOutput.h"
#include "Testing/B3DTestSuiteRegistry.h"
#include "Utility/B3DCommandLine.h"
#include "B3DApplication.h"

#include "Factories/B3DUtilityTestFactory.h"
#include "Factories/B3DCoreTestFactory.h"

using namespace b3d;

/**
 * Runs tests for a specific layer using the provided test output.
 * @param	layer		The layer to run tests for.
 * @param	output		The test output handler.
 * @return				True if all tests passed.
 */
static bool RunTestsForLayer(TestLayer layer, TestOutput& output)
{
	Vector<SPtr<ITestSuiteFactory>> factories = TestSuiteRegistry::Instance().GetFactoriesByLayer(layer);

	bool allPassed = true;
	for (const auto& factory : factories)
	{
		Vector<SPtr<TestSuite>> suites = factory->CreateTestSuites();
		for (const auto& suite : suites)
			suite->Run(output);
	}

	return allPassed;
}

/**
 * Runs tests for a specific layer with the configured output format.
 * @param	layer			The layer to run tests for.
 * @param	outputFormat	Output format ("console" or "json").
 * @param	outputPath		Path for json output.
 * @param	exitCode		Running exit code to accumulate failures.
 */
static void RunTestsForLayerWithOutput(TestLayer layer, const String& outputFormat, const String& outputPath, i32& exitCode)
{
	if (outputFormat == "json")
	{
		Path jsonPath = outputPath.empty() ? Path("test_results.json") : Path(outputPath);
		JSONTestOutput testOutput(jsonPath);
		RunTestsForLayer(layer, testOutput);
		if (testOutput.GetExitCode() != 0)
			exitCode = testOutput.GetExitCode();
	}
	else
	{
		ConsoleTestOutput testOutput;
		RunTestsForLayer(layer, testOutput);
		if (testOutput.GetExitCode() != 0)
			exitCode = testOutput.GetExitCode();
	}
}

int main(int argc, char* argv[])
{
	CrashHandler::StartUp();
	CommandLine::Initialize(argc, argv);

	String outputFormat = CommandLine::GetParameterValue("test-output-format", "console");
	String outputPath = CommandLine::GetParameterValue("test-output-path", "");
	String testLayer = CommandLine::GetParameterValue("test-layer", "all");

	bool runUtility = (testLayer == "all" || testLayer == "utility");
	bool runCore = (testLayer == "all" || testLayer == "core");
	bool runEditor = (testLayer == "editor");

	i32 exitCode = 0;

	// Start registry once and register all relevant factories
	TestSuiteRegistry::StartUp();

	if (runUtility)
		TestSuiteRegistry::Instance().RegisterFactory(B3DMakeShared<UtilityTestFactory>());

	if (runCore)
		TestSuiteRegistry::Instance().RegisterFactory(B3DMakeShared<CoreTestFactory>());

	// Phase 1: Run Utility-layer tests (no Application needed)
	if (runUtility)
		RunTestsForLayerWithOutput(TestLayer::Utility, outputFormat, outputPath, exitCode);

	// Phase 2: Run Core-layer tests (requires Application)
	if (runCore)
	{
		VideoMode videoMode(1280, 720);
		Application::StartUp(videoMode, "UnitTestRunner", false);

		RunTestsForLayerWithOutput(TestLayer::Core, outputFormat, outputPath, exitCode);

		Application::ShutDown();
	}

	// Phase 3: Editor-layer tests would be loaded via plugins
	// This requires loading EditorCore plugin which registers EditorTestFactory
	// For now, this is a placeholder for future implementation
	if (runEditor)
	{
		// TODO: Load editor test plugins
		// Application::Instance().LoadPlugin("EditorCore");
		// RunTestsForLayerWithOutput(TestLayer::Editor, outputFormat, outputPath, exitCode);
		// Application::ShutDown();
	}

	TestSuiteRegistry::ShutDown();
	CrashHandler::ShutDown();
	return exitCode;
}
