---
title: Containers
---
Containers are data types that contain a set of elements. b3d::f uses wrappers for C++ standard library containers like vector, set or map. Aside from different names these containers act exactly like standard library containers.

All available containers:
 - @b3d::Vector - A sequential list of elements. Fast iteration, slow lookup, slow insertion/deletion (except for the last element).
 - @b3d::List - A sequential list of elements. Slow iteration, slow lookup, fast insertion/deletion for all elements.
 - @b3d::Stack - A collection of elements where elements can only be retrieved and inserted in a specific order. Last element inserted is always the first element retrieved.
 - @b3d::Queue - A collection of elements where elements can only be retrieved and inserted in a specific order. First element inserted is always the first element retrieved.
 - @b3d::Set - An ordered list of elements. Fast iteration, fast lookup, mediocre insertion/deletion.
 - @b3d::Map - Same as **Set**, only each element is represented as a key-value pair, while **Set** only contains keys.
 - @b3d::UnorderedSet - Similar to **Set** it allows fast lookup, but elements aren't sorted. In general offers better performance than **Set**.
 - @b3d::UnorderedMap - Same as **UnorderedSet**, only each element is represented as a key-value pair, while **Set** only contains keys.
 - @b3d::TInlineArray - A dynamically sized container that statically allocates enough room for N elements. If the element count exceeds the statically allocated buffer size the vector falls back to general purpose dynamic allocator.
 - @b3d::TArray - Dynamically sized array, similar to **Vector** but without relying on the standard library.
 - @b3d::DenseMap - Hash-map with densely stored values, using quadratic probing for lookup.
 
An example with a vector and an unordered map:
~~~~~~~~~~~~~{.cpp}
int nextUserId = 0;
Vector<String> userNames = { "Sleepy", "Grumpy", "Dopey" };

// Generate IDs for each user and store them in a map for quick lookup.
UnorderedMap<String, int> users;
for(auto& entry : userNames)
	users[entry] = nextUserId++;

// Perform lookup to check if user exists
auto iterFind = users.find("Sneezy");
if(iterFind != users.end())
{
	// User exists
	int userId = iterFind->second;
}
~~~~~~~~~~~~~
