//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsGLPrerequisites.h"
#include "RenderAPI/BsVertexBuffer.h"
#include "BsGLHardwareBuffer.h"
#include "BsGLVertexArrayObjectManager.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup GL
		 *  @{
		 */

		/**	OpenGL implementation of a vertex buffer. */
		class GLVertexBuffer : public VertexBuffer
		{
		public:
			GLVertexBuffer(const VertexBufferCreateInformation& desc, GpuDeviceFlags deviceMask);
			~GLVertexBuffer();

			/**	Returns internal OpenGL buffer ID. */
			GLuint GetGlBufferId() const { return static_cast<GLHardwareBuffer*>(mBuffer)->GetGlBufferId(); }

			/**	Registers a new VertexArrayObject that uses this vertex buffer. */
			void RegisterVao(const GLVertexArrayObject& vao);

			/**	Unregisters a VAO from this vertex buffer. Does not destroy it. */
			void UnregisterVao(const GLVertexArrayObject& vao);

		protected:
			void Initialize() override;

		private:
			Vector<GLVertexArrayObject> mVAObjects;
		};

		/** @} */
	} // namespace render
} // namespace b3d
