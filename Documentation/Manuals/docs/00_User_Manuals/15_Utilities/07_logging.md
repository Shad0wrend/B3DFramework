---
title: Logging	messages
---
Logging can be a useful way to debug issues during development, or notify the user that an error occurred. In b3d::f it is handled though the @b3d::Debug class, which can recognize what severity the message is and from what component or module it came. Use @b3d::GetDebug for an easy way to access the **Debug** instance.

To log a new message, you can use two of the following methods:
 - Use the @B3D_LOG macro.
 - Use the @b3d::Debug::log function, which is similar to the macro but requires the use of @B3D_LOG_GET_CATEGORY_ID macro when providing the log category if the user doesn't know the ID of the category.
 
~~~~~~~~~~~~~{.cpp}
UINT32 x = 5;

GetDebug().log("Value of x is: " + toString(x), LogVerbosity::Info, BS_LOG_GET_CATEGORY_ID(Uncategorized));

if(x != 5)
	BS_LOG(Error, Uncategorized, "X must equal 5!");
~~~~~~~~~~~~~

All logged messages will be output to the standard console output, as well as the attached debugger output (if any).

Messages are also saved internally, and can be output to a either .html file or to a text file by calling @b3d::Debug::saveLog.

~~~~~~~~~~~~~{.cpp}
GetDebug().saveLog("C:\myLog.html", SavedLogType::HTML);
GetDebug().saveLog("C:\myLog.txt", SavedLogType::Textual);
~~~~~~~~~~~~~

Sometimes you want to register your own log categories than the what b3d::f provides. For that you can use @B3D_LOG_CATEGORY macro that will create the Log category to use based on the given ID number.

~~~~~~~~~~~~~{.cpp}
BS_LOG_CATEGORY(UserApp, 100)
BS_LOG_CATEGORY(UserModule, 120)
~~~~~~~~~~~~~

Finally, the category should be registered to the @b3d::Log class that is responsible for storing log entries and the names of the categories based on their ID, so that the outputted log would contain the name of the category. This can be done by using the @B3D_LOG_CATEGORY_IMPL macro. The instance of @b3d::Log can be accessed by calling @b3d::Debug::getLog().

~~~~~~~~~~~~~{.cpp}
BS_LOG_CATEGORY_IMPL(UserApp)
GetDebug().getLog().registerCategory("UserModule", BS_LOG_GET_CATEGORY_ID(UserModule));
~~~~~~~~~~~~~

**NOTE:** It is recommended to give the user log categories sufficiently high ID number, since they may collide with the b3d::f's log categories.
