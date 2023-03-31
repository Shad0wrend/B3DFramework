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

	/** Describes blend states for a single render target. */
	struct B3D_CORE_EXPORT RenderTargetBlendStateInformation
	{
		bool operator==(const RenderTargetBlendStateInformation& rhs) const;

		/**
		 * Queries is blending enabled for the specified render target. Blending allows you to combine the color from
		 * current and previous pixel based on some value.
		 */
		bool BlendEnable = false;

		/**
		 * Determines what should the source blend factor be. This value determines what will the color being generated
		 * currently be multiplied by.
		 */
		BlendFactor ColorSourceFactor = BF_ONE;

		/**
		 * Determines what should the destination blend factor be. This value determines what will the color already in
		 * render target be multiplied by.
		 */
		BlendFactor ColorDestinationFactor = BF_ZERO;

		/**
		 * Determines how are source and destination colors combined (after they are multiplied by their respective blend
		 * factors).
		 */
		BlendOperation ColorBlendOperation = BO_ADD;

		/**
		 * Determines what should the alpha source blend factor be. This value determines what will the alpha value being
		 * generated currently be multiplied by.
		 */
		BlendFactor AlphaSourceFactor = BF_ONE;

		/**
		 * Determines what should the alpha destination blend factor be. This value determines what will the alpha value
		 * already in render target be multiplied by.
		 */
		BlendFactor AlphaDestinationFactor = BF_ZERO;

		/**
		 * Determines how are source and destination alpha values combined (after they are multiplied by their respective
		 * blend factors).
		 */
		BlendOperation AlphaBlendOperation = BO_ADD;

		/**
		 * Render target write mask allows to choose which pixel components should the pixel shader output.
		 *
		 * Only the first four bits are used. First bit representing red, second green, third blue and fourth alpha value.
		 * Set bits means pixel shader will output those channels.
		 */
		u8 RenderTargetWriteMask = 0xFF;
	};

	/** Describes a graphics pipeline blend state. */
	struct B3D_CORE_EXPORT BlendStateInformation
	{
		bool operator==(const BlendStateInformation& rhs) const;

		/**
		 * Alpha to coverage allows you to perform blending without needing to worry about order of rendering like regular
		 * blending does. It requires multi-sampling to be active in order to work, and you need to supply an alpha texture
		 * that determines object transparency.
		 *
		 * Blending is then performed by only using sub-samples covered by the alpha texture for the current pixel and
		 * combining them with sub-samples previously stored.
		 *
		 * Be aware this is a limited technique only useful for certain situations. Unless you are having performance
		 * problems use regular blending.
		 */
		bool EnableAlphaToCoverage = false;

		/**
		 * When not set, only the first render target blend descriptor will be used for all render targets. If set each
		 * render target will use its own blend descriptor.
		 */
		bool EnableIndependantBlend = false;

		RenderTargetBlendStateInformation RenderTargets[B3D_MAXIMUM_RENDER_TARGET_COUNT];

		/**	Generates a hash value from a blend state descriptor. */
		static u64 GenerateHash(const BlendStateInformation& value);
	};

	/** Descriptor structure used for initializing a GPU pipeline state. */
	struct PIPELINE_STATE_DESC
	{
		BlendStateInformation BlendState;
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
			BlendStateInformation BlendState;
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

		BlendStateInformation GetBlendState() const { return mData.BlendState; }
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
