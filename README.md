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

**Reference to all defined funktions:**

- **void pollTouchScreen();**
  this is the main function which should be called in the main loop
  positions will be checked and under certain conditions events will be triggered
- **void setResolution(int16_t xResolution, int16_t yResolution);**
  the number of pixels for the used TFT display default is x=240 y=320
  if coordinates are from right to left or from bottom to top, use negative numbers
- **void setDrawMode(boolean drawMode);**
  Switches draw mode on or off
  move events will be triggered only if in draw mode
  swipe bevents only if not in draw mode
- **void calibrate(uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax);**
  values to define the top left and the bottom right values
  returned from the touch screen. default values are for
  for a 2.4 inch TFT display with 320 x 240 pixels
- **void setMoveTreshold(uint16_t threshold);**
  define the minimal difference to the last point to detect a move
  default = 10
- **void setSwipe(uint16_t swipeX, uint16_t swipeY);**
  define the minimal distance between touch start and touch end to
  detect a swipe event default = 500 and 700
- **void setLongClick(uint16_t clickLong);**
  define the minimal time in ms to interpret a click as a long click
  default = 1000 ms
- **void setDblClick(uint16_t dblclick);**
  define the minimal time between to clicks to detect a double click
  default = 500 ms 
- **void registerOnTouchDown(void (\*callback)(TS_Point p));**
  register a callback function for touch start event
  void onEvent(TS_Point position)
- **void registerOnTouchUp(void (\*callback)(TS_Point p));**
  register a callback function for touch en event
  void onEvent(TS_Point position)
- **void registerOnTouchClick(void (\*callback)(TS_Point p));**
  register a callback function for touch click event
  void onEvent(TS_Point position)
- **void registerOnTouchDblClick(void (\*callback)(TS_Point p));**
  register a callback function for touch double click event
  void onEvent(TS_Point position)
- **void registerOnTouchLong(void (\*callback)(TS_Point p));**
  register a callback function for touch long click event
  void onEvent(TS_Point position)
- **void registerOnTouchDraw(void (\*callback)(TS_Point p));**
  register a callback function for touch draw event
  void onEvent(TS_Point position)
- **void registerOnTouchSwipe(void (\*callback)(uint8_t direction));**
  register a callback function for touch swipe event
  direction 0 = right to left 1 = left to right
  2 = bottom to top 3 = top to bottom
  void onEvent(uint8_t direction)
- **boolean isInArea(TS_Point p, int16_t x1, int16_t y1, int16_t x2, int16_t y2);**
  function to detect if a position is inside a rectangle
  this is useful to detect if a click hit a button

