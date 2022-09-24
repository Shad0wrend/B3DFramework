//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/BsRenderQueue.h"
#include "RenderAPI/BsSubMesh.h"
#include "Material/BsShader.h"
#include "Mesh/BsMesh.h"
#include "Material/BsMaterial.h"
#include "Renderer/BsRenderElement.h"

using namespace std::placeholders;

namespace bs { namespace ct
{
	RenderQueue::RenderQueue(StateReduction mode)
		:mStateReductionMode(mode)
	{

	}

	void RenderQueue::Clear()
	{
		mSortableElements.clear();
		mSortableElementIdx.clear();
		mElements.clear();

		mSortedRenderElements.clear();
	}

	void RenderQueue::Add(const RenderElement* element, float distFromCamera, UINT32 techniqueIdx)
	{
		SPtr<Material> material = element->Material;
		SPtr<Shader> shader = material->GetShader();
		
		UINT32 queuePriority = shader->GetQueuePriority();
		QueueSortType sortType = shader->GetQueueSortType();
		UINT32 shaderId = shader->GetId();
		bool separablePasses = shader->GetAllowSeparablePasses();

		switch (sortType)
		{
		case QueueSortType::None:
			distFromCamera = 0;
			break;
		case QueueSortType::BackToFront:
			distFromCamera = -distFromCamera;
			break;
		case QueueSortType::FrontToBack:
			break;
		}

		UINT32 numPasses = material->GetNumPasses(techniqueIdx);
		if (!separablePasses)
			numPasses = std::min(1U, numPasses);

		for (UINT32 i = 0; i < numPasses; i++)
		{
			UINT32 idx = (UINT32)mSortableElementIdx.size();
			mSortableElementIdx.push_back(idx);

			mSortableElements.push_back(SortableElement());
			SortableElement& sortableElem = mSortableElements.back();

			sortableElem.SeqIdx = idx;
			sortableElem.Priority = queuePriority;
			sortableElem.ShaderId = shaderId;
			sortableElem.TechniqueIdx = techniqueIdx;
			sortableElem.PassIdx = i;
			sortableElem.DistFromCamera = distFromCamera;

			mElements.push_back(element);
		}
	}

	void RenderQueue::Sort()
	{
		std::function<bool(UINT32, UINT32, const Vector<SortableElement>&)> sortMethod;

		switch (mStateReductionMode)
		{
		case StateReduction::None:
			sortMethod = &ElementSorterNoGroup;
			break;
		case StateReduction::Material:
			sortMethod = &ElementSorterPreferGroup;
			break;
		case StateReduction::Distance:
			sortMethod = &ElementSorterPreferDistance;
			break;
		}

		// Sort only indices since we generate an entirely new data set anyway, it doesn't make sense to move sortable elements
		std::sort(mSortableElementIdx.begin(), mSortableElementIdx.end(), std::bind(sortMethod, _1, _2, mSortableElements));

		UINT32 prevShaderId = (UINT32)-1;
		UINT32 prevTechniqueIdx = (UINT32)-1;
		UINT32 prevPassIdx = (UINT32)-1;
		for (UINT32 i = 0; i < (UINT32)mSortableElementIdx.size(); i++)
		{
			const UINT32 idx = mSortableElementIdx[i];
			const SortableElement& elem = mSortableElements[idx];
			const RenderElement* renderElem = mElements[idx];

			const bool separablePasses = renderElem->Material->GetShader()->GetAllowSeparablePasses();

			if (separablePasses)
			{
				mSortedRenderElements.push_back(RenderQueueElement());

				RenderQueueElement& sortedElem = mSortedRenderElements.back();
				sortedElem.RenderElem = renderElem;
				sortedElem.TechniqueIdx = elem.TechniqueIdx;
				sortedElem.PassIdx = elem.PassIdx;

				if (prevShaderId != elem.ShaderId || prevTechniqueIdx != elem.TechniqueIdx ||  prevPassIdx != elem.PassIdx)
				{
					sortedElem.ApplyPass = true;
					prevShaderId = elem.ShaderId;
					prevTechniqueIdx = elem.TechniqueIdx;
					prevPassIdx = elem.PassIdx;
				}
				else
					sortedElem.ApplyPass = false;
			}
			else
			{
				const UINT32 numPasses = renderElem->Material->GetNumPasses(elem.TechniqueIdx);
				for (UINT32 j = 0; j < numPasses; j++)
				{
					mSortedRenderElements.push_back(RenderQueueElement());

					RenderQueueElement& sortedElem = mSortedRenderElements.back();
					sortedElem.RenderElem = renderElem;
					sortedElem.TechniqueIdx = elem.TechniqueIdx;
					sortedElem.PassIdx = j;

					if (prevShaderId != elem.ShaderId || prevTechniqueIdx != elem.TechniqueIdx || prevPassIdx != j)
					{
						sortedElem.ApplyPass = true;
						prevShaderId = elem.ShaderId;
						prevTechniqueIdx = elem.TechniqueIdx;
						prevPassIdx = j;
					}
					else
						sortedElem.ApplyPass = false;
				}
			}
		}
	}

	bool RenderQueue::ElementSorterNoGroup(UINT32 aIdx, UINT32 bIdx, const Vector<SortableElement>& lookup)
	{
		const SortableElement& a = lookup[aIdx];
		const SortableElement& b = lookup[bIdx];

		UINT8 isHigher = (a.Priority > b.Priority) << 2 |
			(a.DistFromCamera < b.DistFromCamera) << 1 |
			(a.SeqIdx < b.SeqIdx);

		UINT8 isLower = (a.Priority < b.Priority) << 2 |
			(a.DistFromCamera > b.DistFromCamera) << 1 |
			(a.SeqIdx > b.SeqIdx);

		return isHigher > isLower;
	}

	bool RenderQueue::ElementSorterPreferGroup(UINT32 aIdx, UINT32 bIdx, const Vector<SortableElement>& lookup)
	{
		const SortableElement& a = lookup[aIdx];
		const SortableElement& b = lookup[bIdx];
		
		UINT8 isHigher = (a.Priority > b.Priority) << 5 |
			(a.ShaderId < b.ShaderId) << 4 |
			(a.TechniqueIdx < b.TechniqueIdx) << 3 |
			(a.PassIdx < b.PassIdx) << 2 |
			(a.DistFromCamera < b.DistFromCamera) << 1 |
			(a.SeqIdx < b.SeqIdx);

		UINT8 isLower = (a.Priority < b.Priority) << 5 |
			(a.ShaderId > b.ShaderId) << 4 |
			(a.TechniqueIdx > b.TechniqueIdx) << 3 |
			(a.PassIdx > b.PassIdx) << 2 |
			(a.DistFromCamera > b.DistFromCamera) << 1 |
			(a.SeqIdx > b.SeqIdx);

		return isHigher > isLower;
	}

	bool RenderQueue::ElementSorterPreferDistance(UINT32 aIdx, UINT32 bIdx, const Vector<SortableElement>& lookup)
	{
		const SortableElement& a = lookup[aIdx];
		const SortableElement& b = lookup[bIdx];

		UINT8 isHigher = (a.Priority > b.Priority) << 5 |
			(a.DistFromCamera < b.DistFromCamera) << 4 |
			(a.ShaderId < b.ShaderId) << 3 |
			(a.TechniqueIdx < b.TechniqueIdx) << 2 |
			(a.PassIdx < b.PassIdx) << 1 |
			(a.SeqIdx < b.SeqIdx);

		UINT8 isLower = (a.Priority < b.Priority) << 5 |
			(a.DistFromCamera > b.DistFromCamera) << 4 |
			(a.ShaderId > b.ShaderId) << 3 |
			(a.TechniqueIdx > b.TechniqueIdx) << 2 |
			(a.PassIdx > b.PassIdx) << 1 |
			(a.SeqIdx > b.SeqIdx);

		return isHigher > isLower;
	}

	const Vector<RenderQueueElement>& RenderQueue::GetSortedElements() const
	{
		return mSortedRenderElements;
	}
}}
