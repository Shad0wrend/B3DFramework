//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Testing/BsConsoleTestOutput.h"
#include "Testing/BsTestSuite.h"
#include "Animation/BsAnimationCurve.h"
#include "Particles/BsParticleDistribution.h"
#include "RTTI/BsStringRTTI.h"
#include "Serialization/BsBinarySerializer.h"
#include "FileSystem/BsDataStream.h"

using namespace bs;

struct UnitTestSerializationObjectB : IReflectable
{
	u32 IntA = 100;
	String StrA = "100";

	bool operator==(const UnitTestSerializationObjectB& other) const
	{
		return IntA == other.IntA && StrA == other.StrA;
	}

	/************************************************************************/
	/* 								RTTI		                     		*/
	/************************************************************************/
public:
	friend class SerializationTestObjectBRTTI;
	static RTTITypeBase* GetRttiStatic();
	RTTITypeBase* GetRtti() const override;
};

struct UnitTestSerializationObjectA : IReflectable
{
	UnitTestSerializationObjectA()
	{
		ArrStrA = { "10", "11", "12" };
		ArrStrB = { "13", "14", "15" };
		ArrStrC = { "16", "17", "18" };

		ArrObjA = { UnitTestSerializationObjectB(), UnitTestSerializationObjectB(), UnitTestSerializationObjectB() };
		ArrObjB = { UnitTestSerializationObjectB(), UnitTestSerializationObjectB(), UnitTestSerializationObjectB() };

		ArrObjPtrA = { B3DMakeShared<UnitTestSerializationObjectB>(), B3DMakeShared<UnitTestSerializationObjectB>(), B3DMakeShared<UnitTestSerializationObjectB>() };
		ArrObjPtrB = { B3DMakeShared<UnitTestSerializationObjectB>(), B3DMakeShared<UnitTestSerializationObjectB>(), B3DMakeShared<UnitTestSerializationObjectB>() };
	}

	u32 IntA = 5;
	String StrA = "5";
	String StrB = "7";

	UnitTestSerializationObjectB ObjA;
	UnitTestSerializationObjectB ObjB;

	SPtr<UnitTestSerializationObjectB> ObjPtrA = B3DMakeShared<UnitTestSerializationObjectB>();
	SPtr<UnitTestSerializationObjectB> ObjPtrB = B3DMakeShared<UnitTestSerializationObjectB>();
	SPtr<UnitTestSerializationObjectB> ObjPtrC = B3DMakeShared<UnitTestSerializationObjectB>();
	SPtr<UnitTestSerializationObjectB> ObjPtrD = nullptr;
	SPtr<UnitTestSerializationObjectB> ObjPtrE = nullptr;

	Vector<String> ArrStrA;
	Vector<String> ArrStrB;
	Vector<String> ArrStrC;

	Vector<UnitTestSerializationObjectB> ArrObjA;
	Vector<UnitTestSerializationObjectB> ArrObjB;

	Vector<SPtr<UnitTestSerializationObjectB>> ArrObjPtrA;
	Vector<SPtr<UnitTestSerializationObjectB>> ArrObjPtrB;

	UnorderedMap<u32, String> PlainMapA = { { 5, "value5" }, { 10, "value10" }, { 15, "value15" } };
	UnorderedMap<u32, String> PlainMapB = { { 5, "value5" }, { 10, "value10" }, { 15, "value15" } };
	UnorderedMap<u32, String> PlainMapC = { { 5, "value5" }, { 10, "value10" }, { 15, "value15" } };
	UnorderedMap<u32, String> PlainMapD = { { 5, "value5" }, { 10, "value10" }, { 15, "value15" } };
	UnorderedMap<u32, String> PlainMapE = { { 5, "value5" }, { 10, "value10" }, { 15, "value15" } };

	UnorderedMap<String, UnitTestSerializationObjectB> ObjectMapA = {{ "a", UnitTestSerializationObjectB() }, { "b", UnitTestSerializationObjectB() }};
	UnorderedMap<String, UnitTestSerializationObjectB> ObjectMapB = {{ "a", UnitTestSerializationObjectB() }, { "b", UnitTestSerializationObjectB() }};
	UnorderedMap<String, UnitTestSerializationObjectB> ObjectMapC = { { "a", UnitTestSerializationObjectB() }, { "b", UnitTestSerializationObjectB() }};

	UnorderedMap<String, SPtr<UnitTestSerializationObjectB>> ObjectPointerMap = { { "a", B3DMakeShared<UnitTestSerializationObjectB>() },
																				  { "b", B3DMakeShared<UnitTestSerializationObjectB>() },
																				  { "c", B3DMakeShared<UnitTestSerializationObjectB>() },
																				  { "d", B3DMakeShared<UnitTestSerializationObjectB>() },
																				  { "e", nullptr } };

	/************************************************************************/
	/* 								RTTI		                     		*/
	/************************************************************************/
public:
	friend class UnitTestSerializationObjectARTTI;
	static RTTITypeBase* GetRttiStatic();
	RTTITypeBase* GetRtti() const override;
};

class UnitTestSerializationObjectARTTI : public RTTIType<UnitTestSerializationObjectA, IReflectable, UnitTestSerializationObjectARTTI>
{
private:
	B3D_RTTI_BEGIN_MEMBERS
		B3D_RTTI_MEMBER(IntA, 0)
		B3D_RTTI_MEMBER(StrA, 1)
		B3D_RTTI_MEMBER(StrB, 2)

		B3D_RTTI_MEMBER(ObjA, 3)
		B3D_RTTI_MEMBER(ObjB, 4)

		B3D_RTTI_MEMBER(ObjPtrA, 5)
		B3D_RTTI_MEMBER(ObjPtrB, 6)
		B3D_RTTI_MEMBER(ObjPtrC, 7)
		B3D_RTTI_MEMBER(ObjPtrD, 8)
		B3D_RTTI_MEMBER(ObjPtrE, 9)

		B3D_RTTI_MEMBER_CONTAINER(ArrStrA, 10)
		B3D_RTTI_MEMBER_CONTAINER(ArrStrB, 11)
		B3D_RTTI_MEMBER_CONTAINER(ArrStrC, 12)

		B3D_RTTI_MEMBER_CONTAINER(ArrObjA, 13)
		B3D_RTTI_MEMBER_CONTAINER(ArrObjB, 14)

		B3D_RTTI_MEMBER_CONTAINER(ArrObjPtrA, 15)
		B3D_RTTI_MEMBER_CONTAINER(ArrObjPtrB, 16)

		B3D_RTTI_MEMBER_CONTAINER(PlainMapA, 17)
		B3D_RTTI_MEMBER_CONTAINER(PlainMapB, 18)
		B3D_RTTI_MEMBER_CONTAINER(PlainMapC, 19)
		B3D_RTTI_MEMBER_CONTAINER(PlainMapD, 20)
		B3D_RTTI_MEMBER_CONTAINER(PlainMapE, 21)

		B3D_RTTI_MEMBER_CONTAINER(ObjectMapA, 22)
		B3D_RTTI_MEMBER_CONTAINER(ObjectMapB, 23)
		B3D_RTTI_MEMBER_CONTAINER(ObjectMapC, 24)

		B3D_RTTI_MEMBER_CONTAINER(ObjectPointerMap, 25)

	B3D_RTTI_END_MEMBERS

public:
	const String& GetRttiName() override
	{
		static String name = "UnitTestSerializationObjectA";
		return name;
	}

	u32 GetRttiId() const override
	{
		return TID_UnitTestSerializationObjectA;
	}

	SPtr<IReflectable> NewRttiObject() override
	{
		return B3DMakeShared<UnitTestSerializationObjectA>();
	}
};

class UnitTestSerializationObjectBRTTI : public RTTIType<UnitTestSerializationObjectB, IReflectable, UnitTestSerializationObjectBRTTI>
{
private:
	B3D_RTTI_BEGIN_MEMBERS
		B3D_RTTI_MEMBER(IntA, 0)
		B3D_RTTI_MEMBER(StrA, 1)
	B3D_RTTI_END_MEMBERS

public:
	const String& GetRttiName() override
	{
		static String name = "UnitTestSerializationObjectB";
		return name;
	}

	u32 GetRttiId() const override
	{
		return TID_UnitTestSerializationObjectB;
	}

	SPtr<IReflectable> NewRttiObject() override
	{
		return B3DMakeShared<UnitTestSerializationObjectB>();
	}
};

RTTITypeBase* UnitTestSerializationObjectB::GetRttiStatic()
{
	return UnitTestSerializationObjectBRTTI::Instance();
}

RTTITypeBase* UnitTestSerializationObjectB::GetRtti() const
{
	return GetRttiStatic();
}

RTTITypeBase* UnitTestSerializationObjectA::GetRttiStatic()
{
	return UnitTestSerializationObjectARTTI::Instance();
}

RTTITypeBase* UnitTestSerializationObjectA::GetRtti() const
{
	return GetRttiStatic();
}

static float EvaluatePosition(float acceleration, float velocity, float time)
{
	return acceleration * time * time * 0.5f + velocity * time;
}

static float EvaluateVelocity(float acceleration, float time)
{
	return acceleration * time;
}

class CoreTestSuite : public TestSuite
{
public:
	CoreTestSuite();

private:
	void TestAnimCurveIntegration();
	void TestLookupTable();
	void TestBinarySerialization();
	void TestSerializedObject();
	void TestBinaryDelta();

	template<typename T>
	void TestAssertArraysMatch(const T& lhs, const T& rhs);

	template<typename T>
	void TestAssertArrayContentsMatch(const T& lhs, const T& rhs);

	template<typename T>
	void TestAssertMapsMatch(const T& lhs, const T& rhs);

	void TestAssertObjectsMatch(const SPtr<UnitTestSerializationObjectA>& lhs, const SPtr<UnitTestSerializationObjectA>& rhs, bool isDelta);

	static SPtr<UnitTestSerializationObjectA> CreateSerializationTestObjectVariantA();
	static SPtr<UnitTestSerializationObjectA> CreateSerializationTestObjectVariantB();
};

CoreTestSuite::CoreTestSuite()
{
	B3D_ADD_TEST(CoreTestSuite::TestAnimCurveIntegration)
	B3D_ADD_TEST(CoreTestSuite::TestLookupTable)
	B3D_ADD_TEST(CoreTestSuite::TestBinarySerialization)
	B3D_ADD_TEST(CoreTestSuite::TestSerializedObject)
	B3D_ADD_TEST(CoreTestSuite::TestBinaryDelta)

	// TODO - Add unit tests for:
	// - Binary cloner test that restores external references
	// - SceneObject/Component serialization/deserialization
	// - Various Prefab operations
}

SPtr<UnitTestSerializationObjectA> CoreTestSuite::CreateSerializationTestObjectVariantA()
{
	return B3DMakeShared<UnitTestSerializationObjectA>();
}

SPtr<UnitTestSerializationObjectA> CoreTestSuite::CreateSerializationTestObjectVariantB()
{
	SPtr<UnitTestSerializationObjectA> object = B3DMakeShared<UnitTestSerializationObjectA>();

	object->IntA = 995;
	object->StrA = "potato";
	object->ArrStrB = { "orange", "carrot" };
	object->ArrStrC[2] = "banana";
	object->ObjB.IntA = 9940;
	object->ObjPtrB->StrA = "kiwi";
	object->ObjPtrC = nullptr;
	object->ObjPtrD = B3DMakeShared<UnitTestSerializationObjectB>();
	object->ObjPtrE = object->ObjPtrA;
	object->ArrObjB[1].StrA = "strawberry";
	object->ArrObjPtrB[0]->IntA = 99100;
	object->PlainMapB =  { { 25, "newValue25" }, { 210, "newValue210" }, { 215, "newValue215" } };
	object->PlainMapC[5] = "newValue5";
	object->PlainMapD[225] = "newValue225";
	object->PlainMapE.erase(object->PlainMapE.find(10));
	object->ObjectMapB["a"].IntA = 9940;
	object->ObjectMapB["c"] = UnitTestSerializationObjectB();
	object->ObjectMapC["b"].StrA = "strawberry";
	object->ObjectMapC.erase("a");
	object->ObjectPointerMap["b"] = nullptr;
	object->ObjectPointerMap["f"] = object->ObjectPointerMap["a"];
	object->ObjectPointerMap.erase("c");
	object->ObjectPointerMap["d"]->IntA = 9940;

	return object;
}

template<typename T>
void CoreTestSuite::TestAssertArraysMatch(const T& lhs, const T& rhs)
{
	B3D_TEST_ASSERT(lhs.size() == rhs.size())
	for(u32 i = 0; i < (u32)lhs.size(); i++)
		B3D_TEST_ASSERT(lhs[i] == rhs[i])
}

template<typename T>
void CoreTestSuite::TestAssertArrayContentsMatch(const T& lhs, const T& rhs)
{
	B3D_TEST_ASSERT(lhs.size() == rhs.size())
	for(u32 i = 0; i < (u32)lhs.size(); i++)
	{
		if(lhs[i] != nullptr)
		{
			B3D_TEST_ASSERT(rhs[i] != nullptr)
			B3D_TEST_ASSERT(*(lhs[i]) == *(rhs[i]))
		}
		else
		{
			B3D_TEST_ASSERT(rhs[i] == nullptr)
		}
	}
}

template<typename T>
void CoreTestSuite::TestAssertMapsMatch(const T& lhs, const T& rhs)
{
	B3D_TEST_ASSERT(lhs.size() == rhs.size())
	for(const auto& pair : lhs)
	{
		auto found = rhs.find(pair.first);
		B3D_TEST_ASSERT(found != rhs.end())
		B3D_TEST_ASSERT(found->second == pair.second)
	}
}

void CoreTestSuite::TestAssertObjectsMatch(const SPtr<UnitTestSerializationObjectA>& lhs, const SPtr<UnitTestSerializationObjectA>& rhs, bool isDelta = false)
{
	B3D_TEST_ASSERT(lhs->IntA == rhs->IntA)
	B3D_TEST_ASSERT(lhs->StrA == rhs->StrA)
	B3D_TEST_ASSERT(lhs->StrB == rhs->StrB)

	B3D_TEST_ASSERT(lhs->ObjA.IntA == rhs->ObjA.IntA)
	B3D_TEST_ASSERT(lhs->ObjB.IntA == rhs->ObjB.IntA)

	B3D_TEST_ASSERT(*lhs->ObjPtrA == *rhs->ObjPtrA)
	B3D_TEST_ASSERT(*lhs->ObjPtrB == *rhs->ObjPtrB)
	B3D_TEST_ASSERT(lhs->ObjPtrC == rhs->ObjPtrC)
	B3D_TEST_ASSERT(*lhs->ObjPtrD == *rhs->ObjPtrD)
	B3D_TEST_ASSERT(*lhs->ObjPtrE == *rhs->ObjPtrE)

	if(!isDelta) // TODO Skipping these tests for deltas, as it's not a case it handled yet.
	{
		B3D_TEST_ASSERT(lhs->ObjPtrA == lhs->ObjPtrE)
		B3D_TEST_ASSERT(rhs->ObjPtrA == rhs->ObjPtrE)
	}

	TestAssertArraysMatch(lhs->ArrStrA, rhs->ArrStrA);
	TestAssertArraysMatch(lhs->ArrStrB, rhs->ArrStrB);
	TestAssertArraysMatch(lhs->ArrStrC, rhs->ArrStrC);
	TestAssertArraysMatch(lhs->ArrObjA, rhs->ArrObjA);
	TestAssertArraysMatch(lhs->ArrObjB, rhs->ArrObjB);
	TestAssertArrayContentsMatch(lhs->ArrObjPtrA, rhs->ArrObjPtrA);
	TestAssertArrayContentsMatch(lhs->ArrObjPtrB, rhs->ArrObjPtrB);

	TestAssertMapsMatch(lhs->PlainMapA, rhs->PlainMapA);
	TestAssertMapsMatch(lhs->PlainMapB, rhs->PlainMapB);
	TestAssertMapsMatch(lhs->PlainMapC, rhs->PlainMapC);
	TestAssertMapsMatch(lhs->PlainMapD, rhs->PlainMapD);
	TestAssertMapsMatch(lhs->PlainMapE, rhs->PlainMapE);

	TestAssertMapsMatch(lhs->ObjectMapA, rhs->ObjectMapA);
	TestAssertMapsMatch(lhs->ObjectMapB, rhs->ObjectMapB);
	TestAssertMapsMatch(lhs->ObjectMapC, rhs->ObjectMapC);

	B3D_TEST_ASSERT(lhs->ObjectPointerMap.size() == rhs->ObjectPointerMap.size())
	for(const auto& pair : lhs->ObjectPointerMap)
	{
		auto found = rhs->ObjectPointerMap.find(pair.first);
		B3D_TEST_ASSERT(found != rhs->ObjectPointerMap.end())

		if(pair.second != nullptr)
		{
			B3D_TEST_ASSERT(found->second != nullptr)
			B3D_TEST_ASSERT(*(pair.second) == *(found->second))
		}
		else
		{
			B3D_TEST_ASSERT(found->second == nullptr)
		}
	}

	if(!isDelta) // TODO Skipping these tests for deltas, as it's not a case it handled yet.
	{
		B3D_TEST_ASSERT(lhs->ObjectPointerMap.find("a") != lhs->ObjectPointerMap.end())
		B3D_TEST_ASSERT(lhs->ObjectPointerMap.find("f") != lhs->ObjectPointerMap.end())
		B3D_TEST_ASSERT(lhs->ObjectPointerMap["a"] == lhs->ObjectPointerMap["f"])

		B3D_TEST_ASSERT(rhs->ObjectPointerMap.find("a") != rhs->ObjectPointerMap.end())
		B3D_TEST_ASSERT(rhs->ObjectPointerMap.find("f") != rhs->ObjectPointerMap.end())
		B3D_TEST_ASSERT(rhs->ObjectPointerMap["a"] == rhs->ObjectPointerMap["f"])
	}
}

void CoreTestSuite::TestAnimCurveIntegration()
{
	static constexpr float EPSILON = 0.0001f;

	// Construct some curves
	TAnimationCurve<float> curveConstant(
		{
			TKeyframe<float>{ 1.0f, 0.0f, 0.0f, 0.0f },
		});

	TAnimationCurve<float> curveLinear(
		{ TKeyframe<float>{ 0.0f, 0.0f, 1.0f, 0.0f },
		  TKeyframe<float>{ 1.0f, 1.0f, 0.0f, 1.0f } });

	TAnimationCurve<float> curveAcceleration(
		{ TKeyframe<float>{ -9.81f, 0.0f, 0.0f, 0.0f },
		  TKeyframe<float>{ -9.81f, 0.0f, 0.0f, 10.0f } });

	{
		TCurveIntegrationCache<float> cache;
		B3D_TEST_ASSERT(Math::ApproxEquals(curveConstant.EvaluateIntegrated(0.0f, cache), 0.0f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(curveConstant.EvaluateIntegrated(0.5f, cache), 0.5f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(curveConstant.EvaluateIntegrated(1.0f, cache), 1.0f, EPSILON));
	}

	{
		TCurveIntegrationCache<float> cache;
		B3D_TEST_ASSERT(Math::ApproxEquals(curveLinear.EvaluateIntegrated(0.0f, cache), 0.0f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(curveLinear.EvaluateIntegrated(0.5f, cache), 0.125f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(curveLinear.EvaluateIntegrated(1.0f, cache), 0.5f, EPSILON));
	}

	{
		TCurveIntegrationCache<float> cache;

		float times[] = { 0.0f, 0.5f, 1.0f };
		for(auto time : times)
		{
			B3D_TEST_ASSERT(Math::ApproxEquals(curveConstant.EvaluateIntegratedDouble(time, cache), EvaluatePosition(1.0f, 0.0f, time), EPSILON));
		}
	}

	{
		TCurveIntegrationCache<float> cache;

		float times[] = { 0.0f, 0.5f, 1.0f, 2.0f, 3.0f, 5.0f, 10.0f };
		for(auto time : times)
		{
			B3D_TEST_ASSERT(Math::ApproxEquals(curveAcceleration.EvaluateIntegrated(time, cache), EvaluateVelocity(-9.81f, time), EPSILON));
		}

		std::pair<float, float> range = curveAcceleration.CalculateRangeIntegrated(cache);
		B3D_TEST_ASSERT(Math::ApproxEquals(range.first, -98.1f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(range.second, 0.0f, EPSILON));
	}

	{
		TCurveIntegrationCache<float> cache;

		float times[] = { 0.0f, 0.5f, 1.0f, 2.0f, 3.0f, 5.0f, 10.0f };
		for(auto time : times)
		{
			B3D_TEST_ASSERT(Math::ApproxEquals(curveAcceleration.EvaluateIntegratedDouble(time, cache), EvaluatePosition(-9.81f, 0.0f, time)));
		}

		std::pair<float, float> range = curveAcceleration.CalculateRangeIntegratedDouble(cache);
		B3D_TEST_ASSERT(Math::ApproxEquals(range.first, -490.5f, EPSILON));
		B3D_TEST_ASSERT(Math::ApproxEquals(range.second, 0.0f, EPSILON));
	}
}

void CoreTestSuite::TestLookupTable()
{
	static constexpr float EPSILON = 0.001f;

	TAnimationCurve<Vector3> curve({
		TKeyframe<Vector3>{ Vector3(0.0f, 0.0f, 0.0f), Vector3::kZero, Vector3::kOne, 0.0f },
		TKeyframe<Vector3>{ Vector3(5.0f, 3.0f, 10.0f), Vector3::kOne, Vector3::kZero, 10.0f },
	});

	Vector3Distribution dist = curve;
	auto lookupTable = dist.ToLookupTable(128);

	for(u32 i = 0; i < 10; i++)
	{
		const float* left;
		const float* right;
		float lerp;

		float t = (i / 9.0f) * 1.0f;
		lookupTable.Evaluate(t, left, right, lerp);

		Vector3* leftVec = (Vector3*)left;
		Vector3* rightVec = (Vector3*)right;

		Vector3 valueLookup = Vector3::Lerp(lerp, *leftVec, *rightVec);
		Vector3 valueCurve = curve.Evaluate(t);

		for(u32 j = 0; j < 3; j++)
			B3D_TEST_ASSERT(Math::ApproxEquals(valueLookup[j], valueCurve[j], EPSILON));
	}
}

void CoreTestSuite::TestBinarySerialization()
{
	const SPtr<UnitTestSerializationObjectA> object = CreateSerializationTestObjectVariantB();
	
	SPtr<MemoryDataStream> stream = B3DMakeShared<MemoryDataStream>();
	BinarySerializer serializer;
	serializer.Encode(object.get(), stream, BinarySerializerFlag::None);

	stream->Seek(0);

	const SPtr<UnitTestSerializationObjectA> deserializedObject = B3DRTTICast<UnitTestSerializationObjectA>(serializer.Decode(stream, (u32)stream->Size()));
	TestAssertObjectsMatch(object, deserializedObject);
}

void CoreTestSuite::TestSerializedObject()
{
	const SPtr<UnitTestSerializationObjectA> object = CreateSerializationTestObjectVariantB();

	const SPtr<SerializedObject> serializedObject = SerializedObject::Create(*object);
	const SPtr<UnitTestSerializationObjectA> deserializedObject = B3DRTTICast<UnitTestSerializationObjectA>(serializedObject->Decode());

	TestAssertObjectsMatch(object, deserializedObject);
}

void CoreTestSuite::TestBinaryDelta()
{
	const SPtr<UnitTestSerializationObjectA> objectA = CreateSerializationTestObjectVariantA();
	const SPtr<UnitTestSerializationObjectA> objectB = CreateSerializationTestObjectVariantB();

	const SPtr<SerializedObject> serializedObjectA = SerializedObject::Create(*objectA.get());
	const SPtr<SerializedObject> serializedObjectB = SerializedObject::Create(*objectB.get());

	IDeltaHandler& deltaHandler = objectA->GetRtti()->GetDeltaHandler();
	SPtr<SerializedObject> delta = deltaHandler.GenerateDelta(serializedObjectA, serializedObjectB);
	deltaHandler.ApplyDelta(objectA, delta, nullptr);

	TestAssertObjectsMatch(objectA, objectB, true);
}

using namespace bs;

int main()
{
	MemStack::BeginThread();

	SPtr<TestSuite> tests = CoreTestSuite::Create<CoreTestSuite>();

	ExceptionTestOutput testOutput;
	tests->Run(testOutput);

	MemStack::EndThread();
	return 0;
}
