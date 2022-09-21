//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptComponent.h"
#include "../../../Foundation/bsfCore/Physics/BsCharacterController.h"
#include "Math/BsVector3.h"
#include "../../../Foundation/bsfCore/Physics/BsCharacterController.h"
#include "../../../Foundation/bsfCore/Physics/BsCharacterController.h"
#include "Math/BsRadian.h"

namespace bs { class CCharacterController; }
namespace bs { struct __ControllerColliderCollisionInterop; }
namespace bs { struct __ControllerControllerCollisionInterop; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptCCharacterController : public TScriptComponent<ScriptCCharacterController, CCharacterController>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "CharacterController")

		ScriptCCharacterController(MonoObject* managedInstance, const GameObjectHandle<CCharacterController>& value);

	private:
		void onColliderHit(const ControllerColliderCollision& p0);
		void onControllerHit(const ControllerControllerCollision& p0);

		typedef void(BS_THUNKCALL *onColliderHitThunkDef) (MonoObject*, MonoObject* p0, MonoException**);
		static onColliderHitThunkDef onColliderHitThunk;
		typedef void(BS_THUNKCALL *onControllerHitThunkDef) (MonoObject*, MonoObject* p0, MonoException**);
		static onControllerHitThunkDef onControllerHitThunk;

		static CharacterCollisionFlag InternalMove(ScriptCCharacterController* thisPtr, Vector3* displacement);
		static void InternalGetFootPosition(ScriptCCharacterController* thisPtr, Vector3* __output);
		static void InternalSetFootPosition(ScriptCCharacterController* thisPtr, Vector3* position);
		static float InternalGetRadius(ScriptCCharacterController* thisPtr);
		static void InternalSetRadius(ScriptCCharacterController* thisPtr, float radius);
		static float InternalGetHeight(ScriptCCharacterController* thisPtr);
		static void InternalSetHeight(ScriptCCharacterController* thisPtr, float height);
		static void InternalGetUp(ScriptCCharacterController* thisPtr, Vector3* __output);
		static void InternalSetUp(ScriptCCharacterController* thisPtr, Vector3* up);
		static CharacterClimbingMode InternalGetClimbingMode(ScriptCCharacterController* thisPtr);
		static void InternalSetClimbingMode(ScriptCCharacterController* thisPtr, CharacterClimbingMode mode);
		static CharacterNonWalkableMode InternalGetNonWalkableMode(ScriptCCharacterController* thisPtr);
		static void InternalSetNonWalkableMode(ScriptCCharacterController* thisPtr, CharacterNonWalkableMode mode);
		static float InternalGetMinMoveDistance(ScriptCCharacterController* thisPtr);
		static void InternalSetMinMoveDistance(ScriptCCharacterController* thisPtr, float value);
		static float InternalGetContactOffset(ScriptCCharacterController* thisPtr);
		static void InternalSetContactOffset(ScriptCCharacterController* thisPtr, float value);
		static float InternalGetStepOffset(ScriptCCharacterController* thisPtr);
		static void InternalSetStepOffset(ScriptCCharacterController* thisPtr, float value);
		static void InternalGetSlopeLimit(ScriptCCharacterController* thisPtr, Radian* __output);
		static void InternalSetSlopeLimit(ScriptCCharacterController* thisPtr, Radian* value);
		static uint64_t InternalGetLayer(ScriptCCharacterController* thisPtr);
		static void InternalSetLayer(ScriptCCharacterController* thisPtr, uint64_t layer);
	};
}
