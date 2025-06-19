//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include <windows.h>
#include <BsGLPrerequisites.h>

namespace b3d
{
	namespace render
	{
		/** @addtogroup GL
		 *  @{
		 */

		class Win32GLSupport;
		class Win32Context;
		class Win32RenderWindow;

		/**	Retrieves last Windows API error and returns a description of it. */
		String TranslateWglError();

		/** @} */
	} // namespace render
} // namespace b3d
