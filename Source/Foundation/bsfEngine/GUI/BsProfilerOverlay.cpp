//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsProfilerOverlay.h"
#include "Scene/BsSceneObject.h"
#include "GUI/BsCGUIWidget.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUIElement.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUISpace.h"
#include "RenderAPI/BsViewport.h"
#include "Utility/BsTime.h"
#include "Resources/BsBuiltinResources.h"
#include "Profiling/BsProfilingManager.h"
#include "RenderAPI/BsRenderTarget.h"
#include "Renderer/BsCamera.h"
#include "Localization/BsHEString.h"

#define BS_SHOW_PRECISE_PROFILING 0

namespace bs
{
	constexpr UINT32 MAX_DEPTH = 4;

	class BasicRowFiller
	{
	public:
		UINT32 curIdx;
		GUILayout& labelLayout;
		GUILayout& contentLayout;
		GUIWidget& widget;
		Vector<ProfilerOverlay::BasicRow>& rows;

		BasicRowFiller(Vector<ProfilerOverlay::BasicRow>& _rows, GUILayout& _labelLayout, GUILayout& _contentLayout, GUIWidget& _widget)
			:curIdx(0), labelLayout(_labelLayout), contentLayout(_contentLayout), widget(_widget), rows(_rows)
		{ }

		~BasicRowFiller()
		{
			UINT32 excessEntries = (UINT32)rows.size() - curIdx;
			for(UINT32 i = 0; i < excessEntries; i++)
			{
				ProfilerOverlay::BasicRow& row = rows[curIdx + i];

				if (!row.disabled)
				{
					row.labelLayout->SetVisible(false);
					row.contentLayout->SetVisible(false);
					row.disabled = true;
				}
			}

			rows.resize(curIdx);
		}

		void AddData(UINT32 depth, const String& name, float pctOfParent, UINT32 numCalls, UINT64 numAllocs,
			UINT64 numFrees, double avgTime, double totalTime, double avgSelfTime, double totalSelfTime)
		{
			if(curIdx >= rows.size())
			{
				rows.push_back(ProfilerOverlay::BasicRow());

				ProfilerOverlay::BasicRow& newRow = rows.back();

				newRow.disabled = false;
				newRow.name = HEString(u8"{0}");
				newRow.pctOfParent = HEString(u8"{0} %");
				newRow.numCalls = HEString(u8"{0}");
				newRow.numAllocs = HEString(u8"{0}");
				newRow.numFrees = HEString(u8"{0}");
				newRow.avgTime = HEString(u8"{0}");
				newRow.totalTime = HEString(u8"{0}");
				newRow.avgTimeSelf = HEString(u8"{0}");
				newRow.totalTimeSelf = HEString(u8"{0}");

				newRow.labelLayout = labelLayout.InsertNewElement<GUILayoutX>(labelLayout.GetNumChildren() - 1); // Insert before flexible space
				newRow.contentLayout = contentLayout.InsertNewElement<GUILayoutX>(contentLayout.GetNumChildren() - 1); // Insert before flexible space

				newRow.labelSpace = newRow.labelLayout->AddNewElement<GUIFixedSpace>(0);
				newRow.guiName = newRow.labelLayout->AddNewElement<GUILabel>(newRow.name, GUIOptions(GUIOption::FixedWidth(200)));

				newRow.guiPctOfParent = newRow.contentLayout->AddNewElement<GUILabel>(newRow.pctOfParent, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumCalls = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numCalls, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumAllocs = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numAllocs, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumFrees = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numFrees, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiAvgTime = newRow.contentLayout->AddNewElement<GUILabel>(newRow.avgTime, GUIOptions(GUIOption::FixedWidth(60)));
				newRow.guiTotalTime = newRow.contentLayout->AddNewElement<GUILabel>(newRow.totalTime, GUIOptions(GUIOption::FixedWidth(60)));
				newRow.guiAvgTimeSelf = newRow.contentLayout->AddNewElement<GUILabel>(newRow.avgTimeSelf, GUIOptions(GUIOption::FixedWidth(100)));
				newRow.guiTotalTimeSelf = newRow.contentLayout->AddNewElement<GUILabel>(newRow.totalTimeSelf, GUIOptions(GUIOption::FixedWidth(100)));
			}
			
			ProfilerOverlay::BasicRow& row = rows[curIdx];

			row.labelSpace->SetSize(depth * 20);
			row.name.SetParameter(0, name);
			row.pctOfParent.SetParameter(0, toString(pctOfParent * 100.0f, 2, 0, ' ', std::ios::fixed));
			row.numCalls.SetParameter(0, toString(numCalls));
			row.numAllocs.SetParameter(0, toString(numAllocs));
			row.numFrees.SetParameter(0, toString(numFrees));
			row.avgTime.SetParameter(0, toString(avgTime, 2, 0, ' ', std::ios::fixed));
			row.totalTime.SetParameter(0, toString(totalTime, 2, 0, ' ', std::ios::fixed));
			row.avgTimeSelf.SetParameter(0, toString(avgSelfTime, 2, 0, ' ', std::ios::fixed));
			row.totalTimeSelf.SetParameter(0, toString(totalSelfTime, 2, 0, ' ', std::ios::fixed));

			row.guiName->SetContent(row.name);
			row.guiPctOfParent->SetContent(row.pctOfParent);
			row.guiNumCalls->SetContent(row.numCalls);
			row.guiNumAllocs->SetContent(row.numAllocs);
			row.guiNumFrees->SetContent(row.numFrees);
			row.guiAvgTime->SetContent(row.avgTime);
			row.guiTotalTime->SetContent(row.totalTime);
			row.guiAvgTimeSelf->SetContent(row.avgTimeSelf);
			row.guiTotalTimeSelf->SetContent(row.totalTimeSelf);

			if (row.disabled)
			{
				row.labelLayout->SetVisible(true);
				row.contentLayout->SetVisible(true);
				row.disabled = false;
			}

			curIdx++;
		}
	};

	class PreciseRowFiller
	{
	public:
		UINT32 curIdx;
		GUILayout& labelLayout;
		GUILayout& contentLayout;
		GUIWidget& widget;
		Vector<ProfilerOverlay::PreciseRow>& rows;

		PreciseRowFiller(Vector<ProfilerOverlay::PreciseRow>& _rows, GUILayout& _labelLayout, GUILayout& _contentLayout, GUIWidget& _widget)
			:curIdx(0), labelLayout(_labelLayout), contentLayout(_contentLayout), widget(_widget), rows(_rows)
		{ }

		~PreciseRowFiller()
		{
			UINT32 excessEntries = (UINT32)rows.size() - curIdx;
			for(UINT32 i = 0; i < excessEntries; i++)
			{
				ProfilerOverlay::PreciseRow& row = rows[curIdx + i];

				if (!row.disabled)
				{
					row.labelLayout->SetVisible(false);
					row.contentLayout->SetVisible(false);
					row.disabled = true;
				}
			}

			rows.resize(curIdx);
		}

		void AddData(UINT32 depth, const String& name, float pctOfParent, UINT32 numCalls, UINT64 numAllocs,
			UINT64 numFrees, UINT64 avgCycles, UINT64 totalCycles, UINT64 avgSelfCycles, UINT64 totalSelfCycles)
		{
			if(curIdx >= rows.size())
			{
				rows.push_back(ProfilerOverlay::PreciseRow());

				ProfilerOverlay::PreciseRow& newRow = rows.back();

				newRow.disabled = false;
				newRow.name = HEString(u8"{0}");
				newRow.pctOfParent = HEString(u8"{0}");
				newRow.numCalls = HEString(u8"{0}");
				newRow.numAllocs = HEString(u8"{0}");
				newRow.numFrees = HEString(u8"{0}");
				newRow.avgCycles = HEString(u8"{0}");
				newRow.totalCycles = HEString(u8"{0}");
				newRow.avgCyclesSelf = HEString(u8"{0}");
				newRow.totalCyclesSelf = HEString(u8"{0}");

				newRow.labelLayout = labelLayout.InsertNewElement<GUILayoutX>(labelLayout.GetNumChildren() - 1); // Insert before flexible space
				newRow.contentLayout = contentLayout.InsertNewElement<GUILayoutX>(contentLayout.GetNumChildren() - 1); // Insert before flexible space

				newRow.labelSpace = newRow.labelLayout->AddNewElement<GUIFixedSpace>(0);
				newRow.guiName = newRow.labelLayout->AddNewElement<GUILabel>(newRow.name, GUIOptions(GUIOption::FixedWidth(200)));

				newRow.guiPctOfParent = newRow.contentLayout->AddNewElement<GUILabel>(newRow.pctOfParent, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumCalls = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numCalls, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumAllocs = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numAllocs, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiNumFrees = newRow.contentLayout->AddNewElement<GUILabel>(newRow.numFrees, GUIOptions(GUIOption::FixedWidth(50)));
				newRow.guiAvgCycles = newRow.contentLayout->AddNewElement<GUILabel>(newRow.avgCycles, GUIOptions(GUIOption::FixedWidth(60)));
				newRow.guiTotalCycles = newRow.contentLayout->AddNewElement<GUILabel>(newRow.totalCycles, GUIOptions(GUIOption::FixedWidth(60)));
				newRow.guiAvgCyclesSelf = newRow.contentLayout->AddNewElement<GUILabel>(newRow.avgCyclesSelf, GUIOptions(GUIOption::FixedWidth(100)));
				newRow.guiTotalCyclesSelf = newRow.contentLayout->AddNewElement<GUILabel>(newRow.totalCyclesSelf, GUIOptions(GUIOption::FixedWidth(100)));
			}

			ProfilerOverlay::PreciseRow& row = rows[curIdx];

			row.labelSpace->SetSize(depth * 20);
			row.name.SetParameter(0, name);
			row.pctOfParent.SetParameter(0, toString(pctOfParent * 100.0f, 2, 0, ' ', std::ios::fixed));
			row.numCalls.SetParameter(0, toString(numCalls));
			row.numAllocs.SetParameter(0, toString(numAllocs));
			row.numFrees.SetParameter(0, toString(numFrees));
			row.avgCycles.SetParameter(0, toString(avgCycles));
			row.totalCycles.SetParameter(0, toString(totalCycles));
			row.avgCyclesSelf.SetParameter(0, toString(avgSelfCycles));
			row.totalCyclesSelf.SetParameter(0, toString(totalSelfCycles));

			row.guiName->SetContent(row.name);
			row.guiPctOfParent->SetContent(row.pctOfParent);
			row.guiNumCalls->SetContent(row.numCalls);
			row.guiNumAllocs->SetContent(row.numAllocs);
			row.guiNumFrees->SetContent(row.numFrees);
			row.guiAvgCycles->SetContent(row.avgCycles);
			row.guiTotalCycles->SetContent(row.totalCycles);
			row.guiAvgCyclesSelf->SetContent(row.avgCyclesSelf);
			row.guiTotalCyclesSelf->SetContent(row.totalCyclesSelf);

			if (row.disabled)
			{
				row.labelLayout->SetVisible(true);
				row.contentLayout->SetVisible(true);
				row.disabled = false;
			}

			curIdx++;
		}
	};

	class GPUSampleRowFiller
	{
	public:
		UINT32 curIdx;
		GUILayout& labelLayout;
		GUILayout& contentLayout;
		GUIWidget& widget;
		Vector<ProfilerOverlay::GPUSampleRow>& rows;

		GPUSampleRowFiller(Vector<ProfilerOverlay::GPUSampleRow>& rows, GUILayout& labelLayout, GUILayout& contentLayout,
			GUIWidget& _widget)
			:curIdx(0), labelLayout(labelLayout), contentLayout(contentLayout), widget(_widget), rows(rows)
		{ }

		~GPUSampleRowFiller()
		{
			UINT32 excessEntries = (UINT32)rows.size() - curIdx;
			for (UINT32 i = 0; i < excessEntries; i++)
			{
				ProfilerOverlay::GPUSampleRow& row = rows[curIdx + i];

				if (!row.disabled)
				{
					row.labelLayout->SetVisible(false);
					row.contentLayout->SetVisible(false);
					row.disabled = true;
				}
			}

			rows.resize(curIdx);
		}

		void AddData(UINT32 depth, const String& name, float timeMs)
		{
			if (curIdx >= rows.size())
			{
				rows.push_back(ProfilerOverlay::GPUSampleRow());

				ProfilerOverlay::GPUSampleRow& newRow = rows.back();

				newRow.disabled = false;
				newRow.name = HEString(u8"{1}");
				newRow.time = HEString(u8"{0}");

				newRow.labelLayout = labelLayout.InsertNewElement<GUILayoutX>(labelLayout.GetNumChildren() - 1); // Insert before flexible space
				newRow.contentLayout = contentLayout.InsertNewElement<GUILayoutX>(contentLayout.GetNumChildren() - 1); // Insert before flexible space

				newRow.labelSpace = newRow.labelLayout->AddNewElement<GUIFixedSpace>(0);
				newRow.guiName = newRow.labelLayout->AddNewElement<GUILabel>(newRow.name, GUIOptions(GUIOption::FixedWidth(200)));

				newRow.guiTime = newRow.contentLayout->AddNewElement<GUILabel>(newRow.time, GUIOptions(GUIOption::FixedWidth(100)));
			}

			ProfilerOverlay::GPUSampleRow& row = rows[curIdx];

			row.labelSpace->SetSize(depth * 20);
			row.name.SetParameter(0, name);
			row.time.SetParameter(0, toString(timeMs));

			row.guiName->SetContent(row.name);
			row.guiTime->SetContent(row.time);

			if (row.disabled)
			{
				row.labelLayout->SetVisible(false);
				row.contentLayout->SetVisible(false);
				row.disabled = false;
			}

			curIdx++;
		}
	};

	ProfilerOverlay::ProfilerOverlay(const SPtr<Camera>& camera)
		:mType(ProfilerOverlayType::CPUSamples), mIsShown(true)
	{
		SetTarget(camera);
	}

	ProfilerOverlay::~ProfilerOverlay()
	{
		if(mTarget != nullptr)
			mTargetResizedConn.Disconnect();

		if(mWidgetSO)
			mWidgetSO->Destroy();
	}

	void ProfilerOverlay::SetTarget(const SPtr<Camera>& camera)
	{
		if(mTarget != nullptr)
			mTargetResizedConn.Disconnect();

		mTarget = camera->GetViewport();

		mTargetResizedConn = mTarget->GetTarget()->onResized.Connect(std::bind(&ProfilerOverlay::TargetResized, this));

		if(mWidgetSO)
			mWidgetSO->Destroy();

		mWidgetSO = SceneObject::Create("ProfilerOverlay", SOF_Internal | SOF_Persistent | SOF_DontSave);
		mWidget = mWidgetSO->AddComponent<CGUIWidget>(camera);
		mWidget->SetDepth(127);
		mWidget->SetSkin(BuiltinResources::Instance().GetGuiSkin());

		// Set up CPU sample areas
		mBasicLayoutLabels = mWidget->GetPanel()->AddNewElement<GUILayoutY>();
		mPreciseLayoutLabels = mWidget->GetPanel()->AddNewElement<GUILayoutY>();
		mBasicLayoutContents = mWidget->GetPanel()->AddNewElement<GUILayoutY>();
		mPreciseLayoutContents = mWidget->GetPanel()->AddNewElement<GUILayoutY>();

		// Set up CPU sample title bars
		mTitleBasicName = GUILabel::Create(HEString(u8"Name"), GUIOptions(GUIOption::FixedWidth(200)));
		mTitleBasicPctOfParent = GUILabel::Create(HEString(u8"% parent"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitleBasicNumCalls = GUILabel::Create(HEString(u8"# calls"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitleBasicNumAllocs = GUILabel::Create(HEString(u8"# allocs"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitleBasicNumFrees = GUILabel::Create(HEString(u8"# frees"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitleBasicAvgTime = GUILabel::Create(HEString(u8"Avg. time"), GUIOptions(GUIOption::FixedWidth(60)));
		mTitleBasicTotalTime = GUILabel::Create(HEString(u8"Total time"), GUIOptions(GUIOption::FixedWidth(60)));
		mTitleBasicAvgTitleSelf = GUILabel::Create(HEString(u8"Avg. self time"), GUIOptions(GUIOption::FixedWidth(100)));
		mTitleBasicTotalTimeSelf = GUILabel::Create(HEString(u8"Total self time"), GUIOptions(GUIOption::FixedWidth(100)));

		mTitlePreciseName = GUILabel::Create(HEString(u8"Name"), GUIOptions(GUIOption::FixedWidth(200)));
		mTitlePrecisePctOfParent = GUILabel::Create(HEString(u8"% parent"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitlePreciseNumCalls = GUILabel::Create(HEString(u8"# calls"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitlePreciseNumAllocs = GUILabel::Create(HEString(u8"# allocs"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitlePreciseNumFrees = GUILabel::Create(HEString(u8"# frees"), GUIOptions(GUIOption::FixedWidth(50)));
		mTitlePreciseAvgCycles = GUILabel::Create(HEString(u8"Avg. cycles"), GUIOptions(GUIOption::FixedWidth(60)));
		mTitlePreciseTotalCycles = GUILabel::Create(HEString(u8"Total cycles"), GUIOptions(GUIOption::FixedWidth(60)));
		mTitlePreciseAvgCyclesSelf = GUILabel::Create(HEString(u8"Avg. self cycles"), GUIOptions(GUIOption::FixedWidth(100)));
		mTitlePreciseTotalCyclesSelf = GUILabel::Create(HEString(u8"Total self cycles"), GUIOptions(GUIOption::FixedWidth(100)));

		GUILayout* basicTitleLabelLayout = mBasicLayoutLabels->AddNewElement<GUILayoutX>();
		GUILayout* preciseTitleLabelLayout = mPreciseLayoutLabels->AddNewElement<GUILayoutX>();
		GUILayout* basicTitleContentLayout = mBasicLayoutContents->AddNewElement<GUILayoutX>();
		GUILayout* preciseTitleContentLayout = mPreciseLayoutContents->AddNewElement<GUILayoutX>();

		basicTitleLabelLayout->AddElement(mTitleBasicName);
		basicTitleContentLayout->AddElement(mTitleBasicPctOfParent);
		basicTitleContentLayout->AddElement(mTitleBasicNumCalls);
		basicTitleContentLayout->AddElement(mTitleBasicNumAllocs);
		basicTitleContentLayout->AddElement(mTitleBasicNumFrees);
		basicTitleContentLayout->AddElement(mTitleBasicAvgTime);
		basicTitleContentLayout->AddElement(mTitleBasicTotalTime);
		basicTitleContentLayout->AddElement(mTitleBasicAvgTitleSelf);
		basicTitleContentLayout->AddElement(mTitleBasicTotalTimeSelf);

		preciseTitleLabelLayout->AddElement(mTitlePreciseName);
		preciseTitleContentLayout->AddElement(mTitlePrecisePctOfParent);
		preciseTitleContentLayout->AddElement(mTitlePreciseNumCalls);
		preciseTitleContentLayout->AddElement(mTitlePreciseNumAllocs);
		preciseTitleContentLayout->AddElement(mTitlePreciseNumFrees);
		preciseTitleContentLayout->AddElement(mTitlePreciseAvgCycles);
		preciseTitleContentLayout->AddElement(mTitlePreciseTotalCycles);
		preciseTitleContentLayout->AddElement(mTitlePreciseAvgCyclesSelf);
		preciseTitleContentLayout->AddElement(mTitlePreciseTotalCyclesSelf);

		mBasicLayoutLabels->AddNewElement<GUIFlexibleSpace>();
		mPreciseLayoutLabels->AddNewElement<GUIFlexibleSpace>();
		mBasicLayoutContents->AddNewElement<GUIFlexibleSpace>();
		mPreciseLayoutContents->AddNewElement<GUIFlexibleSpace>();

#if BS_SHOW_PRECISE_PROFILING == 0
		mPreciseLayoutLabels->SetActive(false);
		mPreciseLayoutContents->SetActive(false);
#endif

		// Set up GPU sample areas
		mGPULayoutFrameContents = mWidget->GetPanel()->AddNewElement<GUILayoutX>();
		mGPULayoutFrameContentsLeft = mGPULayoutFrameContents->AddNewElement<GUILayoutY>();
		mGPULayoutFrameContentsRight = mGPULayoutFrameContents->AddNewElement<GUILayoutY>();

		mGPULayoutSamples = mWidget->GetPanel()->AddNewElement<GUIPanel>();

		HString gpuSamplesStr(u8"__ProfOvGPUSamples", u8"Samples");
		mGPULayoutSamples->AddNewElement<GUILabel>(gpuSamplesStr);

		for(UINT32 i = 0; i < GPU_NUM_SAMPLE_COLUMNS; i++)
		{
			mGPULayoutSampleLabels[i] = mGPULayoutSamples->AddNewElement<GUILayoutY>();
			mGPULayoutSampleContents[i] = mGPULayoutSamples->AddNewElement<GUILayoutY>();

			HString gpuSamplesNameStr(u8"__ProfOvGPUSampName", u8"Name");
			HString gpuSamplesTimeStr(u8"__ProfOvGPUSampTime", u8"Time");
			mGPULayoutSampleLabels[i]->AddElement(GUILabel::Create(gpuSamplesNameStr, GUIOptions(GUIOption::FixedWidth(200))));
			mGPULayoutSampleContents[i]->AddElement(GUILabel::Create(gpuSamplesTimeStr, GUIOptions(GUIOption::FixedWidth(100))));

			mGPULayoutSampleLabels[i]->AddNewElement<GUIFlexibleSpace>();
			mGPULayoutSampleContents[i]->AddNewElement<GUIFlexibleSpace>();
		}

		mGPUFrameNumStr = HEString(u8"__ProfOvFrame", u8"Frame #{0}");
		mGPUTimeStr = HEString(u8"__ProfOvTime", u8"Time: {0}ms");
		mGPUDrawCallsStr = HEString(u8"__ProfOvDrawCalls", u8"Draw calls: {0}");
		mGPURenTargetChangesStr = HEString(u8"__ProfOvRTChanges", u8"Render target changes: {0}");
		mGPUPresentsStr = HEString(u8"__ProfOvPresents", u8"Presents: {0}");
		mGPUClearsStr = HEString(u8"__ProfOvClears", u8"Clears: {0}");
		mGPUVerticesStr = HEString(u8"__ProfOvVertices", u8"Num. vertices: {0}");
		mGPUPrimitivesStr = HEString(u8"__ProfOvPrimitives", u8"Num. primitives: {0}");
		mGPUSamplesStr = HEString(u8"__ProfOvSamples", u8"Samples drawn: {0}");
		mGPUPipelineStateChangesStr = HEString(u8"__ProfOvPSChanges", u8"Pipeline state changes: {0}");

		mGPUObjectsCreatedStr = HEString(u8"__ProfOvObjsCreated", u8"Objects created: {0}");
		mGPUObjectsDestroyedStr = HEString(u8"__ProfOvObjsDestroyed", u8"Objects destroyed: {0}");
		mGPUResourceWritesStr = HEString(u8"__ProfOvResWrites", u8"Resource writes: {0}");
		mGPUResourceReadsStr = HEString(u8"__ProfOvResReads", u8"Resource reads: {0}");
		mGPUParamBindsStr = HEString(u8"__ProfOvGpuParamBinds", u8"GPU parameter binds: {0}");
		mGPUVertexBufferBindsStr = HEString(u8"__ProfOvVBBinds", u8"VB binds: {0}");
		mGPUIndexBufferBindsStr = HEString(u8"__ProfOvIBBinds", u8"IB binds: {0}");

		mGPUFrameNumLbl = GUILabel::Create(mGPUFrameNumStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUTimeLbl = GUILabel::Create(mGPUTimeStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUDrawCallsLbl = GUILabel::Create(mGPUDrawCallsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPURenTargetChangesLbl = GUILabel::Create(mGPURenTargetChangesStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUPresentsLbl = GUILabel::Create(mGPUPresentsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUClearsLbl = GUILabel::Create(mGPUClearsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUVerticesLbl = GUILabel::Create(mGPUVerticesStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUPrimitivesLbl = GUILabel::Create(mGPUPrimitivesStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUSamplesLbl = GUILabel::Create(mGPUSamplesStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUPipelineStateChangesLbl = GUILabel::Create(mGPUPipelineStateChangesStr, GUIOptions(GUIOption::FixedWidth(200)));

		mGPUObjectsCreatedLbl = GUILabel::Create(mGPUObjectsCreatedStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUObjectsDestroyedLbl = GUILabel::Create(mGPUObjectsDestroyedStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUResourceWritesLbl = GUILabel::Create(mGPUResourceWritesStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUResourceReadsLbl = GUILabel::Create(mGPUResourceReadsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUParamBindsLbl = GUILabel::Create(mGPUParamBindsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUVertexBufferBindsLbl = GUILabel::Create(mGPUVertexBufferBindsStr, GUIOptions(GUIOption::FixedWidth(200)));
		mGPUIndexBufferBindsLbl = GUILabel::Create(mGPUIndexBufferBindsStr, GUIOptions(GUIOption::FixedWidth(200)));

		mGPULayoutFrameContentsLeft->AddElement(mGPUFrameNumLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUTimeLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUDrawCallsLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPURenTargetChangesLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUPresentsLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUClearsLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUVerticesLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUPrimitivesLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUSamplesLbl);
		mGPULayoutFrameContentsLeft->AddElement(mGPUPipelineStateChangesLbl);
		mGPULayoutFrameContentsLeft->AddNewElement<GUIFlexibleSpace>();

		mGPULayoutFrameContentsRight->AddElement(mGPUObjectsCreatedLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUObjectsDestroyedLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUResourceWritesLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUResourceReadsLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUParamBindsLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUVertexBufferBindsLbl);
		mGPULayoutFrameContentsRight->AddElement(mGPUIndexBufferBindsLbl);
		mGPULayoutFrameContentsRight->AddNewElement<GUIFlexibleSpace>();

		UpdateCpuSampleAreaSizes();
		UpdateGpuSampleAreaSizes();

		if (!mIsShown)
			Hide();
		else
		{
			if (mType == ProfilerOverlayType::CPUSamples)
				Show(ProfilerOverlayType::CPUSamples);
			else
				Show(ProfilerOverlayType::GPUSamples);
		}
	}

	void ProfilerOverlay::Show(ProfilerOverlayType type)
	{
		if (type == ProfilerOverlayType::CPUSamples)
		{
			mBasicLayoutLabels->SetVisible(true);
			mPreciseLayoutLabels->SetVisible(true);
			mBasicLayoutContents->SetVisible(true);
			mPreciseLayoutContents->SetVisible(true);
			mGPULayoutFrameContents->SetVisible(false);
			mGPULayoutSamples->SetVisible(false);
		}
		else
		{
			mGPULayoutFrameContents->SetVisible(true);
			mGPULayoutSamples->SetVisible(true);
			mBasicLayoutLabels->SetVisible(false);
			mPreciseLayoutLabels->SetVisible(false);
			mBasicLayoutContents->SetVisible(false);
			mPreciseLayoutContents->SetVisible(false);
		}

		mType = type;
		mIsShown = true;
	}

	void ProfilerOverlay::Hide()
	{
		mBasicLayoutLabels->SetVisible(false);
		mPreciseLayoutLabels->SetVisible(false);
		mBasicLayoutContents->SetVisible(false);
		mPreciseLayoutContents->SetVisible(false);
		mGPULayoutFrameContents->SetVisible(false);
		mGPULayoutSamples->SetVisible(false);
		mIsShown = false;
	}

	void ProfilerOverlay::Update()
	{
		const ProfilerReport& latestSimReport = ProfilingManager::Instance().GetReport(ProfiledThread::Sim);
		const ProfilerReport& latestCoreReport = ProfilingManager::Instance().GetReport(ProfiledThread::Core);

		UpdateCpuSampleContents(latestSimReport, latestCoreReport);

		while (ProfilerGPU::Instance().GetNumAvailableReports() > 1)
			ProfilerGPU::Instance().GetNextReport(); // Drop any extra reports, we only want the latest

		if (ProfilerGPU::Instance().GetNumAvailableReports() > 0)
		{
			GPUProfilerReport report = ProfilerGPU::Instance().GetNextReport();

			// TODO - Currently displaying just the first view. I need to add a way to toggle between views
			if(!report.viewSamples.empty())
				UpdateGpuSampleContents(report.viewSamples[0]);
		}
	}

	void ProfilerOverlay::TargetResized()
	{
		UpdateCpuSampleAreaSizes();
		UpdateGpuSampleAreaSizes();
	}

	void ProfilerOverlay::UpdateCpuSampleAreaSizes()
	{
		static const INT32 PADDING = 10;
		static const float LABELS_CONTENT_RATIO = 0.3f;

		UINT32 width = (UINT32)std::max(0, (INT32)mTarget->GetPixelArea().width - PADDING * 2);
		UINT32 height = (UINT32)std::max(0, (INT32)(mTarget->GetPixelArea().height - PADDING * 3));

		UINT32 labelsWidth = Math::CeilToInt(width * LABELS_CONTENT_RATIO);
		UINT32 contentWidth = width - labelsWidth;

		mBasicLayoutLabels->SetPosition(PADDING, PADDING);
		mBasicLayoutLabels->SetWidth(labelsWidth);
		mBasicLayoutLabels->SetHeight(height);

		mPreciseLayoutLabels->SetPosition(PADDING, height + PADDING * 2);
		mPreciseLayoutLabels->SetWidth(labelsWidth);
		mPreciseLayoutLabels->SetHeight(height);

		mBasicLayoutContents->SetPosition(PADDING + labelsWidth, PADDING);
		mBasicLayoutContents->SetWidth(contentWidth);
		mBasicLayoutContents->SetHeight(height);

		mPreciseLayoutContents->SetPosition(PADDING + labelsWidth, height + PADDING * 2);
		mPreciseLayoutContents->SetWidth(contentWidth);
		mPreciseLayoutContents->SetHeight(height);
	}

	void ProfilerOverlay::UpdateGpuSampleAreaSizes()
	{
		static const INT32 PADDING = 10;
		static const float SAMPLES_FRAME_RATIO = 0.25f;
		static const INT32 HEADER_HEIGHT = 20;
		static const INT32 NUM_COLUMNS = 3;
		static const INT32 HEIGHT_PER_ENTRY = 15;

		UINT32 width = (UINT32)std::max(0, (INT32)mTarget->GetPixelArea().width - PADDING * 2);
		UINT32 height = (UINT32)std::max(0, (INT32)(mTarget->GetPixelArea().height - PADDING * 3));

		UINT32 frameHeight = Math::CeilToInt(height * SAMPLES_FRAME_RATIO);
		UINT32 samplesHeight = height - frameHeight;

		mGPULayoutFrameContents->SetPosition(PADDING, PADDING);
		mGPULayoutFrameContents->SetWidth(width);
		mGPULayoutFrameContents->SetHeight(frameHeight);

		mGPULayoutSamples->SetPosition(PADDING, PADDING + frameHeight + PADDING);
		mGPULayoutSamples->SetWidth(width);
		mGPULayoutSamples->SetHeight(samplesHeight);

		UINT32 columnWidth = width / NUM_COLUMNS;
		UINT32 columnHeight = samplesHeight - HEADER_HEIGHT;
		for(UINT32 i = 0; i < NUM_COLUMNS; i++)
		{
			mGPULayoutSampleLabels[i]->SetPosition(columnWidth * i, HEADER_HEIGHT);
			mGPULayoutSampleLabels[i]->SetWidth(columnWidth / 2);
			mGPULayoutSampleLabels[i]->SetHeight(columnHeight);

			mGPULayoutSampleContents[i]->SetPosition(columnWidth * i + columnWidth / 2, HEADER_HEIGHT);
			mGPULayoutSampleContents[i]->SetWidth(columnWidth / 2);
			mGPULayoutSampleContents[i]->SetHeight(columnHeight);
		}

		mNumGPUSamplesPerColumn = columnHeight / HEIGHT_PER_ENTRY;
	}

	void ProfilerOverlay::UpdateCpuSampleContents(const ProfilerReport& simReport, const ProfilerReport& coreReport)
	{
		static const UINT32 NUM_ROOT_ENTRIES = 2;

		const CPUProfilerBasicSamplingEntry& simBasicRootEntry = simReport.cpuReport.GetBasicSamplingData();
		const CPUProfilerPreciseSamplingEntry& simPreciseRootEntry = simReport.cpuReport.GetPreciseSamplingData();

		const CPUProfilerBasicSamplingEntry& coreBasicRootEntry = coreReport.cpuReport.GetBasicSamplingData();
		const CPUProfilerPreciseSamplingEntry& corePreciseRootEntry = coreReport.cpuReport.GetPreciseSamplingData();

		struct TodoBasic
		{
			TodoBasic(const CPUProfilerBasicSamplingEntry& _entry, UINT32 _depth)
				:entry(_entry), depth(_depth)
			{ }

			const CPUProfilerBasicSamplingEntry& entry;
			UINT32 depth;
		};

		struct TodoPrecise
		{
			TodoPrecise(const CPUProfilerPreciseSamplingEntry& _entry, UINT32 _depth)
				:entry(_entry), depth(_depth)
			{ }

			const CPUProfilerPreciseSamplingEntry& entry;
			UINT32 depth;
		};

		BasicRowFiller basicRowFiller(mBasicRows, *mBasicLayoutLabels, *mBasicLayoutContents, *mWidget->GetInternalInternal());
		Stack<TodoBasic> todoBasic;

		const CPUProfilerBasicSamplingEntry* basicRootEntries[NUM_ROOT_ENTRIES];
		basicRootEntries[0] = &simBasicRootEntry;
		basicRootEntries[1] = &coreBasicRootEntry;

		for(UINT32 i = 0; i < NUM_ROOT_ENTRIES; i++)
		{
			todoBasic.push(TodoBasic(*basicRootEntries[i], 0));

			while(!todoBasic.empty())
			{
				TodoBasic curEntry = todoBasic.top();
				todoBasic.pop();

				const CPUProfilerBasicSamplingEntry::Data& data = curEntry.entry.data;
				basicRowFiller.AddData(curEntry.depth, data.name, data.pctOfParent, data.numCalls, data.memAllocs, data.memFrees,
					data.avgTimeMs, data.totalTimeMs, data.avgSelfTimeMs, data.totalSelfTimeMs);

				if(curEntry.depth <= MAX_DEPTH)
				{
					for(auto iter = curEntry.entry.childEntries.rbegin(); iter != curEntry.entry.childEntries.rend(); ++iter)
					{
						todoBasic.push(TodoBasic(*iter, curEntry.depth + 1));
					}
				}
			}
		}

		PreciseRowFiller preciseRowFiller(mPreciseRows, *mBasicLayoutLabels, *mBasicLayoutContents, *mWidget->GetInternalInternal());
		Stack<TodoPrecise> todoPrecise;

		const CPUProfilerPreciseSamplingEntry* preciseRootEntries[NUM_ROOT_ENTRIES];
		preciseRootEntries[0] = &simPreciseRootEntry;
		preciseRootEntries[1] = &corePreciseRootEntry;

		for(UINT32 i = 0; i < NUM_ROOT_ENTRIES; i++)
		{
			todoPrecise.push(TodoPrecise(*preciseRootEntries[i], 0));

			while(!todoBasic.empty())
			{
				TodoPrecise curEntry = todoPrecise.top();
				todoPrecise.pop();

				const CPUProfilerPreciseSamplingEntry::Data& data = curEntry.entry.data;
				preciseRowFiller.AddData(curEntry.depth, data.name, data.pctOfParent, data.numCalls, data.memAllocs, data.memFrees,
					data.avgCycles, data.totalCycles, data.avgSelfCycles, data.totalSelfCycles);

				if(curEntry.depth <= MAX_DEPTH)
				{
					for(auto iter = curEntry.entry.childEntries.rbegin(); iter != curEntry.entry.childEntries.rend(); ++iter)
					{
						todoPrecise.push(TodoPrecise(*iter, curEntry.depth + 1));
					}
				}
			}
		}
	}

	void ProfilerOverlay::UpdateGpuSampleContents(const GPUProfileSample& frameSample)
	{
		mGPUFrameNumStr.SetParameter(0, toString((UINT64)gTime().GetFrameIdx()));
		mGPUTimeStr.SetParameter(0, toString(frameSample.timeMs));
		mGPUDrawCallsStr.SetParameter(0, toString(frameSample.numDrawCalls));
		mGPURenTargetChangesStr.SetParameter(0, toString(frameSample.numRenderTargetChanges));
		mGPUPresentsStr.SetParameter(0, toString(frameSample.numPresents));
		mGPUClearsStr.SetParameter(0, toString(frameSample.numClears));
		mGPUVerticesStr.SetParameter(0, toString(frameSample.numVertices));
		mGPUPrimitivesStr.SetParameter(0, toString(frameSample.numPrimitives));
		mGPUSamplesStr.SetParameter(0, toString(frameSample.numDrawnSamples));
		mGPUPipelineStateChangesStr.SetParameter(0, toString(frameSample.numPipelineStateChanges));

		mGPUObjectsCreatedStr.SetParameter(0, toString(frameSample.numObjectsCreated));
		mGPUObjectsDestroyedStr.SetParameter(0, toString(frameSample.numObjectsDestroyed));
		mGPUResourceWritesStr.SetParameter(0, toString(frameSample.numResourceWrites));
		mGPUResourceReadsStr.SetParameter(0, toString(frameSample.numResourceReads));
		mGPUParamBindsStr.SetParameter(0, toString(frameSample.numGpuParamBinds));
		mGPUVertexBufferBindsStr.SetParameter(0, toString(frameSample.numVertexBufferBinds));
		mGPUIndexBufferBindsStr.SetParameter(0, toString(frameSample.numIndexBufferBinds));

		mGPUFrameNumLbl->SetContent(mGPUFrameNumStr);
		mGPUTimeLbl->SetContent(mGPUTimeStr);
		mGPUDrawCallsLbl->SetContent(mGPUDrawCallsStr);
		mGPURenTargetChangesLbl->SetContent(mGPURenTargetChangesStr);
		mGPUPresentsLbl->SetContent(mGPUPresentsStr);
		mGPUClearsLbl->SetContent(mGPUClearsStr);
		mGPUVerticesLbl->SetContent(mGPUVerticesStr);
		mGPUPrimitivesLbl->SetContent(mGPUPrimitivesStr);
		mGPUSamplesLbl->SetContent(mGPUSamplesStr);
		mGPUPipelineStateChangesLbl->SetContent(mGPUPipelineStateChangesStr);

		mGPUObjectsCreatedLbl->SetContent(mGPUObjectsCreatedStr);
		mGPUObjectsDestroyedLbl->SetContent(mGPUObjectsDestroyedStr);
		mGPUResourceWritesLbl->SetContent(mGPUResourceWritesStr);
		mGPUResourceReadsLbl->SetContent(mGPUResourceReadsStr);
		mGPUParamBindsLbl->SetContent(mGPUParamBindsStr);
		mGPUVertexBufferBindsLbl->SetContent(mGPUVertexBufferBindsStr);
		mGPUIndexBufferBindsLbl->SetContent(mGPUIndexBufferBindsStr);

		GPUSampleRowFiller sampleRowFillers[GPU_NUM_SAMPLE_COLUMNS] =
		{
			GPUSampleRowFiller(mGPUSampleRows[0], *mGPULayoutSampleLabels[0], *mGPULayoutSampleContents[0], *mWidget->GetInternalInternal()),
			GPUSampleRowFiller(mGPUSampleRows[1], *mGPULayoutSampleLabels[1], *mGPULayoutSampleContents[1], *mWidget->GetInternalInternal()),
			GPUSampleRowFiller(mGPUSampleRows[2], *mGPULayoutSampleLabels[2], *mGPULayoutSampleContents[2], *mWidget->GetInternalInternal())
		};

		struct Todo
		{
			Todo(const GPUProfileSample& entry, UINT32 depth)
				:entry(entry), depth(depth)
			{ }

			const GPUProfileSample& entry;
			UINT32 depth;
		};

		UINT32 column = 0;
		UINT32 currentCount = 0;

		Stack<Todo> todo;
		todo.push(Todo(frameSample, 0));

		while (!todo.empty())
		{
			Todo curEntry = todo.top();
			todo.pop();

			const GPUProfileSample& data = curEntry.entry;

			if(column < GPU_NUM_SAMPLE_COLUMNS)
				sampleRowFillers[column].AddData(curEntry.depth, data.name, data.timeMs);

			currentCount++;
			if (currentCount % mNumGPUSamplesPerColumn == 0)
				column++;

			if (curEntry.depth <= MAX_DEPTH)
			{
				for (auto iter = curEntry.entry.children.rbegin(); iter != curEntry.entry.children.rend(); ++iter)
					todo.push(Todo(*iter, curEntry.depth + 1));
			}
		}
	}
}
