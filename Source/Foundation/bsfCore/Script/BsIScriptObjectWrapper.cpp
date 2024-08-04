//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Script/BsIScriptObjectWrapper.h"
#include "BsCoreApplication.h"

using namespace bs;

IScriptObjectWrapper::IScriptObjectWrapper(IScriptExportable* nativeObject)
	:mNativeObject(nativeObject)
{
	// TODO - Register self with the script object wrapper registry
	// - The registry should can for objects that have both C++ and C# ref count at 1, and allow those objects to be destroyed
	//  - Likely by releasing the managed reference and letting the normal process do the rest
	// - The exceptions would be objects that are explicitly destroyed, which would not require a circular reference - Instead the native object can hold the C# reference, but not vice versa
	//  - When the native destroy destructor triggers, C# object would go out of scope
	//  - Code-gen would need to add checks to ensure access to such object through C# gracefully fails
}

IScriptObjectWrapper::~IScriptObjectWrapper()
{
	// TODO - Remove self from the script object wrapper registry
}

void IScriptObjectWrapper::NotifyNativeObjectDestroyed()
{
	EnsureMainThread();

	mNativeObject = nullptr;
}
