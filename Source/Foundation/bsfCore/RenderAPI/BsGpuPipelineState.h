//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "CoreThread/BsCoreObject.h"

namespace bs
{
	/** @addtogroup RenderAPI
	 *  @{
	 */

	/** Descriptor structure used for initializing a GPU pipeline state. */
	struct PIPELINE_STATE_DESC
	{
		SPtr<BlendState> BlendState;
		SPtr<RasterizerState> RasterizerState;
		SPtr<DepthStencilState> DepthStencilState;

		SPtr<GpuProgram> VertexProgram;
		SPtr<GpuProgram> FragmentProgram;
		SPtr<GpuProgram> GeometryProgram;
		SPtr<GpuProgram> HullProgram;
		SPtr<GpuProgram> DomainProgram;
	};

	/** @} */

	namespace ct
	{
		/** @addtogroup RenderAPI-Internal
		 *  @{
		 */

		/** Descriptor structure describing a GPU graphics pipeline state. */
		struct GpuGraphicsPipelineStateInformation
		{
			SPtr<BlendState> BlendState;
			SPtr<RasterizerState> RasterizerState;
			SPtr<DepthStencilState> DepthStencilState;

			SPtr<GpuProgram> VertexProgram;
			SPtr<GpuProgram> FragmentProgram;
			SPtr<GpuProgram> GeometryProgram;
			SPtr<GpuProgram> HullProgram;
			SPtr<GpuProgram> DomainProgram;
		};

		/** Descriptor structure used for initializing a GPU graphics pipeline state. */
		struct GpuGraphicsPipelineStateCreateInformation : GpuGraphicsPipelineStateInformation
		{
			GpuGraphicsPipelineStateCreateInformation() = default;

			GpuGraphicsPipelineStateCreateInformation(const GpuGraphicsPipelineStateInformation& other)
				: GpuGraphicsPipelineStateInformation(other)
			{}
		};

		/** Descriptor structure describing a GPU compute pipeline state. */
		struct GpuComputePipelineStateInformation
		{
			SPtr<GpuProgram> Program;
		};

		/** Descriptor structure used for initializing a GPU compute pipeline state. */
		struct GpuComputePipelineStateCreateInformation : GpuComputePipelineStateInformation 
		{
			GpuComputePipelineStateCreateInformation() = default;

			GpuComputePipelineStateCreateInformation(const GpuComputePipelineStateInformation& other)
				: GpuComputePipelineStateInformation(other)
			{}
		};

		/** @} */
	} // namespace ct

	/** @addtogroup Implementation
	 *  @{
	 */

	/** Contains all data used by a GPU pipeline state, templated so it may contain both core and sim thread data. */
	template <bool Core>
	struct TGpuPipelineStateTypes
	{};

	template <>
	struct TGpuPipelineStateTypes<false>
	{
		typedef GpuPipelineParameterLayout GpuPipelineParameterLayoutType;
		typedef PIPELINE_STATE_DESC StateDescType;
	};

	template <>
	struct TGpuPipelineStateTypes<true>
	{
		typedef ct::GpuPipelineParameterLayout GpuPipelineParameterLayoutType;
		typedef ct::GpuGraphicsPipelineStateInformation StateDescType;
	};

	/**
	 * Templated version of GraphicsPipelineState so it can be used for both core and non-core versions of the pipeline
	 * state.
	 */
	template <bool Core>
	class B3D_CORE_EXPORT TGraphicsPipelineState
	{
	public:
		using BlendStateType = SPtr<CoreVariantType<BlendState, Core>>;
		using RasterizerStateType = SPtr<CoreVariantType<RasterizerState, Core>>;
		using DepthStencilStateType = SPtr<CoreVariantType<DepthStencilState, Core>>;
		using GpuProgramType = SPtr<CoreVariantType<GpuProgram, Core>>;
		using StateDescType = typename TGpuPipelineStateTypes<Core>::StateDescType;
		using GpuPipelineParameterLayoutType = typename TGpuPipelineStateTypes<Core>::GpuPipelineParameterLayoutType;

		virtual ~TGraphicsPipelineState() = default;

		bool HasVertexProgram() const { return mData.VertexProgram != nullptr; }
		bool HasFragmentProgram() const { return mData.FragmentProgram != nullptr; }
		bool HasGeometryProgram() const { return mData.GeometryProgram != nullptr; }
		bool HasHullProgram() const { return mData.HullProgram != nullptr; }
		bool HasDomainProgram() const { return mData.DomainProgram != nullptr; }

		BlendStateType GetBlendState() const { return mData.BlendState; }
		RasterizerStateType GetRasterizerState() const { return mData.RasterizerState; }
		DepthStencilStateType GetDepthStencilState() const { return mData.DepthStencilState; }

		const GpuProgramType& GetVertexProgram() const { return mData.VertexProgram; }
		const GpuProgramType& GetFragmentProgram() const { return mData.FragmentProgram; }
		const GpuProgramType& GetGeometryProgram() const { return mData.GeometryProgram; }
		const GpuProgramType& GetHullProgram() const { return mData.HullProgram; }
		const GpuProgramType& GetDomainProgram() const { return mData.DomainProgram; }

		/** Returns an object containing the layout of all parameters in all the GPU programs used in this pipeline state. */
		const SPtr<GpuPipelineParameterLayoutType>& GetParameterLayout() const { return mParameterLayout; }

	protected:
		TGraphicsPipelineState() = default;
		TGraphicsPipelineState(const StateDescType& desc);

		StateDescType mData;
		SPtr<GpuPipelineParameterLayoutType> mParameterLayout;
	};

	/**
	 * Templated version of ComputePipelineState so it can be used for both core and non-core versions of the pipeline
	 * state.
	 */
	template <bool Core>
	class B3D_CORE_EXPORT TComputePipelineState
	{
	public:
		using GpuProgramType = SPtr<CoreVariantType<GpuProgram, Core>>;
		using GpuPipelineParameterLayoutType = typename TGpuPipelineStateTypes<Core>::GpuPipelineParameterLayoutType;

		virtual ~TComputePipelineState() = default;

		const GpuProgramType& GetProgram() const { return mProgram; }

		/** Returns an object containing the layout of all parameters in the GPU program used in this pipeline state. */
		const SPtr<GpuPipelineParameterLayoutType>& GetParameterLayout() const { return mParameterLayout; }

	protected:
		TComputePipelineState();
		TComputePipelineState(const GpuProgramType& program);

		GpuProgramType mProgram;
		SPtr<GpuPipelineParameterLayoutType> mParameterLayout;
	};

	/** @} */

	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Describes the state of the GPU pipeline that determines how are primitives rendered. It consists of programmable
	 * states (vertex, fragment, geometry, etc. GPU programs), as well as a set of fixed states (blend, rasterizer,
	 * depth-stencil). Once created the state is immutable, and can be bound to RenderAPI for rendering.
	 */
	class B3D_CORE_EXPORT GpuGraphicsPipelineState : public CoreObject, public TGraphicsPipelineState<false>
	{
	public:
		virtual ~GpuGraphicsPipelineState() = default;

		/**
		 * Retrieves a core implementation of the pipeline object usable only from the core thread.
		 *
		 * @note	Core thread only.
		 */
		SPtr<ct::GpuGraphicsPipelineState> GetCore() const;

		/** @copydoc RenderStateManager::CreateGraphicsPipelineState */
		static SPtr<GpuGraphicsPipelineState> Create(const PIPELINE_STATE_DESC& desc);

	protected:
		GpuGraphicsPipelineState(const PIPELINE_STATE_DESC& desc);

		SPtr<ct::CoreObject> CreateCore() const override;
	};

	/**
	 * Describes the state of the GPU pipeline that determines how are compute programs executed. It consists of
	 * of a single programmable state (GPU program). Once created the state is immutable, and can be bound to RenderAPI for
	 * use.
	 */
	class B3D_CORE_EXPORT GpuComputePipelineState : public CoreObject, public TComputePipelineState<false>
	{
	public:
		virtual ~GpuComputePipelineState() = default;

		/**
		 * Retrieves a core implementation of the pipeline object usable only from the core thread.
		 *
		 * @note	Core thread only.
		 */
		SPtr<ct::GpuComputePipelineState> GetCore() const;

		/** @copydoc RenderStateManager::CreateComputePipelineState */
		static SPtr<GpuComputePipelineState> Create(const SPtr<GpuProgram>& program);

	protected:
		GpuComputePipelineState(const SPtr<GpuProgram>& program);

		SPtr<ct::CoreObject> CreateCore() const override;
	};

	/** @} */

	namespace ct
	{
		/** @addtogroup RenderAPI-Internal
		 *  @{
		 */

		/**
		 * Describes the state of the GPU pipeline that determines how are primitives rendered. It consists of programmable
		 * states (vertex, fragment, geometry, etc. GPU programs), as well as a set of fixed states (blend, rasterizer,
		 * depth-stencil).
		 *
		 * @note	Thread safe (Immutable).
		 */
		class B3D_CORE_EXPORT GpuGraphicsPipelineState : public CoreObject, public TGraphicsPipelineState<true>
		{
		public:
			GpuGraphicsPipelineState(GpuDevice& gpuDevice, const GpuGraphicsPipelineStateCreateInformation& createInformation);
			virtual ~GpuGraphicsPipelineState() = default;

			void Initialize() override;

		protected:
			GpuDevice& mGpuDevice;
		};

		/**
		 * Describes the state of the GPU pipeline that determines how are compute programs executed. It consists of
		 * of a single programmable state (GPU program). 
		 *
		 * @note	Thread safe (Immutable).
		 */
		class B3D_CORE_EXPORT GpuComputePipelineState : public CoreObject, public TComputePipelineState<true>
		{
		public:
			GpuComputePipelineState(GpuDevice& gpuDevice, const GpuComputePipelineStateCreateInformation& createInformation);
			virtual ~GpuComputePipelineState() = default;

			void Initialize() override;

		protected:
			GpuDevice& mGpuDevice;
		};

		/** @} */
	} // namespace ct
} // namespace bs
