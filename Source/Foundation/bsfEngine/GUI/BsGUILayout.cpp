//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIInteractable.h"
#include "Reflection/BsRTTIType.h"

using namespace bs;

/** @cond RTTI */
/** @addtogroup RTTI-Impl-Engine
 *  @{
 */

namespace bs
{
	class B3D_EXPORT GUILayoutRTTI : public TRTTIType<GUILayout, GUIElement, GUILayoutRTTI>
	{
	public:
		const String& GetRttiName()
		{
			static String name = "GUILayout";
			return name;
		}

		u32 GetRttiId() const override { return TID_GUILayout; }

		SPtr<IReflectable> NewRttiObject() { return nullptr; }
	};
} // namespace bs

/** @} */
/** @endcond */

GUILayout::GUILayout(const GUISizeConstraints& dimensions)
	: GUIElement(dimensions)
{}

void GUILayout::AddElement(GUIElement* element)
{
	if(element->IsPendingDestroy())
		return;

	RegisterChildElement(element);

	if(mIsCullingEnabled)
	{
		B3D_ENSURE(!element->GetQuadTreeId().IsValid());
		mQuadTree->AddElement(element);
	}
}

void GUILayout::RemoveElement(GUIElement* element)
{
	if(mIsCullingEnabled)
	{
		const SpatialTreeElementId& quadTreeId = element->GetQuadTreeId();
		if(B3D_ENSURE(quadTreeId.IsValid()))
		{
			mQuadTree->RemoveElement(quadTreeId);
			element->SetQuadTreeId(SpatialTreeElementId());
		}

		// TODO - Remove from visible list if not culled
	}

	UnregisterChildElement(element);
}

void GUILayout::InsertElement(u32 idx, GUIElement* element)
{
	if(idx > (u32)mChildren.size())
		B3D_EXCEPT(InvalidParametersException, "Index out of range: " + ToString(idx) + ". Valid range: 0 .. " + ToString((u32)mChildren.size()));

	if(element->IsPendingDestroy())
		return;

	GUIElement* parentElement = element->GetParent();
	if(parentElement != nullptr)
	{
		parentElement->UnregisterChildElement(element);
	}

	element->SetParent(this);
	mChildren.Insert(mChildren.begin() + idx, element);

	element->SetActiveRecursive(IsActive());
	element->SetHiddenRecursive(IsHidden());
	element->SetDisabledRecursive(IsDisabled());

	if(mIsCullingEnabled)
	{
		B3D_ENSURE(!element->GetQuadTreeId().IsValid());
		mQuadTree->AddElement(element);
	}

	MarkLayoutAsDirty();
}

void GUILayout::Clear()
{
	if(mIsCullingEnabled)
	{
		ClearQuadTreeElementIds();
		mQuadTree = B3DMakeUnique<GUIElementQuadTree>(Vector2(-kMaximumQuadtreeSize * 0.5f, -kMaximumQuadtreeSize * 0.5f), kMaximumQuadtreeSize);

		// TODO - Clear Culled flag
	}

	DestroyChildElements();
}

void GUILayout::RemoveElementAt(u32 idx)
{
	if(idx >= (u32)mChildren.size())
		B3D_EXCEPT(InvalidParametersException, "Index out of range: " + ToString(idx) + ". Valid range: 0 .. " + ToString((u32)mChildren.size()));

	GUIElement* child = mChildren[idx];
	mChildren.erase(mChildren.begin() + idx);

	child->SetParent(nullptr);

	if(mIsCullingEnabled)
	{
		const SpatialTreeElementId& quadTreeId = child->GetQuadTreeId();
		if(B3D_ENSURE(quadTreeId.IsValid()))
		{
			mQuadTree->RemoveElement(quadTreeId);
			child->SetQuadTreeId(SpatialTreeElementId());
		}

		// TODO - Remove from visible list if not culled
	}

	MarkLayoutAsDirty();
}

void GUILayout::SetEnableCulling(bool enable)
{
	if(mIsCullingEnabled == enable)
		return;
	
	mIsCullingEnabled = enable;

	if(enable)
		RebuildQuadTree();
	else
	{
		ClearQuadTreeElementIds();
		mQuadTree = nullptr;

		// TODO - Clear Culled flag from GUI elements
	}
}

void GUILayout::RebuildQuadTree()
{
	B3D_ENSURE(mIsCullingEnabled);

	mQuadTree = B3DMakeUnique<GUIElementQuadTree>(Vector2(-kMaximumQuadtreeSize * 0.5f, -kMaximumQuadtreeSize * 0.5f), kMaximumQuadtreeSize);

	for(const auto& child : mChildren)
		mQuadTree->AddElement(child);
}

void GUILayout::ClearQuadTreeElementIds()
{
	for(const auto& element : mChildren)
	{
		const SpatialTreeElementId& quadTreeId = element->GetQuadTreeId();
		if(B3D_ENSURE(quadTreeId.IsValid()))
		{
			element->SetQuadTreeId(SpatialTreeElementId());
		}
	}
}

void GUILayout::UpdateAbsoluteCoordinatesForChildren()
{
#if 0 // WIP
	if(mIsCullingEnabled)
	{
		Rect2 relativeClippedArea = (Rect2)mAbsoluteClippedArea;
		relativeClippedArea.X -= (float)mAbsolutePosition.X;
		relativeClippedArea.Y -= (float)mAbsolutePosition.Y;

		GUIElementQuadTree::AreaIntersectIterator areaIterator(*mQuadTree, relativeClippedArea);
		while(areaIterator.MoveNext())
		{
			GUIElement* const element = areaIterator.GetElement();

			// TODO - Register visible element, clear culled flag
		}

		// TODO - Clear previously visible elements
		// TODO - Set culled flag on all other elements
		//  - To avoid iterating over all elements, likely need to keep track of which elements are new and haven't been checked yet (or add them to visible list immediately)
		
	}
	else
#endif
	{
		for(auto& child : mChildren)
		{
			child->UpdateAbsoluteCoordinates(mAbsolutePosition, mAbsoluteClippedArea);
			child->UpdateAbsoluteCoordinatesForChildren();
		}
	}
}

RTTIType* GUILayout::GetRttiStatic()
{
	return GUILayoutRTTI::Instance();
}

RTTIType* GUILayout::GetRtti() const
{
	return GetRttiStatic();
}
