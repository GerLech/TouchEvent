# TouchEvent
An ARDUINO library to poll touchscreen based on XPD 2046 controller and call functions on certain events.

It is Based on the library [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) by Paul Stoffregen.

To init the class youneed to pass a pointer on an instance of XPT2046_Touchscreen.

Into the main loop put a call to the pollTouchScreen() function of this library. This function keeps track about touch position and touch status. If callbacks were registered, these functions will be called on the following events:

- **onTouchDown** Touch screen was touched. The callback function gets the position in the displays coordinates.
- **onTouchUp**   Touch screen was no longer touched. The callback function gets the position in the displays coordinates.
- **onTouchClick** Touch screen was touched and released. The callback function gets the position in the displays coordinates.
- **onTouchDblClick** Touch screen was touched and released twice in a short time. The callback function gets the position in the displays coordinates.
- **onTouchLongClick** Touch screen was touched for a longer time before released. The callback function gets the position in the displays coordinates.
- **onTouchDraw** Touchpoint was moved while touched. This event is only active if draw mode is true. The callback function gets the position in the displays coordinates.
- **onTouchSwipe** Touch was moved a certain distance while touched. This event is only active if draw mode is false. The callback function gets the direction of the movement.

All required parameters will be initialized by common valid default values and can be changed at run time.
