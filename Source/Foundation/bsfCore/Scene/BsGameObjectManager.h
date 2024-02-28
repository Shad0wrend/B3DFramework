//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Utility/BsModule.h"
#include "Scene/BsGameObject.h"

namespace bs
{
	/** @addtogroup Scene-Internal
	 *  @{
	 */

	/**
	 * Tracks GameObject creation and destructions. Also resolves GameObject references from GameObject handles.
	 *
	 * @note	Main thread only.
	 */
	class B3D_CORE_EXPORT GameObjectManager : public Module<GameObjectManager>
	{
	public:
		GameObjectManager() = default;
		~GameObjectManager();

		/**	Destroys any GameObjects that were queued for destruction. */
		void DestroyQueuedObjects();

		/**	Triggered when a game object is being destroyed. */
		Event<void(const HGameObject&)> OnDestroyed;

	private:
		friend class GameObjectCollection;

		/** Notifies the manager that a new game object collection was created. */
		void RegisterGameObjectCollection(const SPtr<GameObjectCollection>& collection);

		/** Notifies the manager that a game object collection was about to be destroyed. */
		void UnregisterGameObjectCollection(const GameObjectCollection& collection);

		UnorderedMap<UUID, WeakSPtr<GameObjectCollection>> mGameObjectCollections;
	};

	/** @} */
} // namespace bs
