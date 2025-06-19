---
title: Modules
---

A @b3d::Module<T> is a specialized form of singleton used for many of b3d::f systems. Unlike standard singletons it requires manual startup and shutdown. To use it for your own objects, simply inherit from it and provide your own class as its template parameter.

~~~~~~~~~~~~~{.cpp}
class MyModule : public Module<MyModule>
{ };
~~~~~~~~~~~~~

Use @b3d::Module<T>::startUp to start it up. Once started use @b3d::Module<T>::instance to access its instance. Once done with it call @b3d::Module<T>::shutDown to release it.

~~~~~~~~~~~~~{.cpp}
MyModule::startUp();
MyModule::instance().doSomething();
MyModule::shutDown();
~~~~~~~~~~~~~
