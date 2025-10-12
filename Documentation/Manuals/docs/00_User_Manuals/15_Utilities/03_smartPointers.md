---
title: Smart pointers
---

Smart pointers allow the user to allocate objects dynamically, but without having to worry about freeing the object. They are prefered to normal memory allocation as they prevent memory leaks when the user might forget to free memory. They are particularily very useful in situations when object ownership is not clearly defined and it might not be clear who is responsible for freeing the object, or when.

# Unique pointers
Unique pointers hold ownership of a dynamically allocated object, and automatically free it when they go out of scope. As their name implies they cannot be copied - in other words, only one pointer to that object can exist. They are mostly useful for temporary allocations, or for places where object ownership is clearly defined to a single owner.

In framework's type're represented with @b3d::UPtr, which is just a wrapper for the standard library's *std::unique_ptr*. Use @b3d::B3DMakeUnique to create a unique pointer pointing to a new instance of T, or @b3d::B3DMakeUniqueFromExisting to create one from an existing instance. 

~~~~~~~~~~~~~{.cpp}
// Helper structure
struct MyStruct
{
	MyStruct() {}
	MyStruct(i32 integerValue, bool booleanValue)
		:IntegerValue(integerValue), BooleanValue(booleanValue)
	{ }

	i32 IntegerValue;
	bool BooleanValue;
};

UPtr<MyStruct> uniqueStructPointer = B3DMakeUnique<MyStruct>(123, false);

// No need to free "uniqueStructPointer", it will automatically be freed when it goes out of scope
~~~~~~~~~~~~~

## Transfering ownership
Since only a single instance of a unique pointer to a specific object may exist, they cannot be copied. However sometimes it is useful to move them to another object (transfer of ownership). In such case you can use *std::move* as shown below:

~~~~~~~~~~~~~{.cpp}
UPtr<MyStruct> uniqueStructPointer = B3DMakeUnique<MyStruct>(123, false);

// Transfer ownership
UPtr<MyStruct> otherUniquePointer = std::move(uniqueStructPointer);

// otherUniquePointer now owns the object, while uniqueStructPointer is undefined
~~~~~~~~~~~~~

# Shared pointers
Shared pointers are similar to unique pointers, as they also don't require the object to be explicitly freed after creation. However, unlike unique pointers they can be copied (therefore their name "shared"). This means multiple entities can hold a shared pointer to a single object. Only once ALL such entities lose their shared pointers will the pointed-to object be destroyed.

In the framework they are represented with @b3d::SPtr, which is just a wrapper for the standard library's *std::shared_ptr*. Use @b3d::B3DMakeShared to create a shared pointer pointing to a new instance of T, or @b3d::B3DMakeSharedFromExisting to create one from an existing instance. 

You will find the framework uses shared pointers commonly all around its codebase.

~~~~~~~~~~~~~{.cpp}
SPtr<MyStruct> sharedStructPointer = B3DMakeShared<MyStruct>(123, false);
SPtr<MyStruct> anotherSharedPointer = sharedStructPointer;

// Object will be freed after both "sharedStructPointer" and "anotherSharedPointer" go out of scope.
// Normally you'd want to pass a copy of the pointer to some other system, otherwise we could have used a unique pointer in this particular example
~~~~~~~~~~~~~
