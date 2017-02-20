/*
 * WindowOptions.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_io.hpp"

using namespace std;
using namespace geometry;

namespace metallicar {

WindowOptions::WindowOptions(
  const string& title,
  int width,
  int height,
  int gameWidth,
  int gameHeight,
  bool fullscreen,
  const string& icon,
  bool cursor
) :
title(title),
width(width),
height(height),
gameWidth(gameWidth),
gameHeight(gameHeight),
fullscreen(fullscreen),
icon(icon),
cursor(cursor)
{
  
}

Point2 WindowOptions::gameCoordinates(
  const Point2& windowCoordinates, bool& out
) const {
  Point2 gc(windowCoordinates);
  out = false;
  gc.x *= gameWidth/float(width);
  gc.y *= gameHeight/float(height);
  float gameRatio = float(gameWidth)/gameHeight;
  float ratio = float(width)/height;
  if (ratio > gameRatio) {
    int viewportWidth = height*gameRatio;
    float viewportX = windowCoordinates.x - (width - viewportWidth)/2.0f;
    gc.x = viewportX*gameWidth/float(viewportWidth);
    float gameWidthf = float(gameWidth);
    if (gc.x < 0.0f) {
      out = true;
      gc.x = 0.0f;
    }
    else if (gc.x > gameWidthf) {
      out = true;
      gc.x = gameWidthf;
    }
  }
  else if (ratio < gameRatio) {
    int viewportHeight = width/gameRatio;
    float viewportY = windowCoordinates.y - (height - viewportHeight)/2.0f;
    gc.y = viewportY*gameHeight/float(viewportHeight);
    float gameHeightf = float(gameHeight);
    if (gc.y < 0.0f) {
      out = true;
      gc.y = 0.0f;
    }
    else if (gc.y > gameHeightf) {
      out = true;
      gc.y = gameHeightf;
    }
  }
  return gc;
}

} // namespace metallicar
