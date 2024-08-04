//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

namespace bs
{
	/** @addtogroup Script
	 *  @{
	 */

	class IScriptObjectWrapper;

	// TODO - Doc
	class B3D_CORE_EXPORT IScriptExportable
	{
	public:
		virtual ~IScriptExportable();

		// TODO - Doc
		IScriptObjectWrapper* GetScriptObjectWrapper() const { return mScriptObjectWrapper; }

	private:
		friend class IScriptObjectWrapper;

		/** Notifies the object that a script object wrapper has been created for it, allowing a script object to access the native object through it. */
		void AssociateWithScriptObjectWrapper(IScriptObjectWrapper* wrapper);

		IScriptObjectWrapper* mScriptObjectWrapper = nullptr;
	};

	/** @} */
} // namespace bs
