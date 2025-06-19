//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#if B3D_PLATFORM == B3D_PLATFORM_ID_WIN32

#	include "Win32/BsWin32GLSupport.h"
#	include "Win32/BsWin32VideoModeInfo.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup GL
		 *  @{
		 */

		/**	Helper method that returns a platform specific GL support object. */
		GLSupport* GetGlSupport()
		{
			return B3DNew<Win32GLSupport>();
		}

		/** @} */
	} // namespace render
} // namespace b3d

#elif B3D_PLATFORM == B3D_PLATFORM_ID_LINUX

#	include "Linux/BsLinuxGLSupport.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup GL
		 *  @{
		 */

		/**	Helper method that returns a platform specific GL support object. */
		GLSupport* getGLSupport()
		{
			return B3DNew<LinuxGLSupport>();
		}

		/** @} */
	} // namespace render
} // namespace b3d

#elif B3D_PLATFORM == B3D_PLATFORM_ID_MACOS

#	include "MacOS/BsMacOSGLSupport.h"

namespace b3d::render
{
	/** @addtogroup GL
	 *  @{
	 */

	/**	Helper method that returns a platform specific GL support object. */
	GLSupport* getGLSupport()
	{
		return B3DNew<MacOSGLSupport>();
	}

	/** @} */
} // namespace b3d::render
#endif
