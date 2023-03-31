//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Utility/BsModule.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsSamplerState.h"
#include "RenderAPI/BsGpuPipelineState.h"
#include "RenderAPI/BsGpuPipelineParameterLayout.h"

namespace bs
{
	/** @addtogroup RenderAPI-Internal
	 *  @{
	 */

	/** Handles creation of various render states. */
	class B3D_CORE_EXPORT RenderStateManager : public Module<RenderStateManager>
	{
	public:
		/**
		 * Creates and initializes a new SamplerState.
		 *
		 * @param[in]	desc	Object describing the sampler state to create.
		 */
		SPtr<SamplerState> CreateSamplerState(const SamplerStateCreateInformation& desc) const;

		/** Creates and initializes a new DepthStencilState. */
		SPtr<DepthStencilState> CreateDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) const;

		/** Creates an uninitialized sampler state. Requires manual initialization after creation. */
		SPtr<SamplerState> CreateSamplerStatePtrInternal(const SamplerStateCreateInformation& desc) const;

		/** Creates an uninitialized depth-stencil state. Requires manual initialization after creation. */
		SPtr<DepthStencilState> CreateDepthStencilStatePtrInternal(const DEPTH_STENCIL_STATE_DESC& desc) const;

		/** Gets a sampler state initialized with default options. */
		const SPtr<SamplerState>& GetDefaultSamplerState() const;

		/**	Gets a depth stencil state initialized with default options. */
		const SPtr<DepthStencilState>& GetDefaultDepthStencilState() const;

	private:
		friend class SamplerState;
		friend class DepthStencilState;

		mutable SPtr<SamplerState> mDefaultSamplerState;
		mutable SPtr<DepthStencilState> mDefaultDepthStencilState;
	};

	namespace ct
	{
		/**	Handles creation of various render states. */
		class B3D_CORE_EXPORT RenderStateManager : public Module<RenderStateManager>
		{
		private:
			/** Contains data about a cached depth stencil state. */
			struct CachedDepthStencilState
			{
				CachedDepthStencilState() = default;

				CachedDepthStencilState(u32 id)
					: Id(id)
				{}

				std::weak_ptr<DepthStencilState> State;
				u32 Id = 0;
			};

		public:
			RenderStateManager() = default;

			/**
			 * @copydoc bs::RenderStateManager::CreateSamplerState
			 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
			 */
			SPtr<SamplerState> CreateSamplerState(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT) const;

			/** @copydoc bs::RenderStateManager::CreateDepthStencilState */
			SPtr<DepthStencilState> CreateDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) const;

			/** Creates an uninitialized sampler state. Requires manual initialization after creation. */
			SPtr<SamplerState> CreateSamplerStateInternal(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT) const;

			/** Creates an uninitialized depth-stencil state. Requires manual initialization after creation. */
			SPtr<DepthStencilState> CreateDepthStencilStateInternal(const DEPTH_STENCIL_STATE_DESC& desc) const;

			/** Gets a sampler state initialized with default options. */
			const SPtr<SamplerState>& GetDefaultSamplerState() const;

			/**	Gets a depth stencil state initialized with default options. */
			const SPtr<DepthStencilState>& GetDefaultDepthStencilState() const;

		protected:
			friend class bs::SamplerState;
			friend class bs::DepthStencilState;
			friend class SamplerState;
			friend class DepthStencilState;

			void OnShutDown() override;

			/** @copydoc CreateSamplerState */
			virtual SPtr<SamplerState> CreateSamplerStateInternalInternal(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask) const;

			/** @copydoc CreateDepthStencilState */
			virtual SPtr<DepthStencilState> CreateDepthStencilStateInternalInternal(const DEPTH_STENCIL_STATE_DESC& desc, u32 id) const;

		private:
			/**	Triggered when a new sampler state is created. */
			void NotifySamplerStateCreated(const SamplerStateCreateInformation& desc, const SPtr<SamplerState>& state) const;

			/**	Triggered when a new sampler state is created. */
			void NotifyDepthStencilStateCreated(const DEPTH_STENCIL_STATE_DESC& desc, const CachedDepthStencilState& state) const;

			/**
			 * Triggered when the last reference to a specific sampler state is destroyed, which means we must clear our cached
			 * version as well.
			 */
			void NotifySamplerStateDestroyed(const SamplerStateInformation& desc) const;

			/**
			 * Attempts to find a cached sampler state corresponding to the provided descriptor. Returns null if one doesn't
			 * exist.
			 */
			SPtr<SamplerState> FindCachedState(const SamplerStateInformation& desc) const;

			/**
			 * Attempts to find a cached depth-stencil state corresponding to the provided descriptor. Returns null if one
			 * doesn't exist.
			 */
			SPtr<DepthStencilState> FindCachedState(const DEPTH_STENCIL_STATE_DESC& desc, u32& id) const;

			mutable SPtr<SamplerState> mDefaultSamplerState;
			mutable SPtr<DepthStencilState> mDefaultDepthStencilState;

			mutable UnorderedMap<SamplerStateInformation, std::weak_ptr<SamplerState>> mCachedSamplerStates;
			mutable UnorderedMap<DEPTH_STENCIL_STATE_DESC, CachedDepthStencilState> mCachedDepthStencilStates;

			mutable u32 mNextDepthStencilStateId = 0;

			mutable Mutex mMutex;
		};
	} // namespace ct

	/** @} */
} // namespace bs
