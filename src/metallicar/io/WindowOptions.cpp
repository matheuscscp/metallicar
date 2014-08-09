/*
 * WindowOptions.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_io.hpp"

using namespace std;

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

} // namespace metallicar
