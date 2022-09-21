//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptSTART_UP_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/RenderAPI/BsRenderWindow.h"
#include "BsScriptRENDER_WINDOW_DESC.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptSTART_UP_DESC::ScriptSTART_UP_DESC(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptSTART_UP_DESC::InitRuntimeData()
	{ }

	MonoObject*ScriptSTART_UP_DESC::Box(const __START_UP_DESCInterop& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__START_UP_DESCInterop ScriptSTART_UP_DESC::Unbox(MonoObject* value)
	{
		return *(__START_UP_DESCInterop*)MonoUtil::Unbox(value);
	}

	START_UP_DESC ScriptSTART_UP_DESC::FromInterop(const __START_UP_DESCInterop& value)
	{
		START_UP_DESC output;
		String tmprenderAPI;
		tmprenderAPI = MonoUtil::MonoToString(value.renderAPI);
		output.renderAPI = tmprenderAPI;
		String tmprenderer;
		tmprenderer = MonoUtil::MonoToString(value.renderer);
		output.renderer = tmprenderer;
		String tmpphysics;
		tmpphysics = MonoUtil::MonoToString(value.physics);
		output.physics = tmpphysics;
		String tmpaudio;
		tmpaudio = MonoUtil::MonoToString(value.audio);
		output.audio = tmpaudio;
		String tmpinput;
		tmpinput = MonoUtil::MonoToString(value.input);
		output.input = tmpinput;
		output.physicsCooking = value.physicsCooking;
		output.asyncAnimation = value.asyncAnimation;
		RENDER_WINDOW_DESC tmpprimaryWindowDesc;
		tmpprimaryWindowDesc = ScriptRENDER_WINDOW_DESC::FromInterop(value.primaryWindowDesc);
		output.primaryWindowDesc = tmpprimaryWindowDesc;
		Vector<String> vecimporters;
		if(value.importers != nullptr)
		{
			ScriptArray arrayimporters(value.importers);
			vecimporters.resize(arrayimporters.Size());
			for(int i = 0; i < (int)arrayimporters.Size(); i++)
			{
				vecimporters[i] = arrayimporters.Get<String>(i);
			}
		}
		output.importers = vecimporters;

		return output;
	}

	__START_UP_DESCInterop ScriptSTART_UP_DESC::ToInterop(const START_UP_DESC& value)
	{
		__START_UP_DESCInterop output;
		MonoString* tmprenderAPI;
		tmprenderAPI = MonoUtil::StringToMono(value.renderAPI);
		output.renderAPI = tmprenderAPI;
		MonoString* tmprenderer;
		tmprenderer = MonoUtil::StringToMono(value.renderer);
		output.renderer = tmprenderer;
		MonoString* tmpphysics;
		tmpphysics = MonoUtil::StringToMono(value.physics);
		output.physics = tmpphysics;
		MonoString* tmpaudio;
		tmpaudio = MonoUtil::StringToMono(value.audio);
		output.audio = tmpaudio;
		MonoString* tmpinput;
		tmpinput = MonoUtil::StringToMono(value.input);
		output.input = tmpinput;
		output.physicsCooking = value.physicsCooking;
		output.asyncAnimation = value.asyncAnimation;
		__RENDER_WINDOW_DESCInterop tmpprimaryWindowDesc;
		tmpprimaryWindowDesc = ScriptRENDER_WINDOW_DESC::ToInterop(value.primaryWindowDesc);
		output.primaryWindowDesc = tmpprimaryWindowDesc;
		int arraySizeimporters = (int)value.importers.size();
		MonoArray* vecimporters;
		ScriptArray arrayimporters = ScriptArray::Create<String>(arraySizeimporters);
		for(int i = 0; i < arraySizeimporters; i++)
		{
			arrayimporters.Set(i, value.importers[i]);
		}
		vecimporters = arrayimporters.GetInternal();
		output.importers = vecimporters;

		return output;
	}

#endif
}
