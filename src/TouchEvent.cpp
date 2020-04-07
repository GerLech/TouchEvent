//Version 0.1
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

#include <Arduino.h>
#include <XPT2046_Touchscreen.h>
#include "TouchEvent.h"

//constructor prefill private members
TouchEvent::TouchEvent(XPT2046_Touchscreen& touch): _touch(touch) {
}
void TouchEvent::pollTouchScreen() {
  TS_Point p,scr;
  boolean tch;
  int16_t xr,yr,dx,dy;
  //first we get position and check if touched
  p=_touch.getPoint();
  scr = toScreen(p);
  tch = (p.z > _minPress);
  if (tch && _down) { //continous touch
    dx = abs(_last.x-p.x); dy = abs(_last.y-p.y);
    _last = p;
    if (_drawMode && ((dx > _moveth) || (dy > _moveth))) { //movement?
      if (_onTouchDraw) _onTouchDraw(scr);
      if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_DRAW);
    }
    if (((millis() - _begin) > _clickLong) && (!_long)) {
      if (_onTouchLong) _onTouchLong(scr);
      if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_LONG);
      _long = true;
    }
  } else if (tch && !_down) { //start of touch
    _start = p; _last = p; _begin = millis(); _long = false;
    _down = tch;
    if (_onTouchDown) _onTouchDown(scr);
    if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_DOWN);
  } else if (!tch && _down) { //Berührung endet
    _down = tch;
    _long = false;
    if (_onTouchUp) _onTouchUp(scr);
    if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_UP);
    if (!_drawMode && ((abs(_start.x - p.x) > _swipeX) || (abs(_start.y - p.y) > _swipeY))) {
      if (_onTouchSwipe) _onTouchSwipe(swipeDirection(_start,p));
    } else {
      if ((millis()-_begin) < _clickLong) {
        if ((millis()-_lastClick) < _dblClick) {
          _lastClick = 0;
          if (_onTouchDblClick) _onTouchDblClick(scr);
          if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_DBLCLICK);
        } else {
          _lastClick=millis();
          if (_autoWhich > 0) {
            switch (_autoWhich) {
              case 1: _tsMinX = p.x; break;
              case 2: _tsMinY = p.y; break;
              case 3: _tsMaxX = p.x; break;
              case 4: _tsMaxY = p.y; break;
            }
            _autoWhich = 0;
            Serial.printf("x %i y%i level %i\n",p.x,p.y,_autoWhich);
          }
          if (_onTouchClick) _onTouchClick(scr);
          if (_onAllEvents) _onAllEvents(scr.x,scr.y,EV::EVT_CLICK);
        }
      }

    }
  }

}

void TouchEvent::setResolution(int16_t xResolution, int16_t yResolution) {
  _xResolution = xResolution;
  _yResolution = yResolution;
}

void TouchEvent::setDrawMode(boolean drawMode) {
  _drawMode = drawMode;
}


void TouchEvent::calibrate(uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax) {
  _tsMinX = xMin;
  _tsMinY = yMin;
  _tsMaxX = xMax;
  _tsMaxY = yMax;
}

void TouchEvent::setMoveTreshold(uint16_t threshold) {
  _moveth = threshold;
}

void TouchEvent::setSwipe(uint16_t swipeX, uint16_t swipeY) {
  _swipeX = swipeX;
  _swipeY = swipeY;
}

void TouchEvent::setLongClick(uint16_t clickLong) {
  _clickLong = clickLong;
}

void TouchEvent::setDblClick(uint16_t dblclick) {
  _dblClick = dblclick;
}

void TouchEvent::registerOnTouchDown(void (*callback)(TS_Point p)) {
  _onTouchDown = callback;
}
void TouchEvent::registerOnTouchUp(void (*callback)(TS_Point p)) {
  _onTouchUp = callback;
}
void TouchEvent::registerOnTouchClick(void (*callback)(TS_Point p)) {
  _onTouchClick = callback;
}
void TouchEvent::registerOnTouchDblClick(void (*callback)(TS_Point p)) {
  _onTouchDblClick = callback;
}
void TouchEvent::registerOnTouchLong(void (*callback)(TS_Point p)) {
  _onTouchLong = callback;
}
void TouchEvent::registerOnTouchDraw(void (*callback)(TS_Point p)) {
  _onTouchDraw = callback;
}
void TouchEvent::registerOnTouchSwipe(void (*callback)(uint8_t direction)) {
  _onTouchSwipe = callback;
}

void TouchEvent::registerOnAllEvents(void (*callback)(int16_t x,int16_t y,EV event)) {
  _onAllEvents = callback;
}

boolean TouchEvent::isInArea(TS_Point p, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  int16_t tmp;
  if (x1 > x2) {
    tmp = x2; x2 = x1; x1 = tmp;
  }
  if (y1 > y2) {
    tmp = y2; y2 = y1; y1 = tmp;
  }
  return ((p.x >= x1) && (p.x <= x2) && (p.y >= y1) && (p.y <= y2));
}



uint8_t TouchEvent::swipeDirection(TS_Point b, TS_Point e) {
  TS_Point ptmp;
  int bx,by,ex,ey;
  uint8_t dir; //swipe direction 0 = left, 1 = right, 2 = up, 3 = down
  ptmp = toScreen(b);
  bx = ptmp.x; by = ptmp.y;
  ptmp = toScreen(e);
  ex = ptmp.x; ey = ptmp.y;
  if (abs(bx - ex) > abs(by - ey)) { //bewegung in x-Richtung
    dir = ((bx-ex) > 0)?0:1;
  } else {//bewegung in y Richtung
    dir = ((by-ey) > 0)?2:3;
  }
  return dir;
}

TS_Point TouchEvent::toScreen(TS_Point p) {
  TS_Point p1;
  if (abs(_xResolution) > abs(_yResolution)) {// orientation is landscape
    if (_xResolution > 0) {
      p1.x = map(p.x, _tsMinX, _tsMaxX, 0, _xResolution);
    } else {
      p1.x = map(p.x, _tsMinX, _tsMaxX, -_xResolution, 0);
    }
    if (_yResolution > 0) {
      p1.y = map(p.y, _tsMinY, _tsMaxY, 0, _yResolution);
    } else {
      p1.y = map(p.y, _tsMinY, _tsMaxY, -_yResolution, 0);
    }
  } else {
    if (_xResolution > 0) {
      p1.x = map(p.y, _tsMinY, _tsMaxY, _xResolution, 0);
    } else {
      p1.x = map(p.y, _tsMinY, _tsMaxY, 0, -_xResolution);
    }
    if (_yResolution > 0) {
      p1.y = map(p.x, _tsMinX, _tsMaxX, 0, _yResolution);
    } else {
      p1.y = map(p.x, _tsMinX, _tsMaxX, -_yResolution, 0);
    }
  }
  return p1;
}

//function to automatic set min and max values
//which: none = 0, xmin = 1, ymin = 2, xmax = 3, ymax = 4
void TouchEvent::autocalibrate(uint8_t which) {
  _autoWhich = which;
}
//read the min and max raw values
void TouchEvent::getMinMax(uint16_t * xmin, uint16_t * ymin, uint16_t * xmax, uint16_t * ymax) {
  *xmin = _tsMinX;
  *ymin = _tsMinY;
  *xmax = _tsMaxX;
  *ymax = _tsMaxY;
}
