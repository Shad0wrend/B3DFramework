//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsVulkanPrerequisites.h"
#include "RenderAPI/BsRenderWindow.h"

namespace bs
{
	/** @addtogroup Vulkan
	 *  @{
	 */

	/**
	 * Render window implementation for Windows and Vulkan.
	 *
	 * @note	Main thread only.
	 */
	class Win32RenderWindow : public RenderWindow
	{
		using Super = RenderWindow;
	public:
		~Win32RenderWindow();

		void Initialize() override;

		Vector2I ScreenToWindowPosition(const Vector2I& screenPos) const override;
		Vector2I WindowToScreenPosition(const Vector2I& windowPos) const override;
		void Move(i32 left, i32 top) override;
		void Resize(u32 width, u32 height) override;
		void Hide() override;
		void Show() override;
		void Minimize() override;
		void Maximize() override;
		void Restore() override;
		void SetFullscreen(u32 width, u32 height, float refreshRate = 60.0f, u32 monitorIdx = 0) override;
		void SetWindowed(u32 width, u32 height) override;
		void SetVSync(bool enabled, u32 interval = 1) override;

		u64 GetPlatformWindowHandle() const override;

	protected:
		friend class VulkanRenderWindowManager;
		friend class ct::Win32RenderWindow;

		Win32RenderWindow(const RenderWindowCreateInformation& createInformation, u32 windowId, const SPtr<RenderWindow>& parentWindow);
		SPtr<ct::RenderProxy> CreateRenderProxy() const override;

		void DoOnWindowMovedOrResized() override;

	private:
		Win32Window* mWindow = nullptr;
		bool mIsChild = false;
		i32 mDisplayFrequency = 0;
	};

	namespace ct
	{
		class VulkanSurface;

		/**
		 * Render window implementation for Windows and Vulkan.
		 *
		 * @note	Render thread only.
		 */
		class Win32RenderWindow : public RenderWindow
		{
		public:
			Win32RenderWindow(const RenderWindowCreateInformation& createInformation, u32 windowId, u64 hWnd, const SPtr<RenderWindow>& parentWindow);
			~Win32RenderWindow();

			void SwapBuffers(u32 syncMask = 0xFFFFFFFF) override;
			void DoOnSwapChainPropertiesModified() override;

			/** Returns the swap chain owned by the window. */
			VulkanSwapChain* GetSwapChain() const { return mSwapChain; }

			/** Rebuilds the swap chain according to the currently set properties. */
			void RebuildSwapChain();

		protected:
			friend class bs::Win32RenderWindow;

			void Initialize() override;

		protected:
			SPtr<VulkanSurface> mSurface;
			VkColorSpaceKHR mColorSpace;
			VkFormat mColorFormat;
			VkFormat mDepthFormat;
			u32 mPresentQueueFamily;
			VulkanSwapChain* mSwapChain = nullptr;
			u64 mHWnd = 0;
		};
	} // namespace ct

	/** @} */
} // namespace bs
