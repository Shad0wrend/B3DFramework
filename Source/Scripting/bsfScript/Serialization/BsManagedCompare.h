//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Serialization/BsBinaryCompare.h"

namespace bs
{
	/** @addtogroup bsfScript
	 *  @{
	 */

	/** Handles that performs equality comparison for managed objects. */
	class B3D_SCRIPT_INTEROP_EXPORT ManagedCompare : public ICompare
	{
	public:
		virtual ~ManagedCompare() = default;

		bool Run(IReflectable& a, IReflectable& b) override;
	};

	/** @} */
} // namespace bs
