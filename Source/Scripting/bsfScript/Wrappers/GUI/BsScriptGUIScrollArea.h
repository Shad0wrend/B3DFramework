//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "GUI/BsGUIScrollArea.h"

#include "Generated/BsScriptGUIElement.generated.h"
#include "Generated/BsScriptGUIInteractable.generated.h"

namespace bs
{
	struct __TArea2_TUnitValue_int32_t__PhysicalPixel___TUnitValue_int32_t__PhysicalPixel__Interop; 

	/** @addtogroup ScriptInteropEngine
	 *  @{
	 */

	/**	Interop class between C++ & CLR for GUIScrollArea.  */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptGUIScrollArea : public TScriptGUIElementWrapper<GUIScrollArea, ScriptGUIScrollArea, ScriptGUIInteractableWrapperBase>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "GUIScrollArea")

		ScriptGUIScrollArea(GUIScrollArea* nativeObject);

		/** Returns the native object that is being wrapped. */
		GUIScrollArea* GetNativeObject() const { return static_cast<GUIScrollArea*>(mNativeObject); }

		static void SetupScriptBindings();
		static MonoObject* CreateScriptObject(bool construct);
	private:
		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void InternalCreateInstance(MonoObject* instance, ScrollBarType vertBarType, ScrollBarType horzBarType, MonoString* scrollBarStyle, MonoString* scrollAreaStyle, MonoArray* guiOptions);
		static MonoObject* InternalGetLayout(ScriptGUIScrollArea* self);
		static void InternalGetContentBounds(ScriptGUIScrollArea* self, __TArea2_TUnitValue_int32_t__PhysicalPixel___TUnitValue_int32_t__PhysicalPixel__Interop* bounds);
		static float InternalGetHorzScroll(ScriptGUIScrollArea* self);
		static void InternalSetHorzScroll(ScriptGUIScrollArea* self, float value);
		static float InternalGetVertScroll(ScriptGUIScrollArea* self);
		static void InternalSetVertScroll(ScriptGUIScrollArea* self, float value);
		static void InternalGetScrollBarWidth(ScriptGUIScrollArea* self, GUILogicalUnit* width);
	};

	/** @} */
} // namespace bs
