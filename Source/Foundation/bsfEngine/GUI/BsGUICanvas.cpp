//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUICanvas.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUITexture.h"
#include "Utility/BsShapeMeshes2D.h"
#include "2D/BsSpriteManager.h"
#include "2D/BsSpriteMaterials.h"
#include "Mesh/BsMeshUtility.h"
#include "Error/BsException.h"

namespace bs
{
	const float GUICanvas::LINE_SMOOTH_BORDER_WIDTH = 3.0f;

	const String& GUICanvas::GetGuiTypeName()
	{
		static String name = "Canvas";
		return name;
	}

	GUICanvas::GUICanvas(const String& styleName, const GUIDimensions& dimensions)
		: GUIElement(styleName, dimensions)
	{ }

	GUICanvas::~GUICanvas()
	{
		Clear();
	}

	GUICanvas* GUICanvas::Create(const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUICanvas>()) GUICanvas(GetStyleName<GUICanvas>(styleName), GUIDimensions::Create(options));
	}

	GUICanvas* GUICanvas::Create(const String& styleName)
	{
		return new (bs_alloc<GUICanvas>()) GUICanvas(GetStyleName<GUICanvas>(styleName), GUIDimensions::Create());
	}

	void GUICanvas::DrawLine(const Vector2I& a, const Vector2I& b, const Color& color, UINT8 depth)
	{
		DrawPolyLine({ a, b }, color, depth);
	}

	void GUICanvas::DrawPolyLine(const Vector<Vector2I>& vertices, const Color& color, UINT8 depth)
	{
		if(vertices.size() < 2)
			return;

		mElements.push_back(CanvasElement());
		CanvasElement& element = mElements.back();

		element.Type = CanvasElementType::Line;
		element.Color = color;
		element.DataId = (UINT32)mTriangleElementData.size();
		element.VertexStart = (UINT32)mVertexData.size();
		element.NumVertices = (UINT32)vertices.size();
		element.Depth = depth;

		mDepthRange = std::max(mDepthRange, (UINT8)(depth + 1));

		mTriangleElementData.push_back(TriangleElementData());
		TriangleElementData& elemData = mTriangleElementData.back();
		elemData.MatInfo.GroupId = 0;
		elemData.MatInfo.Tint = color;

		for (auto& vertex : vertices)
		{
			Vector2 point = Vector2((float)vertex.X, (float)vertex.Y);
			point += Vector2(0.5f, 0.5f); // Offset to the middle of the pixel

			mVertexData.push_back(point);
		}

		mForceTriangleBuild = true;
		MarkContentAsDirtyInternal();
	}

	void GUICanvas::DrawTexture(const HSpriteTexture& texture, const Rect2I& area, TextureScaleMode scaleMode,
		const Color& color, UINT8 depth)
	{
		mElements.push_back(CanvasElement());
		CanvasElement& element = mElements.back();

		element.Type = CanvasElementType::Image;
		element.Color = color;
		element.DataId = (UINT32)mImageData.size();
		element.ScaleMode = scaleMode;
		element.ImageSprite = bs_new<ImageSprite>();
		element.Depth = depth;

		mDepthRange = std::max(mDepthRange, (UINT8)(depth + 1));

		mImageData.push_back({ texture, area });
		MarkContentAsDirtyInternal();
	}

	void GUICanvas::DrawTriangleStrip(const Vector<Vector2I>& vertices, const Color& color, UINT8 depth)
	{
		if (vertices.size() < 3)
		{
			BS_LOG(Warning, GUI, "Invalid number of vertices. Ignoring call.");
			return;
		}

		mElements.push_back(CanvasElement());
		CanvasElement& element = mElements.back();

		element.Type = CanvasElementType::Triangle;
		element.Color = color;
		element.DataId = (UINT32)mTriangleElementData.size();
		element.VertexStart = (UINT32)mVertexData.size();
		element.NumVertices = (UINT32)(vertices.size() - 2) * 3;
		element.Depth = depth;

		mDepthRange = std::max(mDepthRange, (UINT8)(depth + 1));

		// Convert strip to list
		for(UINT32 i = 2; i < (UINT32)vertices.size(); i++)
		{
			if (i % 2 == 0)
			{
				mVertexData.push_back(Vector2((float)vertices[i - 2].X + 0.5f, (float)vertices[i - 2].Y + 0.5f));
				mVertexData.push_back(Vector2((float)vertices[i - 1].X + 0.5f, (float)vertices[i - 1].Y + 0.5f));
				mVertexData.push_back(Vector2((float)vertices[i - 0].X + 0.5f, (float)vertices[i - 0].Y + 0.5f));
			}
			else
			{
				mVertexData.push_back(Vector2((float)vertices[i - 0].X + 0.5f, (float)vertices[i - 0].Y + 0.5f));
				mVertexData.push_back(Vector2((float)vertices[i - 1].X + 0.5f, (float)vertices[i - 1].Y + 0.5f));
				mVertexData.push_back(Vector2((float)vertices[i - 2].X + 0.5f, (float)vertices[i - 2].Y + 0.5f));
			}
		}

		mTriangleElementData.push_back(TriangleElementData());
		TriangleElementData& elemData = mTriangleElementData.back();
		elemData.MatInfo.GroupId = 0;
		elemData.MatInfo.Tint = color;

		mForceTriangleBuild = true;
		MarkContentAsDirtyInternal();
	}

	void GUICanvas::DrawTriangleList(const Vector<Vector2I>& vertices, const Color& color, UINT8 depth)
	{
		if (vertices.size() < 3 || vertices.size() % 3 != 0)
		{
			BS_LOG(Warning, GUI, "Invalid number of vertices. Ignoring call.");
			return;
		}

		mElements.push_back(CanvasElement());
		CanvasElement& element = mElements.back();

		element.Type = CanvasElementType::Triangle;
		element.Color = color;
		element.DataId = (UINT32)mTriangleElementData.size();
		element.VertexStart = (UINT32)mVertexData.size();
		element.NumVertices = (UINT32)vertices.size();
		element.Depth = depth;

		mDepthRange = std::max(mDepthRange, (UINT8)(depth + 1));

		for (auto& vertex : vertices)
			mVertexData.push_back(Vector2((float)vertex.X + 0.5f, (float)vertex.Y + 0.5f));

		mTriangleElementData.push_back(TriangleElementData());
		TriangleElementData& elemData = mTriangleElementData.back();
		elemData.MatInfo.GroupId = 0;
		elemData.MatInfo.Tint = color;

		mForceTriangleBuild = true;
		MarkContentAsDirtyInternal();
	}

	void GUICanvas::DrawText(const String& text, const Vector2I& position, const HFont& font, UINT32 size,
		const Color& color, UINT8 depth)
	{
		mElements.push_back(CanvasElement());
		CanvasElement& element = mElements.back();

		element.Type = CanvasElementType::Text;
		element.Color = color;
		element.DataId = (UINT32)mTextData.size();
		element.Size = size;
		element.TextSprite = bs_new<TextSprite>();
		element.Depth = depth;

		mDepthRange = std::max(mDepthRange, (UINT8)(depth + 1));

		mTextData.push_back({ text, font, position });
		MarkContentAsDirtyInternal();
	}

	void GUICanvas::Clear()
	{
		for (auto& element : mElements)
		{
			if(element.Type == CanvasElementType::Image && element.ImageSprite != nullptr)
				bs_delete(element.ImageSprite);

			if (element.Type == CanvasElementType::Text && element.TextSprite != nullptr)
				bs_delete(element.TextSprite);
		}

		mElements.clear();
		mRenderElements.Clear();
		mDepthRange = 1;

		mVertexData.clear();
		mImageData.clear();
		mTextData.clear();
		mTriangleElementData.clear();
		mClippedVertices.clear();
		mClippedLineVertices.clear();
		mForceTriangleBuild = false;
	}

	void GUICanvas::UpdateRenderElementsInternal()
	{
		Vector2 offset((float)mLayoutData.Area.X, (float)mLayoutData.Area.Y);
		Rect2I clipRect = mLayoutData.GetLocalClipRect();
		BuildAllTriangleElementsIfDirty(offset, clipRect);

		mRenderElements.Clear();
		for(auto& element : mElements)
		{
			element.RenderElemStart = mRenderElements.Size();
			
			switch(element.Type)
			{
			case CanvasElementType::Image:
				BuildImageElement(element);

				for(UINT32 i = 0; i < element.ImageSprite->GetNumRenderElements(); i++)
				{
					mRenderElements.Add(GUIRenderElement());
					GUIRenderElement& renderElement = mRenderElements.Back();

					element.ImageSprite->GetRenderElementInfo(i, renderElement);

					renderElement.Depth = element.Depth;
					renderElement.Type = GUIMeshType::Triangle;
				}
				
				break;
			case CanvasElementType::Text:
				BuildTextElement(element);

				for(UINT32 i = 0; i < element.TextSprite->GetNumRenderElements(); i++)
				{
					mRenderElements.Add(GUIRenderElement());
					GUIRenderElement& renderElement = mRenderElements.Back();

					element.TextSprite->GetRenderElementInfo(i, renderElement);

					renderElement.Depth = element.Depth;
					renderElement.Type = GUIMeshType::Triangle;
				}
				break;
			case CanvasElementType::Line:
				{
					mRenderElements.Add(GUIRenderElement());
					GUIRenderElement& renderElement = mRenderElements.Back();

					renderElement.NumVertices = element.ClippedNumVertices;
					renderElement.NumIndices = element.ClippedNumVertices;

					renderElement.Material = SpriteManager::Instance().GetLineMaterial();
					renderElement.MatInfo = &mTriangleElementData[element.DataId].MatInfo;
					
					renderElement.Depth = element.Depth;
					renderElement.Type = GUIMeshType::Line;

					mTriangleElementData[element.DataId].MatInfo.GroupId = (UINT64)GetParentWidgetInternal();

					// Actual mesh build happens when reading from it, because the mesh size varies due to clipping rectangle/offset
					break;
				}

			case CanvasElementType::Triangle:
				{
					mRenderElements.Add(GUIRenderElement());
					GUIRenderElement& renderElement = mRenderElements.Back();

					renderElement.NumVertices = element.ClippedNumVertices;
					renderElement.NumIndices = element.ClippedNumVertices;

					renderElement.Material = SpriteManager::Instance().GetImageMaterial(SpriteMaterialTransparency::Alpha);
					renderElement.MatInfo = &mTriangleElementData[element.DataId].MatInfo;

					renderElement.Depth = element.Depth;
					renderElement.Type = GUIMeshType::Triangle;

					mTriangleElementData[element.DataId].MatInfo.GroupId = (UINT64)GetParentWidgetInternal();

					// Actual mesh build happens when reading from it, because the mesh size varies due to clipping rectangle/offset
					break;
				}
			}

			element.RenderElemEnd = mRenderElements.Size();
		}

		GUIElement::UpdateRenderElementsInternal();
	}

	Vector2I GUICanvas::GetOptimalSizeInternal() const
	{
		return Vector2I(10, 10);
	}

	void GUICanvas::FillBuffer(
		UINT8* vertices,
		UINT32* indices,
		UINT32 vertexOffset,
		UINT32 indexOffset,
		const Vector2I& offset,
		UINT32 maxNumVerts,
		UINT32 maxNumIndices,
		UINT32 renderElementIdx) const
	{
		UINT8* uvs = vertices + sizeof(Vector2);
		UINT32 indexStride = sizeof(UINT32);

		Vector2I layoutOffset = Vector2I(mLayoutData.Area.X, mLayoutData.Area.Y) + offset;
		Rect2I clipRect = mLayoutData.GetLocalClipRect();

		Vector2 floatOffset((float)layoutOffset.X, (float)layoutOffset.Y);
		BuildAllTriangleElementsIfDirty(floatOffset, clipRect);

		const CanvasElement& element = FindElement(renderElementIdx);
		renderElementIdx -= element.RenderElemStart;

		switch(element.Type)
		{
		case CanvasElementType::Image:
		{
			UINT32 vertexStride = sizeof(Vector2) * 2;
			const Rect2I& area = mImageData[element.DataId].Area;

			layoutOffset.X += area.X;
			layoutOffset.Y += area.Y;
			clipRect.X -= area.X;
			clipRect.Y -= area.Y;

			element.ImageSprite->FillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices,
				vertexStride, indexStride, renderElementIdx, layoutOffset, clipRect);
		}
			break;
		case CanvasElementType::Text:
		{
			UINT32 vertexStride = sizeof(Vector2) * 2;
			const Vector2I& position = mTextData[element.DataId].Position;
			layoutOffset += position;
			clipRect.X -= position.X;
			clipRect.Y -= position.Y;

			element.TextSprite->FillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices,
				vertexStride, indexStride, renderElementIdx, layoutOffset, clipRect);
		}
			break;
		case CanvasElementType::Triangle:
		{
			UINT32 vertexStride = sizeof(Vector2) * 2;

			UINT32 startVert = vertexOffset;
			UINT32 startIndex = indexOffset;

			UINT32 maxVertIdx = maxNumVerts;
			UINT32 maxIndexIdx = maxNumIndices;

			UINT32 numVertices = element.ClippedNumVertices;
			UINT32 numIndices = numVertices;

			assert((startVert + numVertices) <= maxVertIdx);
			assert((startIndex + numIndices) <= maxIndexIdx);

			UINT8* vertDst = vertices + startVert * vertexStride;
			UINT8* uvDst = uvs + startVert * vertexStride;
			UINT32* indexDst = indices + startIndex;

			Vector2 zeroUV(BsZero);
			for(UINT32 i = 0; i < element.ClippedNumVertices; i++)
			{
				memcpy(vertDst, &mClippedVertices[element.ClippedVertexStart + i], sizeof(Vector2));
				memcpy(uvDst, &zeroUV, sizeof(Vector2));

				vertDst += vertexStride;
				uvDst += vertexStride;
				indexDst[i] = i;
			}
		}
			break;
		case CanvasElementType::Line:
		{
			UINT32 vertexStride = sizeof(Vector2);

			UINT32 startVert = vertexOffset;
			UINT32 startIndex = indexOffset;

			UINT32 maxVertIdx = maxNumVerts;
			UINT32 maxIndexIdx = maxNumIndices;

			UINT32 numVertices = element.ClippedNumVertices;
			UINT32 numIndices = numVertices;

			assert((startVert + numVertices) <= maxVertIdx);
			assert((startIndex + numIndices) <= maxIndexIdx);

			UINT8* vertDst = vertices + startVert * vertexStride;
			UINT32* indexDst = indices + startIndex;

			for (UINT32 i = 0; i < element.ClippedNumVertices; i++)
			{
				const Vector2& point = mClippedLineVertices[element.ClippedVertexStart + i];

				memcpy(vertDst, &point, sizeof(Vector2));

				vertDst += vertexStride;
				indexDst[i] = i;
			}
		}
		break;
		}
	}

	void GUICanvas::BuildImageElement(const CanvasElement& element)
	{
		assert(element.Type == CanvasElementType::Image);

		const ImageElementData& imageData = mImageData[element.DataId];

		IMAGE_SPRITE_DESC desc;
		desc.Width = imageData.Area.Width;
		desc.Height = imageData.Area.Height;

		desc.Transparent = true;
		desc.Color = element.Color;

		Vector2I textureSize;
		if (SpriteTexture::CheckIsLoaded(imageData.Texture))
		{
			desc.Texture = imageData.Texture;
			textureSize.X = desc.Texture->GetWidth();
			textureSize.Y = desc.Texture->GetHeight();
		}

		Vector2I destSize(mLayoutData.Area.Width, mLayoutData.Area.Height);
		desc.UvScale = ImageSprite::GetTextureUvScale(textureSize, destSize, element.ScaleMode);

		element.ImageSprite->Update(desc, (UINT64)GetParentWidgetInternal());
	}

	void GUICanvas::BuildTextElement(const CanvasElement& element)
	{
		assert(element.Type == CanvasElementType::Text);

		const TextElementData& textData = mTextData[element.DataId];

		TEXT_SPRITE_DESC desc;
		desc.Font = textData.Font;
		desc.FontSize = element.Size;
		desc.Text = textData.String;
		desc.Color = element.Color;

		element.TextSprite->Update(desc, (UINT64)GetParentWidgetInternal());
	}

	void GUICanvas::BuildTriangleElement(const CanvasElement& element, const Vector2& offset, const Rect2I& clipRect) const
	{
		assert(element.Type == CanvasElementType::Triangle || element.Type == CanvasElementType::Line);

		if (element.Type == CanvasElementType::Triangle)
		{
			UINT8* verticesToClip = (UINT8*)&mVertexData[element.VertexStart];
			UINT32 trianglesToClip = element.NumVertices / 3;

			auto writeCallback = [&](Vector2* vertices, Vector2* uvs, UINT32 count)
			{
				for (UINT32 i = 0; i < count; i++)
					mClippedVertices.push_back(vertices[i] + offset);

				element.ClippedNumVertices += count;
			};

			element.ClippedVertexStart = (UINT32)mClippedVertices.size();
			element.ClippedNumVertices = 0;

			ImageSprite::ClipTrianglesToRect(verticesToClip, nullptr, trianglesToClip, sizeof(Vector2), clipRect,
				writeCallback);
		}
		else
		{
			UINT32 numLines = element.NumVertices - 1;
			const Vector2* linePoints = &mVertexData[element.VertexStart];

			struct Plane2D
			{
				Plane2D(const Vector2& normal, float d)
					:Normal(normal), D(d)
				{ }

				Vector2 Normal;
				float D;
			};

			std::array<Plane2D, 4> clipPlanes =
			{{
				Plane2D(Vector2(1.0f, 0.0f), (float)clipRect.X),
				Plane2D(Vector2(-1.0f, 0.0f), (float)-(clipRect.X + (INT32)clipRect.Width)),
				Plane2D(Vector2(0.0f, 1.0f), (float)clipRect.Y),
				Plane2D(Vector2(0.0f, -1.0f), (float)-(clipRect.Y + (INT32)clipRect.Height))
			}};

			element.ClippedVertexStart = (UINT32)mClippedLineVertices.size();
			element.ClippedNumVertices = 0;

			for (UINT32 i = 0; i < numLines; i++)
			{
				Vector2 a = linePoints[i];
				Vector2 b = linePoints[i + 1];

				bool isVisible = true;
				for(UINT32 j = 0; j < (UINT32)clipPlanes.size(); j++)
				{
					const Plane2D& plane = clipPlanes[j];
					float d0 = plane.Normal.Dot(a) - plane.D;
					float d1 = plane.Normal.Dot(b) - plane.D;

					// Line not visible
					if (d0 <= 0 && d1 <= 0)
					{
						isVisible = false;
						break;
					}

					// Line visible completely
					if (d0 >= 0 && d1 >= 0)
						continue;

					// The line is split by the plane, compute the point of intersection.
					float t = d0 / (d0 - d1);
					Vector2 intersectPt = (1 - t)*a + t*b;

					if (d0 > 0)
						b = intersectPt;
					else
						a = intersectPt;
				}

				if (!isVisible)
					continue;

				mClippedLineVertices.push_back(a + offset);
				mClippedLineVertices.push_back(b + offset);

				element.ClippedNumVertices += 2;
			}
		}
	}

	void GUICanvas::BuildAllTriangleElementsIfDirty(const Vector2& offset, const Rect2I& clipRect) const
	{
		// We need to rebuild if new triangle element(s) were added, or if offset or clip rectangle changed
		bool isDirty = mForceTriangleBuild || (mLastOffset != offset) || (mLastClipRect != clipRect);
		if (!isDirty)
			return;

		mClippedVertices.clear();
		mClippedLineVertices.clear();
		for(auto& element : mElements)
		{
			if (element.Type != CanvasElementType::Triangle && element.Type != CanvasElementType::Line)
				continue;

			BuildTriangleElement(element, offset, clipRect);
		}

		mLastOffset = offset;
		mLastClipRect = clipRect;
		mForceTriangleBuild = false;
	}

	const GUICanvas::CanvasElement& GUICanvas::FindElement(UINT32 renderElementIdx) const
	{
		INT32 start = 0;
		INT32 end = (INT32)(mElements.size() - 1);

		while (start <= end)
		{
			INT32 middle = (start + end) / 2;
			const CanvasElement& current = mElements[middle];

			if (renderElementIdx >= current.RenderElemStart && renderElementIdx < current.RenderElemEnd)
				return current;

			if (renderElementIdx < current.RenderElemStart)
				end = middle - 1;
			else
				start = middle + 1;
		}

		BS_EXCEPT(InvalidParametersException, "Cannot find requested GUI render element.");
	}
}
