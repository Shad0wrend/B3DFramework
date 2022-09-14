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
	 * @note	Sim thread only.
	 */
	class Win32RenderWindow : public RenderWindow
	{
	public:
		~Win32RenderWindow() = default;

		/** @copydoc RenderWindow::screenToWindowPos */
		void GetCustomAttribute(const String& name, void* pData) const ;

		/** @copydoc RenderWindow::screenToWindowPos */
		Vector2I ScreenToWindowPos(const Vector2I& screenPos) const ;

		/** @copydoc RenderWindow::windowToScreenPos */
		Vector2I WindowToScreenPos(const Vector2I& windowPos) const ;

		/** @copydoc RenderWindow::getCore */
		SPtr<ct::Win32RenderWindow> GetCore() const;

		/**	Retrieves internal window handle. */
		HWND GetHWnd() const;

	protected:
		friend class VulkanRenderWindowManager;
		friend class ct::Win32RenderWindow;

		Win32RenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId);

		/** @copydoc RenderWindow::getProperties */
		const RenderTargetProperties& GetPropertiesInternal() const { return mProperties; }

		/** @copydoc RenderWindow::syncProperties */
		void SyncProperties() ;

		/** @copydoc RenderWindow::createCore */
		SPtr<ct::CoreObject> CreateCore() const ;

	private:
		RenderWindowProperties mProperties;
	};

	namespace ct
	{
	/**
	 * Render window implementation for Windows and Vulkan.
	 *
	 * @note	Core thread only.
	 */
	class Win32RenderWindow : public RenderWindow
	{
	public:
		Win32RenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId, VulkanRenderAPI& renderAPI);
		~Win32RenderWindow();

		/** @copydoc RenderWindow::move */
		void Move(INT32 left, INT32 top) ;

		/** @copydoc RenderWindow::resize */
		void Resize(UINT32 width, UINT32 height) ;

		/** @copydoc RenderWindow::setHidden */
		void SetHidden(bool hidden) ;

		/** @copydoc RenderWindow::setActive */
		void SetActive(bool state) ;

		/** @copydoc RenderWindow::minimize */
		void Minimize() ;

		/** @copydoc RenderWindow::maximize */
		void Maximize() ;

		/** @copydoc RenderWindow::restore */
		void Restore() ;

		/** @copydoc RenderWindow::setFullscreen(UINT32, UINT32, float, UINT32) */
		void SetFullscreen(UINT32 width, UINT32 height, float refreshRate = 60.0f, UINT32 monitorIdx = 0) override;

		/** @copydoc RenderWindow::setFullscreen(const VideoMode&) */
		void SetFullscreen(const VideoMode& videoMode) override;

		/** @copydoc RenderWindow::setWindowed */
		void SetWindowed(UINT32 width, UINT32 height) override;
	
		/** @copydoc RenderWindow::setVSync */
		void SetVSync(bool enabled, UINT32 interval = 1) override;

		/** Prepares the back buffer for rendering. Should be called before it is bound to the GPU. */
		void AcquireBackBuffer();

		/** @copydoc RenderWindow::swapBuffers */
		void SwapBuffers(UINT32 syncMask = 0xFFFFFFFF) override;

		/** @copydoc RenderWindow::getCustomAttribute */
		void GetCustomAttribute(const String& name, void* data) const override;

		/** @copydoc RenderWindow::_windowMovedOrResized */
		void WindowMovedOrResizedInternal() override;

		/**	Returns internal window handle. */
		HWND GetWindowHandleInternal() const;
	protected:
		friend class bs::Win32RenderWindow;

		/** @copydoc CoreObject::initialize */
		void Initialize() override;

		/** @copydoc RenderWindow::getProperties */
		const RenderTargetProperties& GetPropertiesInternal() const override { return mProperties; }

		/** @copydoc RenderWindow::getSyncedProperties */
		RenderWindowProperties& GetSyncedProperties() override { return mSyncedProperties; }

		/** @copydoc RenderWindow::syncProperties */
		void SyncProperties() override;

		/** Rebuilds the swap chain according to the currently set properties. */
		void RebuildSwapChain();

	protected:
		Win32Window* mWindow;
		bool mIsChild;
		bool mShowOnSwap;
		INT32 mDisplayFrequency;

		VulkanRenderAPI& mRenderAPI;
		VkSurfaceKHR mSurface;
		VkColorSpaceKHR mColorSpace;
		VkFormat mColorFormat;
		VkFormat mDepthFormat;
		UINT32 mPresentQueueFamily;
		VulkanSwapChain* mSwapChain = nullptr;
		VulkanSemaphore* mSemaphoresTemp[BS_MAX_UNIQUE_QUEUES + 1]; // +1 for present semaphore
		bool mRequiresNewBackBuffer;

		RenderWindowProperties mProperties;
		RenderWindowProperties mSyncedProperties;
	};	
	}
	
	/** @} */
}
