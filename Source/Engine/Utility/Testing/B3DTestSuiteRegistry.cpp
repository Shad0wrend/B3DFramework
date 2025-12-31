//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Testing/B3DTestSuiteRegistry.h"

using namespace b3d;

void TestSuiteRegistry::RegisterFactory(const SPtr<ITestSuiteFactory>& factory)
{
	mFactories.push_back(factory);
}

Vector<SPtr<ITestSuiteFactory>> TestSuiteRegistry::GetFactoriesByLayer(TestLayer layer) const
{
	Vector<SPtr<ITestSuiteFactory>> result;
	for (const auto& factory : mFactories)
	{
		if (factory->GetLayer() == layer)
			result.push_back(factory);
	}
	return result;
}

SPtr<ITestSuiteFactory> TestSuiteRegistry::GetFactoryByName(const String& name) const
{
	for (const auto& factory : mFactories)
	{
		if (factory->GetName() == name)
			return factory;
	}
	return nullptr;
}
