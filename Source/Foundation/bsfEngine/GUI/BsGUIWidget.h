//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Math/BsRect2I.h"
#include "Math/BsVector3.h"
#include "Math/BsQuaternion.h"
#include "Math/BsMatrix4.h"
#include "Utility/BsEvent.h"
#include "2D/BsSpriteMaterial.h"
#include "RenderAPI/BsSubMesh.h"

namespace bs
{
	class GUINavGroup;

	/** @addtogroup Implementation
	 *  @{
	 */

	/**	Data required for rendering a single batch of GUI elements.  */
	struct GUIMeshRenderData
	{
		SubMesh SubMesh;
		SpriteMaterial* Material;
		ct::SpriteMaterialInfo MaterialInformation;

		bool IsLine;
		Rect2I Bounds;
		u32 UniformBufferIndex;
	};

	/** Information about a GUI element that is displaying a render target. */
	struct GUIRenderTargetRenderData
	{
		GUIRenderTargetRenderData() = default;

		GUIRenderTargetRenderData(SPtr<ct::RenderTarget> target, const Rect2I& area)
			: Target(std::move(target)), Area(area)
		{}

		SPtr<ct::RenderTarget> Target;
		u64 LastUpdateCount = (u64)-1;
		Rect2I Area;
	};

	/** Data required for rendering a single batch of GUI elements. */
	struct GUIBatchRenderData
	{
		u32 Id = 0;
		Rect2I Bounds;

		Vector<GUIMeshRenderData> CachedElements;
		Vector<GUIMeshRenderData> NonCachedElements;
		Vector<GUIRenderTargetRenderData> RenderTargetElements;
	};

	/**
	 * Contains data about which draw group needs to be redrawn, as well as a set of new draw groups if
	 * draw groups were updated.
	 */
	struct GUIDrawGroupRenderDataUpdate
	{
		Vector<GUIBatchRenderData> NewBatches;
		Vector<Rect2I> DirtyRegions;
		SPtr<ct::Mesh> TriangleMesh;
		SPtr<ct::Mesh> LineMesh;
	};

	/**
	 * Maintains a set of meshes used for drawing GUI elements. When possible GUI render elements will be merged into the same mesh (i.e. a batch)
	 * in order to reduce render time. Additionally, each batch maintains a list of dirty regions that need to be updated by the GUI renderer.
	 **/
	class B3D_EXPORT GUIMeshBatches
	{
		/** Flags signaling which part of a GUIElement is dirty. */
		enum DirtyFlags
		{
			DirtyMesh = 1 << 0,
			DirtyContent = 1 << 1
		};

	public:
		GUIMeshBatches(GUIWidget* parentWidget);

		/** Iterates over all the render elements in the GUI elements and adds them to suitable batches. */
		void Add(GUIElement* guiElement);

		/** Removes all render elements in the provided GUI element from their current set of batches. */
		void Remove(GUIElement* guiElement);

		/** Rebuilds any dirty internal data and returns the data structure required for updating the GUI renderer. */
		GUIDrawGroupRenderDataUpdate RebuildDirty(bool forceRebuildMeshes);

		/** Notifies the system that element's contents were marked as dirty. */
		void MarkContentDirty(GUIElement* guiElement);

		/** Notifies the system that element's mesh was marked as dirty. */
		void MarkMeshDirty(GUIElement* guiElement);

	private:
		/** Information about a material used by a batch. */
		struct BatchedMaterial
		{
			BatchedMaterial() = default;

			bool CanBeMergedWith(const BatchedMaterial& other) const
			{
				return IsBatchingAllowed && other.IsBatchingAllowed && MaterialHash == other.MaterialHash && MeshType == other.MeshType;
			}

			void Merge(const BatchedMaterial& other)
			{
				B3D_ASSERT(SpriteMaterial != nullptr);
				SpriteMaterial->Merge(SpriteMaterialInformation, other.SpriteMaterialInformation);
			}

			bool IsBatchingAllowed = true;
			u64 MaterialHash = 0;
			GUIMeshType MeshType = GUIMeshType::Triangle;
			SpriteMaterial* SpriteMaterial = nullptr;
			SpriteMaterialInfo SpriteMaterialInformation;
		};

		/** Represents a batched GUI render element. */
		struct BatchedGUIRenderElement
		{
			BatchedGUIRenderElement() = default;

			BatchedGUIRenderElement(GUIElement* element, u32 renderElementIndex, u32 depth)
				: ParentGUIElement(element), RenderElementIndex(renderElementIndex), Depth(depth)
			{}

			GUIElement* ParentGUIElement = nullptr;
			u32 RenderElementIndex = 0;
			u32 Depth = 0;
		};

		/** Represents a batched GUI element. */
		struct BatchedGUIElement
		{
			GUIElement* GUIElement = nullptr;
			SmallVector<u32, 4> BatchPerRenderElement;
			Rect2I Bounds;
		};

		/**
		 * A set of GUI render elements that can be drawn together using a single mesh and material. 
		 * Additionally each batch maintains a list of dirty regions, so the renderer doesn't need to redraw the entire batch when a portion of it changes.
		 */
		struct Batch
		{
			u32 Id = ~0u;
			u32 DepthRangeId = ~0u;

			BatchedMaterial Material;
			Vector<BatchedGUIRenderElement> RenderElements;
			Vector<Rect2I> DirtyRegions;

			// Bounds
			bool IsBoundsDirty = true;
			Rect2I Bounds;

			// Mesh
			u32 IndexOffset = 0;
			u32 IndexCount = 0;
			u32 VertexCount = 0;
			SPtr<Mesh> Mesh;
			bool IsMeshDirty = true;
		};

		/** Contains a set of batches for a specific depth range. */
		struct BatchesInDepthRange
		{
			u32 Id = ~0u;
			Vector<u32> BatchIds;

			// Depth
			u32 DepthRange = 0;
			u32 MinDepth = 0;
		};

		/** Returns a unique batch id. */
		u32 AllocateBatchId();

		/** Frees a batch id allocated with AllocateBatchId(). */
		void FreeBatchId(u32 id);

		/** Splits the provided depth range at the specified depth. Returns the index of second half of the depth range. */
		u32 SplitDepthRange(u32 depthRangeIndex, u32 depth);

		/** Attempts to collapse the provided depth range and the previous depth range into a single depth range. Returns true if the merge was performed. */
		bool CollapseDepthRange(u32 depthRangeIndex);

		/** Rebuilds the GUI element meshes. */
		void RebuildMeshes();

		/**
		 * Adds a specific render element of a GUI element to a batch in the depth range at the provided index. Caller is responsible for
		 * ensuring the element falls within the correct depth range.
		 */
		void Add(BatchedGUIElement& batchedGuiElement, u32 renderElementIndex, u32 depthRangeIndex);

		/** Adds a specific render element of a GUI element to a batch in a suitable depth range. */
		void Add(BatchedGUIElement& batchedGuiElement, u32 renderElementIndex);

		/**
		 * Adds the specified render element of a GUI element to the specified depth range. Based on the provided material information a new batch will be created
		 * in the depth range, or the render element will be appended to an existing batch. Batch it was added to will be returned.
		 */
		Batch* Add(BatchedGUIElement& batchedGuiElement, const BatchedGUIRenderElement& batchedGuiRenderElement, const BatchedMaterial& batchedMaterial, u32 depthRangeIndex);

		/**
		 * Removes a specific render element in the provided GUI element from their batch in the provided depth range.
		 * Caller is responsible for ensuring the provided draw group is contained in the provided depth range.
		 */
		void Remove(BatchedGUIElement& batchedGuiElement, u32 renderElementIndex, u32 depthRangeIndex);

		/** Removes a specific render element in the provided GUI element from their current batch. */
		void Remove(BatchedGUIElement& batchedGuiElement, u32 renderElementIndex);

		/**
		 * Marks region covered by @p element of all the batches associated with the element as dirty, so they
		 * will be redrawn on the next frame. If element is being resized or moved, this should be called on the old
		 * position/size, as well as on the new position/size.
		 */
		void MarkBoundsDirty(const BatchedGUIElement& element);

		/**
		 * Marks region covered by @p element of a particular batch associated with the element as dirty, so it
		 * will be redrawn on the next frame. If element is being resized or moved, this should be called on the old
		 * position/size, as well as on the new position/size.
		 */
		void MarkBoundsDirty(const BatchedGUIElement& element, u32 batchId);

		/** Builds a structure with information required for rendering the provided batch. */
		static GUIBatchRenderData GetRenderData(const Batch& batch);

		/** Calculates the bounds of all elements in all the batches in the provided batch. */
		static Rect2I CalculateBounds(Batch& batche);

		/** Creates information about a material for the provided render element. */
		static BatchedMaterial CreateBatchedMaterial(const BatchedGUIRenderElement& batchedGuiRenderElement);

		/** Creates information about a material for the provided render element. */
		static BatchedMaterial CreateBatchedMaterial(const GUIElement& guiElement, u32 renderElementIndex);

		Vector<BatchesInDepthRange> mDepthRanges;
		UnorderedMap<u32, Batch> mBatches;

		UnorderedMap<GUIElement*, BatchedGUIElement> mElements;
		UnorderedMap<GUIElement*, u32> mDirtyElements;
		Vector<Rect2I> mDirtyRegionsForRemovedBatches;
		bool mBatchesOutOfDateInRenderer = true;
		GUIWidget* mWidget;

		SPtr<Mesh> mTriangleMesh;
		SPtr<Mesh> mLineMesh;
		u32 mNextDepthRangeId = 0;
		u32 mNextBatchId = 0;
		Vector<u32> mFreeBatchIds;
	};

	/** @} */

	/** @addtogroup GUI
	 *  @{
	 */

	/**
	 * A top level container for all types of GUI elements. Every GUI element, layout or area must be assigned to a widget
	 * in order to be rendered.
	 *
	 * Widgets are the only GUI objects that may be arbitrarily transformed, allowing you to create 3D interfaces.
	 */
	class B3D_EXPORT GUIWidget
	{
	public:
		virtual ~GUIWidget();

		/** Sets the skin used for all GUI elements in the widget. This will update the look of all current elements. */
		void SetSkin(const HGUISkin& skin);

		/**	Returns the currently active GUI skin. */
		const GUISkin& GetSkin() const;

		/**	Returns the currently active GUI skin resource. */
		const HGUISkin& GetSkinResource() const { return mSkin; }

		/** Returns the root GUI panel for the widget. */
		GUIPanel* GetPanel() const { return mPanel; }

		/**
		 * Returns the depth to render the widget at. If two widgets overlap the widget with the lower depth will be
		 * rendered in front.
		 */
		u8 GetDepth() const { return mDepth; }

		/**
		 * Changes the depth to render the widget at. If two widgets overlap the widget with the lower depth will be
		 * rendered in front.
		 */
		void SetDepth(u8 depth);

		/**
		 * Checks are the specified coordinates within widget bounds. Coordinates should be relative to the parent window.
		 */
		bool InBounds(const Vector2I& position) const;

		/** Returns bounds of the widget, relative to the parent window. */
		const Rect2I& GetBounds() const { return mBounds; }

		/**
		 * Rebuilds any dirty data required for GUI element rendering and returns the data required for updating the GUI
		 * renderer.
		 */
		GUIDrawGroupRenderDataUpdate RebuildDirtyRenderData();

		/**	Returns the viewport that this widget will be rendered on. */
		Viewport* GetTarget() const;

		/**	Returns the camera this widget is being rendered to. */
		SPtr<Camera> GetCamera() const { return mCamera; }

		/** Changes to which camera does the widget output its contents. */
		void SetCamera(const SPtr<Camera>& camera);

		/**	Returns a list of all elements parented to this widget. */
		const Vector<GUIElement*>& GetElements() const { return mElements; }

		/** Returns the world transform that all GUI elements beloning to this widget will be transformed by. */
		const Matrix4 GetWorldTfrm() const { return mTransform; }

		/**	Checks whether the widget should be rendered or not. */
		bool GetIsActive() const { return mIsActive; }

		/**	Sets whether the widget should be rendered or not. */
		void SetIsActive(bool active);

		/**	Creates a new GUI widget that will be rendered on the provided camera. */
		static SPtr<GUIWidget> Create(const SPtr<Camera>& camera);

		/**	Creates a new GUI widget that will be rendered on the provided camera. */
		static SPtr<GUIWidget> Create(const HCamera& camera);

		/**	Triggered when the widget's viewport size changes. */
		Event<void()> OnOwnerTargetResized;

		/**	Triggered when the parent window gained or lost focus. */
		Event<void()> OnOwnerWindowFocusChanged;

	public: // ***** INTERNAL ******
		/** @name Internal
		 *  @{
		 */

		/** Registers a new element as a child of the widget. */
		void RegisterElementInternal(GUIElementBase* elem);

		/**
		 * Unregisters an element from the widget. Usually called when the element is destroyed, or reparented to another
		 * widget.
		 */
		void UnregisterElementInternal(GUIElementBase* elem);

		/**
		 * Returns the default navigation group assigned to all elements of this widget that don't have an explicit nav-
		 * group. See GUIElement::setNavGroup().
		 */
		SPtr<GUINavGroup> GetDefaultNavGroupInternal() const { return mDefaultNavGroup; }

		/**
		 * Marks the widget mesh dirty requiring a mesh rebuild. Provided element is the one that requested the mesh update.
		 */
		void MarkMeshDirtyInternal(GUIElementBase* elem);

		/**
		 * Marks the elements content as dirty, meaning its internal mesh will need to be rebuilt (this implies the entire
		 * widget mesh will be rebuilt as well).
		 */
		void MarkContentDirtyInternal(GUIElementBase* elem);

		/**	Updates the layout of all child elements, repositioning and resizing them as needed. */
		void UpdateLayoutInternal();

		/**	Updates the layout of the provided element, and queues content updates. */
		void UpdateLayoutInternal(GUIElementBase* elem);

		/**
		 * Updates internal transform values from the specified scene object, in case that scene object's transform changed
		 * since the last call.
		 *
		 * @note	Assumes the same scene object will be provided every time.
		 */
		void UpdateTransformInternal(const HSceneObject& parent);

		/**
		 * Checks if the render target of the destination camera changed, and updates the widget with new information if
		 * it has. Should be called every frame.
		 */
		void UpdateRTInternal();

		/** Destroys the GUI widget and all child GUI elements. This is called automatically when GUIWidget is deleted. */
		void DestroyInternal();

		/** @} */

	protected:
		friend class SceneObject;
		friend class GUIElementBase;
		friend class GUIManager;
		friend class CGUIWidget;

		/**	Constructs a new GUI widget that will be rendered on the provided camera. */
		GUIWidget(const SPtr<Camera>& camera);

		/**	Constructs a new GUI widget that will be rendered on the provided camera. */
		GUIWidget(const HCamera& camera);

		/**	Common code for constructors. */
		void Construct(const SPtr<Camera>& camera);

		/**	Called when the parent window gained or lost focus. */
		virtual void OwnerWindowFocusChanged();

	private:
		struct GUIGroupElement
		{
			GUIGroupElement() = default;

			GUIGroupElement(GUIElement* element, u32 renderElement)
				: Element(element), RenderElement(renderElement)
			{}

			GUIElement* Element = nullptr;
			u32 RenderElement = 0;
		};

		/** Holds information about a set of GUI elements that can be drawn together. */
		struct GUIDrawGroup
		{
			u32 Id = 0;
			u32 DepthRange = 0;
			u32 MinDepth = 0;
			Rect2I Bounds;
			Vector<GUIGroupElement> CachedElements;
			Vector<GUIGroupElement> NonCachedElements;
		};

		/**	Calculates widget bounds using the bounds of all child elements. */
		void UpdateBounds() const;

		/**	Updates the size of the primary GUI panel based on the viewport. */
		void UpdateRootPanel();

		SPtr<Camera> mCamera;
		Vector<GUIElement*> mElements;
		GUIMeshBatches mDrawGroups;
		GUIPanel* mPanel = nullptr;
		u8 mDepth = 128;
		bool mIsActive = true;
		SPtr<GUINavGroup> mDefaultNavGroup;

		Vector3 mPosition = BsZero;
		Quaternion mRotation = BsIdentity;
		Vector3 mScale = Vector3::kOne;
		Matrix4 mTransform = BsIdentity;

		Set<GUIElement*> mDirtyContents;
		Set<GUIElement*> mDirtyContentsTemp;

		mutable u64 mCachedRTId = 0;
		mutable bool mWidgetIsDirty = false;
		mutable Rect2I mBounds;

		HGUISkin mSkin;
	};

	/** @} */
} // namespace bs
