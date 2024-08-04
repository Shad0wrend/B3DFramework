//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

namespace bs
{
	class IScriptExportable;

	/** @addtogroup Script
	 *  @{
	 */

	/**
	 * Provides interoperability between a native object and a script object. This interface in particular is responsible for tracking their lifetimes:
	 *  - If the script object goes out of scope, the native object might be released if nothing else is referencing it.
	 *	- If the native object goes out of scope, this wrapper will be notified so it may safely fail method calls that try to use it in script.
	 *
	 * It is expected each native class exported to script will implement its own version of the script object wrapper, which would provide
	 * methods to create the script object, and allow the script object to call methods and access data on the native object.
	 */
	class B3D_CORE_EXPORT IScriptObjectWrapper
	{
	public:
		IScriptObjectWrapper(IScriptExportable* nativeObject);
		virtual ~IScriptObjectWrapper();

		/** Returns the native object that is linked to the script object managed by the wrapper. */
		IScriptExportable* GetNativeObject() const { return mNativeObject; }

		// TODO - Script need to be able to own the native object

		// TODO - Don't forget that all code accessing the native object will need to check if its alive and fail gracefully if not
		// TODO - This ownership model might not work at all
		// - What happens if a C++ method creates an object and passes it to C#. The C++ one would immediately go out of scope,
		// and C# calls would all fail.
		// - I would need C# to own the object in that case
		// - Other option it to have C# always own the C++ object (i.e. keep a strong handle) - note this also solve a potential issue with having to hold weak handles in wrapper object
		//  - C# objects could then go out of scope if unreferenced in C#
		//   - For objects with explicit destroy functionality, we would still let them keep strong references, but not e.g. for generic IReflectables
		//   - For other objects I could still keep strong C# handles, but register them in some script object registry
		//    - The registry would then be scanned at regular increments, and if an object has a single C# handle and single C++ handle, free it as its pointing to itself

	private:
		friend class IScriptExportable;

		/** Notifies the wrapper that the native object it is managing is about to be destroyed. */
		virtual void NotifyNativeObjectDestroyed();

		// TODO - Doc
		virtual void NotifyManagedObjectDestroyed() { }

		IScriptExportable* mNativeObject = nullptr;
		bool mOwnsNativeObject = false; /**< If true, native object will be kept alive as long as the script object is alive. */
	};

	// TODO - Implement ScriptObjectWrapper and TScriptObjectWrapper types (for use in Mono)
	// - ScriptObjectWrapper implements IScriptObjectWrapper and copies code from ScriptObjectBase
	// - TScriptObjectWrapper inherits ScriptObjectWrapper and specializes it for a specific type and storage type
	//  - Provides GetOrCreate(NativeObject) method
	//  - Sets up script meta-data and hooks up interops same as normal ScriptObject does
	//  - Also needs to receive a callback when finalizer triggers

	/** @} */
} // namespace bs
