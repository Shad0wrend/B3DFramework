---
title: Any
---

@b3d::Any is a specialized data type that allows you to store any kind of object in it. For example:
~~~~~~~~~~~~~{.cpp}
Any var1 = Vector<String>();

struct MyStruct { int a; };
Any var2 = MyStruct();
~~~~~~~~~~~~~

Use @b3d::any_cast and @b3d::any_cast_ref to retrieve valid types from an **Any** variable.
~~~~~~~~~~~~~{.cpp}
Vector<String> val1 = any_cast<Vector<String>>(var1);
MyStruct& val2 = any_cast_ref<MyStruct>(var2);
~~~~~~~~~~~~~
