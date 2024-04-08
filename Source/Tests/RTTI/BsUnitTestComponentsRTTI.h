//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once
#include "RTTI/BsStringRTTI.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	class UnitTestComponentARTTI : public RTTIType<UnitTestComponentA, Component, UnitTestComponentARTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(SceneObjectReference, 0)
			B3D_RTTI_MEMBER(ComponentReference, 1)
			B3D_RTTI_MEMBER(StringValue, 2)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "UnitTestComponentA";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_UnitTestComponentA;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return SceneObject::CreateEmptyComponent<UnitTestComponentA>();
		}
	};

	class UnitTestComponentBRTTI : public RTTIType<UnitTestComponentB, Component, UnitTestComponentBRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(SceneObjectReference, 0)
			B3D_RTTI_MEMBER(StringValue, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "UnitTestComponentB";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_UnitTestComponentB;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return SceneObject::CreateEmptyComponent<UnitTestComponentB>();
		}
	};
} // namespace bs
