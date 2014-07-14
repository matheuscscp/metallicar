/*
 * ScreenOptions.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "Core.hpp"

using namespace std;

namespace metallicar {
namespace engine {

ScreenOptions::ScreenOptions() :
title("metallicar game"), width(800), height(600), fullscreen(false), icon("asset/icon.png")
{
  
}

ScreenOptions::ScreenOptions(const string& title, int width, int height, bool fullscreen, const string& icon) :
title(title), width(width), height(height), fullscreen(fullscreen), icon(icon)
{
  
}

} // engine metallicar
} // namespace metallicar
