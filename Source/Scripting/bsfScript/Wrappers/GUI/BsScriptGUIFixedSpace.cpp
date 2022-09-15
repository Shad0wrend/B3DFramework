//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIFixedSpace.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUISpace.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

namespace bs
{
	ScriptGUIFixedSpace::ScriptGUIFixedSpace(MonoObject* instance, GUIFixedSpace* fixedSpace)
		:TScriptGUIElementBase(instance, fixedSpace), mFixedSpace(fixedSpace), mIsDestroyed(false)
	{

	}

	void ScriptGUIFixedSpace::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIFixedSpace::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_SetSize", (void*)&ScriptGUIFixedSpace::InternalSetSize);
	}

	void ScriptGUIFixedSpace::Destroy()
	{
		if (!mIsDestroyed)
		{
			if (mParent != nullptr)
				mParent->RemoveChild(this);

			GUIFixedSpace::Destroy(mFixedSpace);

			mIsDestroyed = true;
		}
	}

	void ScriptGUIFixedSpace::InternalCreateInstance(MonoObject* instance, UINT32 size)
	{
		GUIFixedSpace* space = GUIFixedSpace::Create(size);

		new (bs_alloc<ScriptGUIFixedSpace>()) ScriptGUIFixedSpace(instance, space);
	}

	void ScriptGUIFixedSpace::InternalSetSize(ScriptGUIFixedSpace* nativeInstance, UINT32 size)
	{
		nativeInstance->mFixedSpace->SetSize(size);
	}
}
