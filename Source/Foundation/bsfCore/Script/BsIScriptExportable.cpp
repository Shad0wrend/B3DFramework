//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Script/BsIScriptExportable.h"

#include "BsIScriptObjectWrapper.h"

using namespace bs;

IScriptExportable::~IScriptExportable()
{
	if(mScriptObjectWrapper != nullptr)
		mScriptObjectWrapper->NotifyNativeObjectDestroyed();
}

void IScriptExportable::AssociateWithScriptObjectWrapper(IScriptObjectWrapper* wrapper)
{
	if(!B3D_ENSURE(mScriptObjectWrapper == nullptr))
		return;

	mScriptObjectWrapper = wrapper;
}

