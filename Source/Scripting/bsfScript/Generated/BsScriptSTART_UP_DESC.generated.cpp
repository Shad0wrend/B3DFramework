//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptSTART_UP_DESC.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/RenderAPI/BsRenderWindow.h"
#include "BsScriptRenderWindowCreateInformation.generated.h"

namespace bs
{
#if !B3D_IS_ENGINE
	ScriptStartUpDesc::ScriptStartUpDesc()
	{ }

	MonoObject* ScriptStartUpDesc::Box(const __START_UP_DESCInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__START_UP_DESCInterop ScriptStartUpDesc::Unbox(MonoObject* value)
	{
		return *(__START_UP_DESCInterop*)MonoUtil::Unbox(value);
	}

	START_UP_DESC ScriptStartUpDesc::FromInterop(const __START_UP_DESCInterop& value)
	{
		START_UP_DESC output;
		String tmpRenderApi;
		tmpRenderApi = MonoUtil::MonoToString(value.RenderApi);
		output.RenderApi = tmpRenderApi;
		String tmpRenderer;
		tmpRenderer = MonoUtil::MonoToString(value.Renderer);
		output.Renderer = tmpRenderer;
		String tmpPhysics;
		tmpPhysics = MonoUtil::MonoToString(value.Physics);
		output.Physics = tmpPhysics;
		String tmpAudio;
		tmpAudio = MonoUtil::MonoToString(value.Audio);
		output.Audio = tmpAudio;
		String tmpInput;
		tmpInput = MonoUtil::MonoToString(value.Input);
		output.Input = tmpInput;
		output.PhysicsCooking = value.PhysicsCooking;
		output.AsyncAnimation = value.AsyncAnimation;
		RenderWindowCreateInformation tmpPrimaryWindowDesc;
		tmpPrimaryWindowDesc = ScriptRenderWindowCreateInformation::FromInterop(value.PrimaryWindowDesc);
		output.PrimaryWindowDesc = tmpPrimaryWindowDesc;
		Vector<String> vecImporters;
		if(value.Importers != nullptr)
		{
			ScriptArray scriptArrayImporters(value.Importers);
			vecImporters.resize(scriptArrayImporters.Size());
			for(int elementIndex = 0; elementIndex < (int)scriptArrayImporters.Size(); elementIndex++)
			{
				vecImporters[elementIndex] = scriptArrayImporters.Get<String>(elementIndex);
			}
		}
		output.Importers = vecImporters;

		return output;
	}

	__START_UP_DESCInterop ScriptStartUpDesc::ToInterop(const START_UP_DESC& value)
	{
		__START_UP_DESCInterop output;
		MonoString* tmpRenderApi;
		tmpRenderApi = MonoUtil::StringToMono(value.RenderApi);
		output.RenderApi = tmpRenderApi;
		MonoString* tmpRenderer;
		tmpRenderer = MonoUtil::StringToMono(value.Renderer);
		output.Renderer = tmpRenderer;
		MonoString* tmpPhysics;
		tmpPhysics = MonoUtil::StringToMono(value.Physics);
		output.Physics = tmpPhysics;
		MonoString* tmpAudio;
		tmpAudio = MonoUtil::StringToMono(value.Audio);
		output.Audio = tmpAudio;
		MonoString* tmpInput;
		tmpInput = MonoUtil::StringToMono(value.Input);
		output.Input = tmpInput;
		output.PhysicsCooking = value.PhysicsCooking;
		output.AsyncAnimation = value.AsyncAnimation;
		__RenderWindowCreateInformationInterop tmpPrimaryWindowDesc;
		tmpPrimaryWindowDesc = ScriptRenderWindowCreateInformation::ToInterop(value.PrimaryWindowDesc);
		output.PrimaryWindowDesc = tmpPrimaryWindowDesc;
		int elementCountImporters = (int)value.Importers.size();
		MonoArray* vecImporters;
		ScriptArray scriptArrayImporters = ScriptArray::Create<String>(elementCountImporters);
		for(int elementIndex = 0; elementIndex < elementCountImporters; elementIndex++)
		{
			scriptArrayImporters.Set(elementIndex, value.Importers[elementIndex]);
		}
		vecImporters = scriptArrayImporters.GetInternal();
		output.Importers = vecImporters;

		return output;
	}

#endif
}
