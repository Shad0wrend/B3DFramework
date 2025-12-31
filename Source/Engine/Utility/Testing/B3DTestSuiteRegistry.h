//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "Utility/B3DModule.h"
#include "Testing/B3DTestSuiteFactory.h"

namespace b3d
{
	/** @addtogroup Testing
	 *  @{
	 */

	/** Keeps track of all registered test suite factories. */
	class B3D_EXPORT TestSuiteRegistry : public Module<TestSuiteRegistry>
	{
	public:
		/** Register a test suite factory. */
		void RegisterFactory(const SPtr<ITestSuiteFactory>& factory);

		/** Get all registered factories. */
		const Vector<SPtr<ITestSuiteFactory>>& GetFactories() const { return mFactories; }

		/** Get factories filtered by layer. */
		Vector<SPtr<ITestSuiteFactory>> GetFactoriesByLayer(TestLayer layer) const;

		/** Get factories filtered by name. */
		SPtr<ITestSuiteFactory> GetFactoryByName(const String& name) const;

	private:
		Vector<SPtr<ITestSuiteFactory>> mFactories;
	};

	/** @} */
} // namespace b3d
