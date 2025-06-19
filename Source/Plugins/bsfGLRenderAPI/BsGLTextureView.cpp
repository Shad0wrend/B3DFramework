//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGLTextureView.h"
#include "BsGLTexture.h"

using namespace b3d;
using namespace b3d::ct;

GLTextureView::GLTextureView(const GLTexture* texture, const TextureViewInformation& desc)
	: TextureView(desc)
{
	const TextureProperties& props = texture->GetProperties();

	GLenum target;
	switch(props.Type)
	{
	case TEX_TYPE_1D:
		{
			if(desc.Surface.FaceCount <= 1)
				target = GL_TEXTURE_1D;
			else
				target = GL_TEXTURE_1D_ARRAY;
		}
		break;
	default:
	case TEX_TYPE_2D:
		{
			if(props.SampleCount <= 1)
			{
				if(desc.Surface.FaceCount <= 1)
					target = GL_TEXTURE_2D;
				else
					target = GL_TEXTURE_2D_ARRAY;
			}
			else
			{
				if(desc.Surface.FaceCount <= 1)
					target = GL_TEXTURE_2D_MULTISAMPLE;
				else
					target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			}
		}
		break;
	case TEX_TYPE_3D:
		target = GL_TEXTURE_3D;
		break;
	case TEX_TYPE_CUBE_MAP:
		{
			if(desc.Surface.FaceCount % 6 == 0)
			{
				if(desc.Surface.FaceCount == 6)
					target = GL_TEXTURE_CUBE_MAP;
				else
					target = GL_TEXTURE_CUBE_MAP_ARRAY;
			}
			else
			{
				if(desc.Surface.FaceCount == 1)
					target = GL_TEXTURE_2D;
				else
					target = GL_TEXTURE_2D_ARRAY;
			}
		}
		break;
	}

#if BS_OPENGL_4_3 || BS_OPENGLES_3_1
	GLuint originalTexture = texture->GetGlid();

	glGenTextures(1, &mViewID);
	B3D_CHECK_GL_ERROR();

	glTextureView(
		mViewID,
		target,
		originalTexture,
		texture->GetGlFormat(),
		desc.Surface.MipLevel,
		desc.Surface.MipLevelCount,
		desc.Surface.Face,
		desc.Surface.FaceCount);
	B3D_CHECK_GL_ERROR();
#endif

	mTarget = GLTexture::GetGlTextureTarget(props.Type, props.SampleCount, desc.Surface.FaceCount);
}

GLTextureView::~GLTextureView()
{
	if(mViewID != 0)
	{
		glDeleteTextures(1, &mViewID);
		B3D_CHECK_GL_ERROR();
	}
}
