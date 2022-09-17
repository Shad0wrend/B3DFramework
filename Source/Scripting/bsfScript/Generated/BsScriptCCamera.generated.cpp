//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCCamera.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCCamera.h"
#include "BsScriptRenderSettings.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptViewport.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptVector2I.h"

namespace bs
{
	ScriptCCamera::ScriptCCamera(MonoObject* managedInstance, const GameObjectHandle<CCamera>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCCamera::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setFlags", (void*)&ScriptCCamera::InternalSetFlags);
		metaData.scriptClass->AddInternalCall("Internal_getFlags", (void*)&ScriptCCamera::InternalGetFlags);
		metaData.scriptClass->AddInternalCall("Internal_getViewport", (void*)&ScriptCCamera::InternalGetViewport);
		metaData.scriptClass->AddInternalCall("Internal_setHorzFOV", (void*)&ScriptCCamera::InternalSetHorzFOV);
		metaData.scriptClass->AddInternalCall("Internal_getHorzFOV", (void*)&ScriptCCamera::InternalGetHorzFOV);
		metaData.scriptClass->AddInternalCall("Internal_setNearClipDistance", (void*)&ScriptCCamera::InternalSetNearClipDistance);
		metaData.scriptClass->AddInternalCall("Internal_getNearClipDistance", (void*)&ScriptCCamera::InternalGetNearClipDistance);
		metaData.scriptClass->AddInternalCall("Internal_setFarClipDistance", (void*)&ScriptCCamera::InternalSetFarClipDistance);
		metaData.scriptClass->AddInternalCall("Internal_getFarClipDistance", (void*)&ScriptCCamera::InternalGetFarClipDistance);
		metaData.scriptClass->AddInternalCall("Internal_setAspectRatio", (void*)&ScriptCCamera::InternalSetAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_getAspectRatio", (void*)&ScriptCCamera::InternalGetAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_getProjectionMatrixRS", (void*)&ScriptCCamera::InternalGetProjectionMatrixRS);
		metaData.scriptClass->AddInternalCall("Internal_getViewMatrix", (void*)&ScriptCCamera::InternalGetViewMatrix);
		metaData.scriptClass->AddInternalCall("Internal_setProjectionType", (void*)&ScriptCCamera::InternalSetProjectionType);
		metaData.scriptClass->AddInternalCall("Internal_getProjectionType", (void*)&ScriptCCamera::InternalGetProjectionType);
		metaData.scriptClass->AddInternalCall("Internal_setOrthoWindowHeight", (void*)&ScriptCCamera::InternalSetOrthoWindowHeight);
		metaData.scriptClass->AddInternalCall("Internal_getOrthoWindowHeight", (void*)&ScriptCCamera::InternalGetOrthoWindowHeight);
		metaData.scriptClass->AddInternalCall("Internal_setOrthoWindowWidth", (void*)&ScriptCCamera::InternalSetOrthoWindowWidth);
		metaData.scriptClass->AddInternalCall("Internal_getOrthoWindowWidth", (void*)&ScriptCCamera::InternalGetOrthoWindowWidth);
		metaData.scriptClass->AddInternalCall("Internal_setPriority", (void*)&ScriptCCamera::InternalSetPriority);
		metaData.scriptClass->AddInternalCall("Internal_getPriority", (void*)&ScriptCCamera::InternalGetPriority);
		metaData.scriptClass->AddInternalCall("Internal_setLayers", (void*)&ScriptCCamera::InternalSetLayers);
		metaData.scriptClass->AddInternalCall("Internal_getLayers", (void*)&ScriptCCamera::InternalGetLayers);
		metaData.scriptClass->AddInternalCall("Internal_setMSAACount", (void*)&ScriptCCamera::InternalSetMSAACount);
		metaData.scriptClass->AddInternalCall("Internal_getMSAACount", (void*)&ScriptCCamera::InternalGetMSAACount);
		metaData.scriptClass->AddInternalCall("Internal_setRenderSettings", (void*)&ScriptCCamera::InternalSetRenderSettings);
		metaData.scriptClass->AddInternalCall("Internal_getRenderSettings", (void*)&ScriptCCamera::InternalGetRenderSettings);
		metaData.scriptClass->AddInternalCall("Internal_notifyNeedsRedraw", (void*)&ScriptCCamera::InternalNotifyNeedsRedraw);
		metaData.scriptClass->AddInternalCall("Internal_worldToScreenPoint", (void*)&ScriptCCamera::InternalWorldToScreenPoint);
		metaData.scriptClass->AddInternalCall("Internal_worldToNdcPoint", (void*)&ScriptCCamera::InternalWorldToNdcPoint);
		metaData.scriptClass->AddInternalCall("Internal_worldToViewPoint", (void*)&ScriptCCamera::InternalWorldToViewPoint);
		metaData.scriptClass->AddInternalCall("Internal_screenToWorldPoint", (void*)&ScriptCCamera::InternalScreenToWorldPoint);
		metaData.scriptClass->AddInternalCall("Internal_screenToViewPoint", (void*)&ScriptCCamera::InternalScreenToViewPoint);
		metaData.scriptClass->AddInternalCall("Internal_screenToNdcPoint", (void*)&ScriptCCamera::InternalScreenToNdcPoint);
		metaData.scriptClass->AddInternalCall("Internal_viewToWorldPoint", (void*)&ScriptCCamera::InternalViewToWorldPoint);
		metaData.scriptClass->AddInternalCall("Internal_viewToScreenPoint", (void*)&ScriptCCamera::InternalViewToScreenPoint);
		metaData.scriptClass->AddInternalCall("Internal_viewToNdcPoint", (void*)&ScriptCCamera::InternalViewToNdcPoint);
		metaData.scriptClass->AddInternalCall("Internal_ndcToWorldPoint", (void*)&ScriptCCamera::InternalNdcToWorldPoint);
		metaData.scriptClass->AddInternalCall("Internal_ndcToViewPoint", (void*)&ScriptCCamera::InternalNdcToViewPoint);
		metaData.scriptClass->AddInternalCall("Internal_ndcToScreenPoint", (void*)&ScriptCCamera::InternalNdcToScreenPoint);
		metaData.scriptClass->AddInternalCall("Internal_screenPointToRay", (void*)&ScriptCCamera::InternalScreenPointToRay);
		metaData.scriptClass->AddInternalCall("Internal_projectPoint", (void*)&ScriptCCamera::InternalProjectPoint);
		metaData.scriptClass->AddInternalCall("Internal_unprojectPoint", (void*)&ScriptCCamera::InternalUnprojectPoint);
		metaData.scriptClass->AddInternalCall("Internal_setMain", (void*)&ScriptCCamera::InternalSetMain);
		metaData.scriptClass->AddInternalCall("Internal_isMain", (void*)&ScriptCCamera::InternalIsMain);

	}

	void ScriptCCamera::InternalSetFlags(ScriptCCamera* thisPtr, CameraFlag flags)
	{
		thisPtr->GetHandle()->SetFlags(flags);
	}

	CameraFlag ScriptCCamera::InternalGetFlags(ScriptCCamera* thisPtr)
	{
		Flags<CameraFlag> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFlags();

		CameraFlag __output;
		__output = (CameraFlag)(uint32_t)tmp__output;

		return __output;
	}

	MonoObject* ScriptCCamera::InternalGetViewport(ScriptCCamera* thisPtr)
	{
		SPtr<Viewport> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetViewport();

		MonoObject* __output;
		__output = ScriptViewport::Create(tmp__output);

		return __output;
	}

	void ScriptCCamera::InternalSetHorzFov(ScriptCCamera* thisPtr, Radian* fovy)
	{
		thisPtr->GetHandle()->SetHorzFOV(*fovy);
	}

	void ScriptCCamera::InternalGetHorzFov(ScriptCCamera* thisPtr, Radian* __output)
	{
		Radian tmp__output;
		tmp__output = thisPtr->GetHandle()->GetHorzFOV();

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalSetNearClipDistance(ScriptCCamera* thisPtr, float nearDist)
	{
		thisPtr->GetHandle()->SetNearClipDistance(nearDist);
	}

	float ScriptCCamera::InternalGetNearClipDistance(ScriptCCamera* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNearClipDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetFarClipDistance(ScriptCCamera* thisPtr, float farDist)
	{
		thisPtr->GetHandle()->SetFarClipDistance(farDist);
	}

	float ScriptCCamera::InternalGetFarClipDistance(ScriptCCamera* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFarClipDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetAspectRatio(ScriptCCamera* thisPtr, float ratio)
	{
		thisPtr->GetHandle()->SetAspectRatio(ratio);
	}

	float ScriptCCamera::InternalGetAspectRatio(ScriptCCamera* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAspectRatio();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalGetProjectionMatrixRs(ScriptCCamera* thisPtr, Matrix4* __output)
	{
		Matrix4 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetProjectionMatrixRS();

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalGetViewMatrix(ScriptCCamera* thisPtr, Matrix4* __output)
	{
		Matrix4 tmp__output;
		tmp__output = thisPtr->GetHandle()->GetViewMatrix();

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalSetProjectionType(ScriptCCamera* thisPtr, ProjectionType pt)
	{
		thisPtr->GetHandle()->SetProjectionType(pt);
	}

	ProjectionType ScriptCCamera::InternalGetProjectionType(ScriptCCamera* thisPtr)
	{
		ProjectionType tmp__output;
		tmp__output = thisPtr->GetHandle()->GetProjectionType();

		ProjectionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetOrthoWindowHeight(ScriptCCamera* thisPtr, float h)
	{
		thisPtr->GetHandle()->SetOrthoWindowHeight(h);
	}

	float ScriptCCamera::InternalGetOrthoWindowHeight(ScriptCCamera* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetOrthoWindowHeight();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetOrthoWindowWidth(ScriptCCamera* thisPtr, float w)
	{
		thisPtr->GetHandle()->SetOrthoWindowWidth(w);
	}

	float ScriptCCamera::InternalGetOrthoWindowWidth(ScriptCCamera* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetOrthoWindowWidth();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetPriority(ScriptCCamera* thisPtr, int32_t priority)
	{
		thisPtr->GetHandle()->SetPriority(priority);
	}

	int32_t ScriptCCamera::InternalGetPriority(ScriptCCamera* thisPtr)
	{
		int32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetPriority();

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetLayers(ScriptCCamera* thisPtr, uint64_t layers)
	{
		thisPtr->GetHandle()->SetLayers(layers);
	}

	uint64_t ScriptCCamera::InternalGetLayers(ScriptCCamera* thisPtr)
	{
		uint64_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLayers();

		uint64_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetMsaaCount(ScriptCCamera* thisPtr, uint32_t count)
	{
		thisPtr->GetHandle()->SetMSAACount(count);
	}

	uint32_t ScriptCCamera::InternalGetMsaaCount(ScriptCCamera* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMSAACount();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCCamera::InternalSetRenderSettings(ScriptCCamera* thisPtr, MonoObject* settings)
	{
		SPtr<RenderSettings> tmpsettings;
		ScriptRenderSettings* scriptsettings;
		scriptsettings = ScriptRenderSettings::toNative(settings);
		if(scriptsettings != nullptr)
			tmpsettings = scriptsettings->GetInternal();
		thisPtr->GetHandle()->SetRenderSettings(tmpsettings);
	}

	MonoObject* ScriptCCamera::InternalGetRenderSettings(ScriptCCamera* thisPtr)
	{
		SPtr<RenderSettings> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetRenderSettings();

		MonoObject* __output;
		__output = ScriptRenderSettings::Create(tmp__output);

		return __output;
	}

	void ScriptCCamera::InternalNotifyNeedsRedraw(ScriptCCamera* thisPtr)
	{
		thisPtr->GetHandle()->notifyNeedsRedraw();
	}

	void ScriptCCamera::InternalWorldToScreenPoint(ScriptCCamera* thisPtr, Vector3* worldPoint, Vector2I* __output)
	{
		Vector2I tmp__output;
		tmp__output = thisPtr->GetHandle()->worldToScreenPoint(*worldPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalWorldToNdcPoint(ScriptCCamera* thisPtr, Vector3* worldPoint, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->worldToNdcPoint(*worldPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalWorldToViewPoint(ScriptCCamera* thisPtr, Vector3* worldPoint, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->worldToViewPoint(*worldPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalScreenToWorldPoint(ScriptCCamera* thisPtr, Vector2I* screenPoint, float depth, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->screenToWorldPoint(*screenPoint, depth);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalScreenToViewPoint(ScriptCCamera* thisPtr, Vector2I* screenPoint, float depth, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->screenToViewPoint(*screenPoint, depth);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalScreenToNdcPoint(ScriptCCamera* thisPtr, Vector2I* screenPoint, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->screenToNdcPoint(*screenPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalViewToWorldPoint(ScriptCCamera* thisPtr, Vector3* viewPoint, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->viewToWorldPoint(*viewPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalViewToScreenPoint(ScriptCCamera* thisPtr, Vector3* viewPoint, Vector2I* __output)
	{
		Vector2I tmp__output;
		tmp__output = thisPtr->GetHandle()->viewToScreenPoint(*viewPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalViewToNdcPoint(ScriptCCamera* thisPtr, Vector3* viewPoint, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetHandle()->viewToNdcPoint(*viewPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalNdcToWorldPoint(ScriptCCamera* thisPtr, Vector2* ndcPoint, float depth, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->ndcToWorldPoint(*ndcPoint, depth);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalNdcToViewPoint(ScriptCCamera* thisPtr, Vector2* ndcPoint, float depth, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->ndcToViewPoint(*ndcPoint, depth);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalNdcToScreenPoint(ScriptCCamera* thisPtr, Vector2* ndcPoint, Vector2I* __output)
	{
		Vector2I tmp__output;
		tmp__output = thisPtr->GetHandle()->ndcToScreenPoint(*ndcPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalScreenPointToRay(ScriptCCamera* thisPtr, Vector2I* screenPoint, Ray* __output)
	{
		Ray tmp__output;
		tmp__output = thisPtr->GetHandle()->screenPointToRay(*screenPoint);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalProjectPoint(ScriptCCamera* thisPtr, Vector3* point, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->projectPoint(*point);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalUnprojectPoint(ScriptCCamera* thisPtr, Vector3* point, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetHandle()->unprojectPoint(*point);

		*__output = tmp__output;
	}

	void ScriptCCamera::InternalSetMain(ScriptCCamera* thisPtr, bool main)
	{
		thisPtr->GetHandle()->SetMain(main);
	}

	bool ScriptCCamera::InternalIsMain(ScriptCCamera* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->isMain();

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
