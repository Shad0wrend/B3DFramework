//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Testing/B3DConsoleTestOutput.h"
#include "Testing/B3DJSONTestOutput.h"
#include "B3DUtilityTestSuite.h"
#include "Utility/B3DCommandLine.h"

using namespace b3d;

int main(int argc, char* argv[])
{
	CrashHandler::StartUp();
	CommandLine::Initialize(argc, argv);

	String outputFormat = CommandLine::GetParameterValue("test-output-format", "console");
	String outputPath = CommandLine::GetParameterValue("test-output-path", "");

	SPtr<TestSuite> tests = UtilityTestSuite::Create<UtilityTestSuite>();

	i32 exitCode;
	if(outputFormat == "json")
	{
		Path jsonPath = outputPath.empty() ? Path("test_results.json") : Path(outputPath);
		JSONTestOutput testOutput(jsonPath);
		tests->Run(testOutput);
		exitCode = testOutput.GetExitCode();
	}
	else
	{
		ConsoleTestOutput testOutput;
		tests->Run(testOutput);
		exitCode = testOutput.GetExitCode();
	}

	CrashHandler::ShutDown();
	return exitCode;
}
