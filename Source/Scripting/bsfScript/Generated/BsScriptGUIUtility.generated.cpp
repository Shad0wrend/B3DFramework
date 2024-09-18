//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIUtility.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIUtility.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "BsScriptTVector2I.generated.h"

namespace bs
{
	ScriptGUIUtility::ScriptGUIUtility(MonoObject* managedInstance, const SPtr<GUIUtility>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptGUIUtility::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_CalculateOptimalSize", (void*)&ScriptGUIUtility::InternalCalculateOptimalSize);
		metaData.ScriptClass->AddInternalCall("Internal_CalculateTextBounds", (void*)&ScriptGUIUtility::InternalCalculateTextBounds);

	}

	MonoObject* ScriptGUIUtility::Create(const SPtr<GUIUtility>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (B3DAllocate<ScriptGUIUtility>()) ScriptGUIUtility(managedInstance, value);
		return managedInstance;
	}
	void ScriptGUIUtility::InternalCalculateOptimalSize(MonoObject* elem, TVector2I<int32_t>* __output)
	{
		GUIElement* tmpelem;
		ScriptGUIElement* scriptObjectWrapperelem;
		scriptObjectWrapperelem = ScriptGUIElement::GetScriptObjectWrapper(elem);
		if(scriptObjectWrapperelem != nullptr)
			tmpelem = static_cast<GUIElement*>(scriptObjectWrapperelem->GetNativeObject());
		TVector2I<int32_t> tmp__output;
		tmp__output = GUIUtility::CalculateOptimalSize(tmpelem);

		*__output = tmp__output;
	}

	void ScriptGUIUtility::InternalCalculateTextBounds(MonoString* text, MonoObject* font, float fontSize, TVector2I<int32_t>* __output)
	{
		String tmptext;
		tmptext = MonoUtil::MonoToString(text);
		TResourceHandle<Font> tmpfont;
		ScriptRRefBase* scriptObjectWrapperfont;
		scriptObjectWrapperfont = ScriptRRefBase::GetScriptObjectWrapper(font);
		if(scriptObjectWrapperfont != nullptr)
			tmpfont = B3DStaticResourceCast<Font>(scriptObjectWrapperfont->GetNativeObject());
		TVector2I<int32_t> tmp__output;
		tmp__output = GUIUtility::CalculateTextBounds(tmptext, tmpfont, fontSize);

		*__output = tmp__output;
	}
}
