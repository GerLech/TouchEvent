/*
||
|| @file Touchevent.h
|| @version 0.1
|| @author Gerald Lechner
|| @contact lechge@gmail.com
||
|| @description
|| | TouchEvent extends the functionality of the XPT2046_Touchscreen
|| | library by Paul Stoffregen
|| | It offers an easy way to implement click, doubleclick, draw and swipe
|| | on a Touchscreen using the XPT2046 controller
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#ifndef TouchEvent_h
#define TouchEvent_h
#include <Arduino.h>
#include <XPT2046_Touchscreen.h>

#define MOVETRESHOLD 10 //define the minimal movement to detect a move
#define SWIPEX 500 //define a minimum X-movement to detect an x swipe
#define SWIPEY 700 //define a minimum Y-movement to detect an y swipe
#define CLICKLONG 1000 //define a minimum time to detect a long click
#define DOUBLECLICK 500 //define a minimum time between two clicks to detect a doubleclick
#define SCREENX 240 //screen resolution in pixels
#define SCREENY 320 //screen resolution in pixels

// for Touchscreen
// Parameter für Touchscreen
#define MINPRESSURE 10 //pressure to detect touch
#define TS_MINX 230 //minimal x return value
#define TS_MINY 350 //minimal y return value
#define TS_MAXX 3700  //maximal x return value
#define TS_MAXY 3900 //maximal y return value


enum class EV : uint8_t {
  EVT_UP    = 0,
  EVT_DOWN  = 1,
  EVT_CLICK = 2,
  EVT_LONG  = 3,
  EVT_SWIPE = 4,
  EVT_DRAW = 5,
  EVT_DBLCLICK = 6,
};


//typedef std::function<void(TS_Point point)> TE_TouchEvent;
//typedef std::function<void(uint8_t direction)> TE_SwipeEvent;

class TouchEvent {
public:
  //initializer we get a pointer to the XPT2046 driver
  TouchEvent(XPT2046_Touchscreen& touch);
  //this is the main function which should be called in the main loop
  //positions will be checked and under certain conditions events will be triggered
  void pollTouchScreen();
  //the number of pixels for the used TFT display default is x=240 y=320
  //if coordinates are from right to left or from bottom to top, use negative numbers
  void setResolution(int16_t xResolution, int16_t yResolution);
  //Switches draw mode on or off
  //move events will be triggered only if in draw mode
  //swipe bevents only if not in draw mode
  void setDrawMode(boolean drawMode);
  //values to define the top left and the bottom right values
  //returned from the touch screen. default values are for
  //for a 2.4 inch TFT display with 320 x 240 pixels
  void calibrate(uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax);
  //define the minimal difference to the last point to detect a move
  //default = 10
  void setMoveTreshold(uint16_t threshold);
  //define the minimal distance between touch start and touch end to
  //detect a swipe event default = 500 and 700
  void setSwipe(uint16_t swipeX, uint16_t swipeY);
  //define the minimal time in ms to interpret a click as a long click
  //default = 1000 ms
  void setLongClick(uint16_t clickLong);
  //define the minimal time between to clicks to detect a double click
  //default = 500 ms
  void setDblClick(uint16_t dblclick);
  //register a callback function for touch start event
  //void onEvent(TS_Point position)
  void registerOnTouchDown(void (*callback)(TS_Point p));
  //register a callback function for touch en event
  //void onEvent(TS_Point position)
  void registerOnTouchUp(void (*callback)(TS_Point p));
  //register a callback function for touch click event
  //void onEvent(TS_Point position)
  void registerOnTouchClick(void (*callback)(TS_Point p));
  //register a callback function for touch double click event
  //void onEvent(TS_Point position)
  void registerOnTouchDblClick(void (*callback)(TS_Point p));
  //register a callback function for touch long click event
  //void onEvent(TS_Point position)
  void registerOnTouchLong(void (*callback)(TS_Point p));
  //register a callback function for touch draw event
  //void onEvent(TS_Point position)
  void registerOnTouchDraw(void (*callback)(TS_Point p));
  //register a callback function for touch swipe event
  //direction 0 = right to left 1 = left to right
  //2 = bottom to top 3 = top to bottom
  //void onEvent(uint8_t direction)
  void registerOnTouchSwipe(void (*callback)(uint8_t direction));
  //register a callback on any event.
  //void onAllEvents(int16_t x, int16_t y, EV event)
  void registerOnAllEvents(void (*callback)(int16_t x,int16_t y,EV event));
  //function to detect if a position is inside a rectangle
  //this is useful to detect if a click hit a button
  boolean isInArea(TS_Point p, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  //function to automatic set min and max values
  //which: none = 0, xmin = 1, ymin = 2, xmax = 3, ymax = 4
  void autocalibrate(uint8_t which);
  //read the min and max raw values
  void getMinMax(uint16_t * xmin, uint16_t * ymin, uint16_t * xmax, uint16_t * ymax);


private:
  XPT2046_Touchscreen& _touch;
  TS_Point _last; //last move position
  TS_Point _start; //position at start touch
  boolean _down; //last touch status
  uint32_t _begin; //start of last touched
  uint32_t _lastClick; //time of last click
  boolean _long; //a long click event was already triggered
  boolean _drawMode; //draw mode means move events but no swipe events
  //setup
  int16_t _xResolution = SCREENX; //screen pixles minus to flip x
  int16_t _yResolution = SCREENY; //screen pixles minus to flip y
  uint16_t _moveth = MOVETRESHOLD; //define the minimal movement to detect a move
  uint16_t _swipeX = SWIPEX; //define a minimum X-movement to detect an x swipe
  uint16_t _swipeY = SWIPEY; //define a minimum Y-movement to detect an y swipe
  uint16_t _clickLong = CLICKLONG; //define a minimum time to detect a long click
  uint16_t _dblClick = DOUBLECLICK; //define a minimum time between two clicks to detect a doubleclick

  uint16_t _minPress = MINPRESSURE; //pressure to detect touch
  uint16_t _tsMinX = TS_MINX; //minimal x return value
  uint16_t _tsMinY = TS_MINY; //minimal y return value
  uint16_t _tsMaxX = TS_MAXX;  //maximal x return value
  uint16_t _tsMaxY = TS_MAXY; //maximal y return value
  uint8_t _autoWhich;

  void(*_onTouchDown)(TS_Point p) = NULL;
  void(*_onTouchUp)(TS_Point p) = NULL;
  void(*_onTouchLong)(TS_Point p) = NULL;
  void(*_onTouchDraw)(TS_Point p) = NULL;
  void(*_onTouchClick)(TS_Point p) = NULL;
  void(*_onTouchDblClick)(TS_Point p) = NULL;
  void(*_onTouchSwipe)(uint8_t direction) = NULL;
  void(*_onAllEvents)(int16_t x,int16_t y,EV event) = NULL;

  //convert raw position into screen coordinates
  TS_Point toScreen(TS_Point p);
  //calculate swipe direction from start and end point
  uint8_t swipeDirection(TS_Point b, TS_Point e);

};
#endif
