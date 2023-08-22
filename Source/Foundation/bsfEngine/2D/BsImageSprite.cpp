//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "2D/BsImageSprite.h"
#include "2D/BsSpriteManager.h"
#include "Image/BsTexture.h"
#include "Image/BsSpriteTexture.h"

using namespace bs;

ImageSprite::~ImageSprite()
{
	ClearMesh();
}

void ImageSprite::Update(const ImageSpriteInformation& information, u64 groupId)
{
	if(!SpriteTexture::CheckIsLoaded(information.Image))
	{
		ClearMesh();
		return;
	}

	// Actually generate a mesh
	if(mCachedRenderElements.size() < 1)
		mCachedRenderElements.resize(1);

	bool useScale9Grid = information.BorderLeft > 0 || information.BorderRight > 0 ||
		information.BorderTop > 0 || information.BorderBottom > 0;

	u32 numQuads = 1;
	if(useScale9Grid)
		numQuads = 9;

	SpriteRenderElementData& renderElem = mCachedRenderElements[0];
	{
		u32 newNumQuads = numQuads;
		if(newNumQuads != renderElem.QuadCount)
		{
			u32 oldVertexCount = renderElem.QuadCount * 4;
			u32 oldIndexCount = renderElem.QuadCount * 6;

			if(renderElem.VertexPositions != nullptr) B3DDeleteMultiple(renderElem.VertexPositions, oldVertexCount);
			if(renderElem.VertexUVs != nullptr) B3DDeleteMultiple(renderElem.VertexUVs, oldVertexCount);
			if(renderElem.Indices != nullptr) B3DDeleteMultiple(renderElem.Indices, oldIndexCount);

			renderElem.VertexPositions = B3DNewMultiple<Vector2>(newNumQuads * 4);
			renderElem.VertexUVs = B3DNewMultiple<Vector2>(newNumQuads * 4);
			renderElem.Indices = B3DNewMultiple<u32>(newNumQuads * 6);
			renderElem.QuadCount = newNumQuads;
		}

		const HTexture& tex = information.Image->GetTexture();

		SpriteMaterialInfo& matInfo = renderElem.MaterialInformation;
		matInfo.GroupId = groupId;
		matInfo.Texture = tex;
		matInfo.Tint = information.Color;
		matInfo.AnimationStartTime = information.AnimationStartTime;

		bool animated = information.Image->GetAnimation().Count > 1;
		if(animated)
			matInfo.SpriteTexture = information.Image;

		renderElem.Material = SpriteManager::Instance().GetImageMaterial(
			information.Transparent ? SpriteMaterialTransparency::Alpha : SpriteMaterialTransparency::Opaque, animated);
	}

	for(u32 i = 0; i < numQuads; i++)
	{
		renderElem.Indices[i * 6 + 0] = i * 4 + 0;
		renderElem.Indices[i * 6 + 1] = i * 4 + 1;
		renderElem.Indices[i * 6 + 2] = i * 4 + 2;
		renderElem.Indices[i * 6 + 3] = i * 4 + 1;
		renderElem.Indices[i * 6 + 4] = i * 4 + 3;
		renderElem.Indices[i * 6 + 5] = i * 4 + 2;
	}

	Vector2I offset = GetAnchorOffset(information.Anchor, information.Width, information.Height);
	Vector2 uvOffset = information.UvOffset;
	Vector2 uvScale = information.UvScale;

	if(useScale9Grid)
	{
		u32 leftBorder = information.BorderLeft;
		u32 rightBorder = information.BorderRight;
		u32 topBorder = information.BorderTop;
		u32 bottomBorder = information.BorderBottom;

		float centerWidth = (float)std::max((i32)0, (i32)information.Width - (i32)leftBorder - (i32)rightBorder);
		float centerHeight = (float)std::max((i32)0, (i32)information.Height - (i32)topBorder - (i32)bottomBorder);

		float topCenterStart = (float)(offset.X + leftBorder);
		float topRightStart = (float)(topCenterStart + centerWidth);

		float middleStart = (float)(offset.Y + topBorder);
		float bottomStart = (float)(middleStart + centerHeight);

		// Top left
		renderElem.VertexPositions[0] = Vector2((float)offset.X, (float)offset.Y);
		renderElem.VertexPositions[1] = Vector2((float)offset.X + leftBorder, (float)offset.Y);
		renderElem.VertexPositions[2] = Vector2((float)offset.X, middleStart);
		renderElem.VertexPositions[3] = Vector2((float)offset.X + leftBorder, middleStart);

		// Top center
		renderElem.VertexPositions[4] = Vector2(topCenterStart, (float)offset.Y);
		renderElem.VertexPositions[5] = Vector2(topCenterStart + centerWidth, (float)offset.Y);
		renderElem.VertexPositions[6] = Vector2(topCenterStart, middleStart);
		renderElem.VertexPositions[7] = Vector2(topCenterStart + centerWidth, middleStart);

		// Top right
		renderElem.VertexPositions[8] = Vector2(topRightStart, (float)offset.Y);
		renderElem.VertexPositions[9] = Vector2(topRightStart + rightBorder, (float)offset.Y);
		renderElem.VertexPositions[10] = Vector2(topRightStart, middleStart);
		renderElem.VertexPositions[11] = Vector2(topRightStart + rightBorder, middleStart);

		// Middle left
		renderElem.VertexPositions[12] = Vector2((float)offset.X, middleStart);
		renderElem.VertexPositions[13] = Vector2((float)offset.X + leftBorder, middleStart);
		renderElem.VertexPositions[14] = Vector2((float)offset.X, bottomStart);
		renderElem.VertexPositions[15] = Vector2((float)offset.X + leftBorder, bottomStart);

		// Middle center
		renderElem.VertexPositions[16] = Vector2(topCenterStart, middleStart);
		renderElem.VertexPositions[17] = Vector2(topCenterStart + centerWidth, middleStart);
		renderElem.VertexPositions[18] = Vector2(topCenterStart, bottomStart);
		renderElem.VertexPositions[19] = Vector2(topCenterStart + centerWidth, bottomStart);

		// Middle right
		renderElem.VertexPositions[20] = Vector2(topRightStart, middleStart);
		renderElem.VertexPositions[21] = Vector2(topRightStart + rightBorder, middleStart);
		renderElem.VertexPositions[22] = Vector2(topRightStart, bottomStart);
		renderElem.VertexPositions[23] = Vector2(topRightStart + rightBorder, bottomStart);

		// Bottom left
		renderElem.VertexPositions[24] = Vector2((float)offset.X, bottomStart);
		renderElem.VertexPositions[25] = Vector2((float)offset.X + leftBorder, bottomStart);
		renderElem.VertexPositions[26] = Vector2((float)offset.X, bottomStart + bottomBorder);
		renderElem.VertexPositions[27] = Vector2((float)offset.X + leftBorder, bottomStart + bottomBorder);

		// Bottom center
		renderElem.VertexPositions[28] = Vector2(topCenterStart, bottomStart);
		renderElem.VertexPositions[29] = Vector2(topCenterStart + centerWidth, bottomStart);
		renderElem.VertexPositions[30] = Vector2(topCenterStart, bottomStart + bottomBorder);
		renderElem.VertexPositions[31] = Vector2(topCenterStart + centerWidth, bottomStart + bottomBorder);

		// Bottom right
		renderElem.VertexPositions[32] = Vector2(topRightStart, bottomStart);
		renderElem.VertexPositions[33] = Vector2(topRightStart + rightBorder, bottomStart);
		renderElem.VertexPositions[34] = Vector2(topRightStart, bottomStart + bottomBorder);
		renderElem.VertexPositions[35] = Vector2(topRightStart + rightBorder, bottomStart + bottomBorder);

		float invWidth = 1.0f / (float)information.Image->GetTexture()->GetProperties().Width;
		float invHeight = 1.0f / (float)information.Image->GetTexture()->GetProperties().Height;

		float uvLeftBorder = information.BorderLeft * invWidth;
		float uvRightBorder = information.BorderRight * invWidth;
		float uvTopBorder = information.BorderTop * invHeight;
		float uvBottomBorder = information.BorderBottom * invHeight;

		float uvCenterWidth = std::max(0.0f, uvScale.X - uvLeftBorder - uvRightBorder);
		float uvCenterHeight = std::max(0.0f, uvScale.Y - uvTopBorder - uvBottomBorder);

		float uvTopCenterStart = uvOffset.X + uvLeftBorder;
		float uvTopRightStart = uvTopCenterStart + uvCenterWidth;

		float uvMiddleStart = uvOffset.Y + uvTopBorder;
		float uvBottomStart = uvMiddleStart + uvCenterHeight;

		// UV - Top left
		renderElem.VertexUVs[0] = information.Image->TransformUv(Vector2(uvOffset.X, uvOffset.Y));
		renderElem.VertexUVs[1] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvOffset.Y));
		renderElem.VertexUVs[2] = information.Image->TransformUv(Vector2(uvOffset.X, uvOffset.Y + uvTopBorder));
		renderElem.VertexUVs[3] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvOffset.Y + uvTopBorder));

		// UV - Top center
		renderElem.VertexUVs[4] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvOffset.Y));
		renderElem.VertexUVs[5] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvOffset.Y));
		renderElem.VertexUVs[6] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvOffset.Y + uvTopBorder));
		renderElem.VertexUVs[7] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvOffset.Y + uvTopBorder));

		// UV - Top right
		renderElem.VertexUVs[8] = information.Image->TransformUv(Vector2(uvTopRightStart, uvOffset.Y));
		renderElem.VertexUVs[9] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvOffset.Y));
		renderElem.VertexUVs[10] = information.Image->TransformUv(Vector2(uvTopRightStart, uvOffset.Y + uvTopBorder));
		renderElem.VertexUVs[11] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvOffset.Y + uvTopBorder));

		// UV - Middle left
		renderElem.VertexUVs[12] = information.Image->TransformUv(Vector2(uvOffset.X, uvMiddleStart));
		renderElem.VertexUVs[13] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvMiddleStart));
		renderElem.VertexUVs[14] = information.Image->TransformUv(Vector2(uvOffset.X, uvMiddleStart + uvCenterHeight));
		renderElem.VertexUVs[15] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvMiddleStart + uvCenterHeight));

		// UV - Middle center
		renderElem.VertexUVs[16] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvMiddleStart));
		renderElem.VertexUVs[17] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvMiddleStart));
		renderElem.VertexUVs[18] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvMiddleStart + uvCenterHeight));
		renderElem.VertexUVs[19] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvMiddleStart + uvCenterHeight));

		// UV - Middle right
		renderElem.VertexUVs[20] = information.Image->TransformUv(Vector2(uvTopRightStart, uvMiddleStart));
		renderElem.VertexUVs[21] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvMiddleStart));
		renderElem.VertexUVs[22] = information.Image->TransformUv(Vector2(uvTopRightStart, uvMiddleStart + uvCenterHeight));
		renderElem.VertexUVs[23] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvMiddleStart + uvCenterHeight));

		// UV - Bottom left
		renderElem.VertexUVs[24] = information.Image->TransformUv(Vector2(uvOffset.X, uvBottomStart));
		renderElem.VertexUVs[25] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvBottomStart));
		renderElem.VertexUVs[26] = information.Image->TransformUv(Vector2(uvOffset.X, uvBottomStart + uvBottomBorder));
		renderElem.VertexUVs[27] = information.Image->TransformUv(Vector2(uvOffset.X + uvLeftBorder, uvBottomStart + uvBottomBorder));

		// UV - Bottom center
		renderElem.VertexUVs[28] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvBottomStart));
		renderElem.VertexUVs[29] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvBottomStart));
		renderElem.VertexUVs[30] = information.Image->TransformUv(Vector2(uvTopCenterStart, uvBottomStart + uvBottomBorder));
		renderElem.VertexUVs[31] = information.Image->TransformUv(Vector2(uvTopCenterStart + uvCenterWidth, uvBottomStart + uvBottomBorder));

		// UV - Bottom right
		renderElem.VertexUVs[32] = information.Image->TransformUv(Vector2(uvTopRightStart, uvBottomStart));
		renderElem.VertexUVs[33] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvBottomStart));
		renderElem.VertexUVs[34] = information.Image->TransformUv(Vector2(uvTopRightStart, uvBottomStart + uvBottomBorder));
		renderElem.VertexUVs[35] = information.Image->TransformUv(Vector2(uvTopRightStart + uvRightBorder, uvBottomStart + uvBottomBorder));
	}
	else
	{
		renderElem.VertexPositions[0] = Vector2((float)offset.X, (float)offset.Y);
		renderElem.VertexPositions[1] = Vector2((float)offset.X + information.Width, (float)offset.Y);
		renderElem.VertexPositions[2] = Vector2((float)offset.X, (float)offset.Y + information.Height);
		renderElem.VertexPositions[3] = Vector2((float)offset.X + information.Width, (float)offset.Y + information.Height);

		renderElem.VertexUVs[0] = information.Image->TransformUv(Vector2(uvOffset.X, uvOffset.Y));
		renderElem.VertexUVs[1] = information.Image->TransformUv(Vector2(uvOffset.X + uvScale.X, uvOffset.Y));
		renderElem.VertexUVs[2] = information.Image->TransformUv(Vector2(uvOffset.X, uvOffset.Y + uvScale.Y));
		renderElem.VertexUVs[3] = information.Image->TransformUv(Vector2(uvOffset.X + uvScale.X, uvOffset.Y + uvScale.Y));
	}

	UpdateBounds();
}

void ImageSprite::ClearMesh()
{
	for(auto& renderElem : mCachedRenderElements)
	{
		u32 vertexCount = renderElem.QuadCount * 4;
		u32 indexCount = renderElem.QuadCount * 6;

		if(renderElem.VertexPositions != nullptr)
		{
			B3DDeleteMultiple(renderElem.VertexPositions, vertexCount);
			renderElem.VertexPositions = nullptr;
		}

		if(renderElem.VertexUVs != nullptr)
		{
			B3DDeleteMultiple(renderElem.VertexUVs, vertexCount);
			renderElem.VertexUVs = nullptr;
		}

		if(renderElem.Indices != nullptr)
		{
			B3DDeleteMultiple(renderElem.Indices, indexCount);
			renderElem.Indices = nullptr;
		}
	}

	mCachedRenderElements.clear();
	UpdateBounds();
}

Vector2 ImageSprite::GetTextureUvScale(Vector2I sourceSize, Vector2I destSize, TextureScaleMode scaleMode)
{
	Vector2 uvScale = Vector2(1.0f, 1.0f);

	switch(scaleMode)
	{
	case TextureScaleMode::ScaleToFit:
		uvScale.X = sourceSize.X / (float)destSize.X;
		uvScale.Y = sourceSize.Y / (float)destSize.Y;

		if(uvScale.X > uvScale.Y)
		{
			uvScale.X = 1.0f;
			uvScale.Y = (destSize.Y * (sourceSize.Y / (float)sourceSize.X)) / destSize.X;
		}
		else
		{
			uvScale.X = (destSize.X * (sourceSize.X / (float)sourceSize.Y)) / destSize.Y;
			uvScale.Y = 1.0f;
		}

		break;
	case TextureScaleMode::CropToFit:
		uvScale.X = sourceSize.X / (float)destSize.X;
		uvScale.Y = sourceSize.Y / (float)destSize.Y;

		if(uvScale.X > uvScale.Y)
		{
			uvScale.X = (destSize.X * (sourceSize.X / (float)sourceSize.Y)) / destSize.Y;
			uvScale.Y = 1.0f;
		}
		else
		{
			uvScale.X = 1.0f;
			uvScale.Y = (destSize.Y * (sourceSize.Y / (float)sourceSize.X)) / destSize.X;
		}

		break;
	case TextureScaleMode::RepeatToFit:
		uvScale.X = destSize.X / (float)sourceSize.X;
		uvScale.Y = destSize.Y / (float)sourceSize.Y;
		break;
	case TextureScaleMode::StretchToFit:
		// Do nothing, (1.0f, 1.0f) is the default UV scale
		break;
	default:
		break;
	}

	return uvScale;
}
