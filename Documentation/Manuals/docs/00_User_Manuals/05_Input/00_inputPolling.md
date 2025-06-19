---
title: Input polling
---

Input polling refers to the process of querying the input system to check if user interacted with a input device in some way. This may include checking if the user pressed a keyboard, mouse or a gamepad button or moved the mouse or an analog axis. 

All the input in b3d::f is handled though the @b3d::Input class, which can be globally accessed through the @b3d::GetInput method.

# Button presses
Use the following methods to check if a button has been pressed, released or is currently being held down:
 - @b3d::Input::isButtonDown - Checks has the button been pressed this frame. Only valid for one frame.
 - @b3d::Input::isButtonHeld - Checks is the button currently being held. This is valid for the first frame the button is pressed, and for any following frame until it is released.
 - @b3d::Input::isButtonUp - Checks has the button been released this frame. Only valid for one frame.
 
These methods work on any kind of input device buttons, including keyboard, gamepad and mouse. Use @b3d::ButtonCode to choose which button to query for.

~~~~~~~~~~~~~{.cpp}
Vector3 position(BsZero);

// Move 5 units forward for every frame while W key is pressed
if(GetInput().isButtonHeld(BC_W))
	position.z += 5.0f;
~~~~~~~~~~~~~

# Analog input
Moving the mouse, gamepad sticks or triggers results in an analog input. While buttons can only be toggled on or off, analog input is received in a specific range (for example, anywhere between -1 and 1). This allows more precise input for applications that require it.

In b3d::f analog input is represented through the concept of *axes*. Use @b3d::Input::getAxisValue to get a value for a specific axis. Check @b3d::InputAxis for a list of all supported axes.

~~~~~~~~~~~~~{.cpp}
Vector3 position(BsZero);

// Move forward or backwards depending on how much does the user move the left gamepad stick forward or backwards
position.z += GetInput().getAxisValue(InputAxis::LeftStickY);
~~~~~~~~~~~~~

Most axes report their input in range [-1, 1], with the exception of mouse axes, which are unbound. 

# Mouse input
Often it is useful to receive mouse position directly, rather than dealing with raw mouse axis data. Use @b3d::Input::getPointerPosition to retrieve the current position of the mouse cursor, in coordinates relative to the screen.

Use @b3d::Input::getPointerDelta to get the difference in coordinates between the position of the mouse on the previous and current frame.

~~~~~~~~~~~~~{.cpp}
Vector2I screenPos = GetInput().getPointerPosition();
~~~~~~~~~~~~~

You can also check if the left mouse button has been double-clicked by checking @b3d::Input::isPointerDoubleClicked().

~~~~~~~~~~~~~{.cpp}
if(GetInput().isPointerDoubleClicked())
	GetDebug().logDebug("Mouse double-clicked!");
~~~~~~~~~~~~~
