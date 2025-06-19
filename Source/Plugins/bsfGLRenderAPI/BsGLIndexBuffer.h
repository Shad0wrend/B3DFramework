//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsGLPrerequisites.h"
#include "RenderAPI/BsIndexBuffer.h"
#include "BsGLHardwareBuffer.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup GL
		 *  @{
		 */

		/**	OpenGL implementation of an index buffer. */
		class GLIndexBuffer : public IndexBuffer
		{
		public:
			GLIndexBuffer(const IndexBufferCreateInformation& desc, GpuDeviceFlags deviceMask);

			/**	Returns internal OpenGL index buffer handle. */
			GLuint GetGlBufferId() const { return static_cast<GLHardwareBuffer*>(mBuffer)->GetGlBufferId(); }

		protected:
			void Initialize() override;
		};

		/** @} */
	} // namespace render
} // namespace b3d
