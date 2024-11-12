//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Utility/BsModule.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Utility/BsEvent.h"

namespace bs
{
	/** @addtogroup RenderAPI-Internal
	 *  @{
	 */

	/** Handles creation and internal updates relating to render windows. */
	class B3D_CORE_EXPORT RenderWindowManager : public Module<RenderWindowManager>
	{
	public:
		RenderWindowManager() = default;
		~RenderWindowManager() = default;

		/** Creates a new render window using the specified options. Optionally makes the created window a child of another window. */
		SPtr<RenderWindow> Create(const RenderWindowCreateInformation& createInformation, const SPtr<RenderWindow>& parentWindow);

		/** Called once per frame. Dispatches events. */
		void Update();

		/** Called by the main thread when window is destroyed. */
		void NotifyWindowDestroyed(RenderWindow& window);

		/**	Called by the main thread when window receives focus. */
		void NotifyFocusReceived(RenderWindow& window);

		/**	Called by the main thread when window loses focus. */
		void NotifyFocusLost(RenderWindow& window);

		/**	Called by the main thread when mouse leaves a window. */
		void NotifyMouseLeft(RenderWindow& window);

		/**	Returns a list of all open render windows. */
		Vector<RenderWindow*> GetRenderWindows() const;

		/** Returns the window that is currently the top-most modal window. Returns null if no modal windows are active. */
		RenderWindow* GetTopMostModal() const;

		/** Event that is triggered when a window gains focus. */
		Event<void(RenderWindow&)> OnFocusGained;

		/**	Event that is triggered when a window loses focus. */
		Event<void(RenderWindow&)> OnFocusLost;

		/**	Event that is triggered when mouse leaves a window. */
		Event<void(RenderWindow&)> OnMouseLeftWindow;

	protected:
		friend class RenderWindow;

		/**	Finds a main thread equivalent of the provided render proxy. */
		RenderWindow* GetRenderProxyObject(const ct::RenderWindow* window) const;

		/** @copydoc Create */
		virtual SPtr<RenderWindow> CreateImplementation(const RenderWindowCreateInformation& createInformation, u32 windowId, const SPtr<RenderWindow>& parentWindow) = 0;

	protected:
		Map<u32, RenderWindow*> mWindows;
		Vector<RenderWindow*> mModalWindowStack;

		RenderWindow* mWindowInFocus = nullptr;

		u32 mNextWindowId = 0;
	};

	/** @} */
} // namespace bs
