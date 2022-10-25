//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

namespace bs
{
	/** @addtogroup RenderAPI-Internal
	 *  @{
	 */

	/** Factory class that you may specialize in order to start up a specific render system. */
	class RenderAPIFactory
	{
	public:
		virtual ~RenderAPIFactory() = default;

		/**	Creates and starts up the render system managed by this factory.  */
		virtual void Create() = 0;

		/**	Returns the name of the render system this factory creates. */
		virtual const char* Name() const = 0;
	};

	/** @} */
} // namespace bs
