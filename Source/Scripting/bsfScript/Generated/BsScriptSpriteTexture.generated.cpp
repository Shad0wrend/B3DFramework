//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptSpriteTexture.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteTexture.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptSpriteSheetGridAnimation.generated.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteTexture.h"

namespace bs
{
	ScriptSpriteTexture::ScriptSpriteTexture(MonoObject* managedInstance, const ResourceHandle<SpriteTexture>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptSpriteTexture::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptSpriteTexture::InternalGetRef);
		metaData.scriptClass->addInternalCall("Internal_setTexture", (void*)&ScriptSpriteTexture::Internal_setTexture);
		metaData.scriptClass->addInternalCall("Internal_getTexture", (void*)&ScriptSpriteTexture::Internal_getTexture);
		metaData.scriptClass->AddInternalCall("Internal_getWidth", (void*)&ScriptSpriteTexture::InternalGetWidth);
		metaData.scriptClass->AddInternalCall("Internal_getHeight", (void*)&ScriptSpriteTexture::Internal_getHeight);
		metaData.scriptClass->addInternalCall("Internal_getFrameWidth", (void*)&ScriptSpriteTexture::Internal_getFrameWidth);
		metaData.scriptClass->addInternalCall("Internal_getFrameHeight", (void*)&ScriptSpriteTexture::Internal_getFrameHeight);
		metaData.scriptClass->addInternalCall("Internal_setOffset", (void*)&ScriptSpriteTexture::Internal_setOffset);
		metaData.scriptClass->addInternalCall("Internal_getOffset", (void*)&ScriptSpriteTexture::Internal_getOffset);
		metaData.scriptClass->addInternalCall("Internal_setScale", (void*)&ScriptSpriteTexture::Internal_setScale);
		metaData.scriptClass->addInternalCall("Internal_getScale", (void*)&ScriptSpriteTexture::Internal_getScale);
		metaData.scriptClass->addInternalCall("Internal_setAnimation", (void*)&ScriptSpriteTexture::Internal_setAnimation);
		metaData.scriptClass->addInternalCall("Internal_getAnimation", (void*)&ScriptSpriteTexture::Internal_getAnimation);
		metaData.scriptClass->addInternalCall("Internal_setAnimationPlayback", (void*)&ScriptSpriteTexture::Internal_setAnimationPlayback);
		metaData.scriptClass->addInternalCall("Internal_getAnimationPlayback", (void*)&ScriptSpriteTexture::Internal_getAnimationPlayback);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptSpriteTexture::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptSpriteTexture::Internal_create0);

	}

	 MonoObject*ScriptSpriteTexture::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->createInstance("bool", ctorParams);
	}
	MonoObject* ScriptSpriteTexture::InternalGetRef(ScriptSpriteTexture* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	void ScriptSpriteTexture::InternalSetTexture(ScriptSpriteTexture* thisPtr, MonoObject* texture)
	{
		ResourceHandle<Texture> tmptexture;
		ScriptRRefBase* scripttexture;
		scripttexture = ScriptRRefBase::toNative(texture);
		if(scripttexture != nullptr)
			tmptexture = static_resource_cast<Texture>(scripttexture->GetHandle());
		thisPtr->GetHandle()->SetTexture(tmptexture);
	}

	MonoObject* ScriptSpriteTexture::InternalGetTexture(ScriptSpriteTexture* thisPtr)
	{
		ResourceHandle<Texture> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetTexture();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	uint32_t ScriptSpriteTexture::InternalGetWidth(ScriptSpriteTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetWidth();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptSpriteTexture::InternalGetHeight(ScriptSpriteTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetHeight();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptSpriteTexture::InternalGetFrameWidth(ScriptSpriteTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFrameWidth();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptSpriteTexture::InternalGetFrameHeight(ScriptSpriteTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFrameHeight();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptSpriteTexture::InternalSetOffset(ScriptSpriteTexture* thisPtr, Vector2* offset)
	{
		thisPtr->GetHandle()->SetOffset(*offset);
	}

	void ScriptSpriteTexture::InternalGetOffset(ScriptSpriteTexture* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetOffset();

		*__output = tmp__output;
	}

	void ScriptSpriteTexture::InternalSetScale(ScriptSpriteTexture* thisPtr, Vector2* scale)
	{
		thisPtr->GetHandle()->SetScale(*scale);
	}

	void ScriptSpriteTexture::InternalGetScale(ScriptSpriteTexture* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetScale();

		*__output = tmp__output;
	}

	void ScriptSpriteTexture::InternalSetAnimation(ScriptSpriteTexture* thisPtr, SpriteSheetGridAnimation* anim)
	{
		thisPtr->GetHandle()->SetAnimation(*anim);
	}

	void ScriptSpriteTexture::InternalGetAnimation(ScriptSpriteTexture* thisPtr, SpriteSheetGridAnimation* __output)
	{
		SpriteSheetGridAnimation tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAnimation();

		*__output = tmp__output;
	}

	void ScriptSpriteTexture::InternalSetAnimationPlayback(ScriptSpriteTexture* thisPtr, SpriteAnimationPlayback playback)
	{
		thisPtr->GetHandle()->SetAnimationPlayback(playback);
	}

	SpriteAnimationPlayback ScriptSpriteTexture::InternalGetAnimationPlayback(ScriptSpriteTexture* thisPtr)
	{
		SpriteAnimationPlayback tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAnimationPlayback();

		SpriteAnimationPlayback __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptSpriteTexture::InternalCreate(MonoObject* managedInstance, MonoObject* texture)
	{
		ResourceHandle<Texture> tmptexture;
		ScriptRRefBase* scripttexture;
		scripttexture = ScriptRRefBase::toNative(texture);
		if(scripttexture != nullptr)
			tmptexture = static_resource_cast<Texture>(scripttexture->GetHandle());
		ResourceHandle<SpriteTexture> instance = SpriteTexture::Create(tmptexture);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	void ScriptSpriteTexture::InternalCreate0(MonoObject* managedInstance, Vector2* uvOffset, Vector2* uvScale, MonoObject* texture)
	{
		ResourceHandle<Texture> tmptexture;
		ScriptRRefBase* scripttexture;
		scripttexture = ScriptRRefBase::toNative(texture);
		if(scripttexture != nullptr)
			tmptexture = static_resource_cast<Texture>(scripttexture->GetHandle());
		ResourceHandle<SpriteTexture> instance = SpriteTexture::Create(*uvOffset, *uvScale, tmptexture);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}
}
