//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsVulkanPrerequisites.h"
#include "RenderAPI/BsRenderWindow.h"
#include <X11/extensions/Xrandr.h>

namespace bs
{
	class LinuxWindow;
	class LinuxRenderWindow;

	namespace ct
	{
		class LinuxRenderWindow;
	}

	/** @addtogroup Vulkan
	 *  @{
	 */

	/**
	 * Render window implementation for Linux.
	 *
	 * @note	Sim thread only.
	 */
	class LinuxRenderWindow : public RenderWindow
	{
	public:
		~LinuxRenderWindow() { }

		/** @copydoc RenderWindow::getCustomAttribute */
		void GetCustomAttribute(const String& name, void* pData) const override;

		/** @copydoc RenderWindow::screenToWindowPos */
		Vector2I ScreenToWindowPos(const Vector2I& screenPos) const override;

		/** @copydoc RenderWindow::windowToScreenPos */
		Vector2I WindowToScreenPos(const Vector2I& windowPos) const override;

		/** @copydoc RenderWindow::getCore */
		SPtr<ct::LinuxRenderWindow> GetCore() const;

	protected:
		friend class VulkanRenderWindowManager;
		friend class ct::LinuxRenderWindow;

		LinuxRenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId);

		/** @copydoc RenderWindow::getProperties */
		const RenderTargetProperties& GetPropertiesInternal() const override { return mProperties; }

		/** @copydoc RenderWindow::syncProperties */
		void SyncProperties() override;

		/** @copydoc RenderWindow::createCore */
		SPtr<ct::CoreObject> CreateCore() const override;

	private:
		RenderWindowProperties mProperties;
	};

	namespace ct
	{
		/**
		 * Render window implementation for Linux.
		 *
		 * @note	Core thread only.
		 */
		class LinuxRenderWindow : public RenderWindow
		{
		public:
			LinuxRenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId, VulkanRenderAPI& renderAPI);
			~LinuxRenderWindow();

			/** @copydoc RenderWindow::setFullscreen(UINT32, UINT32, float, UINT32) */
			void SetFullscreen(UINT32 width, UINT32 height, float refreshRate = 60.0f, UINT32 monitorIdx = 0) override;

			/** @copydoc RenderWindow::setFullscreen(const VideoMode&) */
			void SetFullscreen(const VideoMode& videoMode) override;

			/** @copydoc RenderWindow::setWindowed */
			void SetWindowed(UINT32 width, UINT32 height) override;

			/** @copydoc RenderWindow::setHidden */
			void SetHidden(bool hidden) override;

			/** @copydoc RenderWindow::minimize */
			void Minimize() override;

			/** @copydoc RenderWindow::maximize */
			void Maximize() override;

			/** @copydoc RenderWindow::restore */
			void Restore() override;

			/** @copydoc RenderWindow::move */
			void Move(INT32 left, INT32 top) override;

			/** @copydoc RenderWindow::resize */
			void Resize(UINT32 width, UINT32 height) override;

			/** @copydoc RenderWindow::setVSync */
			void SetVSync(bool enabled, UINT32 interval = 1) override;

			/**
			 * Copies the contents of a frame buffer into the pre-allocated buffer.
			 *
			 * @param[out]	dst		Previously allocated buffer to read the contents into. Must be of valid size.
			 * @param[in]	buffer	Frame buffer to read the contents from.
			 */
			void CopyToMemory(PixelData& dst, FrameBuffer buffer);

			/** Prepares the back buffer for rendering. Should be called before it is bound to the GPU. */
			void AcquireBackBuffer();

			/** @copydoc RenderWindow::swapBuffers */
			void SwapBuffers(UINT32 syncMask) override;

			/** @copydoc RenderWindow::getCustomAttribute */
			void GetCustomAttribute(const String& name, void* pData) const override;

			/** @copydoc RenderWindow::setActive */
			void SetActive(bool state) override;

			/** @copydoc RenderWindow::_windowMovedOrResized */
			void _windowMovedOrResized() override;

			/** Returns a lock that can be used for accessing synced properties. */
			SpinLock& _getPropertiesLock() { return mLock;}

			/** Returns the internal X11 window that this object wraps. */
			LinuxWindow* _getInternal() const { return mWindow; }

		protected:
			/** Changes the video mode to the specified RandR mode on the specified output device. */
			void SetVideoMode(INT32 screen, RROutput output, RRMode mode);

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
			friend class bs::LinuxRenderWindow;

			VulkanRenderAPI& mRenderAPI;
			VkSurfaceKHR mSurface;
			VkColorSpaceKHR mColorSpace;
			VkFormat mColorFormat;
			VkFormat mDepthFormat;
			UINT32 mPresentQueueFamily;
			VulkanSwapChain* mSwapChain;
			VulkanSemaphore* mSemaphoresTemp[BS_MAX_UNIQUE_QUEUES + 1]; // +1 for present semaphore
			bool mRequiresNewBackBuffer;

			LinuxWindow* mWindow;
			RenderWindowProperties mProperties;
			RenderWindowProperties mSyncedProperties;
			bool mIsChild;
			bool mShowOnSwap;
		};
	}

	/** @} */
}

