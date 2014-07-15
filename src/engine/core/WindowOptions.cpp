/*
 * WindowOptions.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {
namespace engine {

WindowOptions::WindowOptions() :
title("metallicar game"), width(800), height(600), fullscreen(false), icon("asset/icon.png"), cursor(true)
{
  
}

WindowOptions::WindowOptions(const string& title, int width, int height, bool fullscreen, const string& icon, bool cursor) :
title(title), width(width), height(height), fullscreen(fullscreen), icon(icon), cursor(cursor)
{
  
}

} // namespace engine
} // namespace metallicar
