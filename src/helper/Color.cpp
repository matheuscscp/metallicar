/*
 * Color.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "Color.hpp"

const Color Color::BLACK  (0.0f, 0.0f, 0.0f);
const Color Color::GRAY   (0.5f, 0.5f, 0.5f);
const Color Color::WHITE  (1.0f, 1.0f, 1.0f);
const Color Color::RED    (1.0f, 0.0f, 0.0f);
const Color Color::GREEN  (0.0f, 1.0f, 0.0f);
const Color Color::BLUE   (0.0f, 0.0f, 1.0f);
const Color Color::CYAN   (0.0f, 1.0f, 1.0f);
const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
const Color Color::YELLOW (1.0f, 1.0f, 0.0f);

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
  
}

Color::Color(int r, int g, int b, int a) :
r(r/255.0f), g(g/255.0f), b(b/255.0f), a(a/255.0f)
{
  
}

Color::Color(uint32_t hex) :
r(((hex >> 16) & 0xFF)/255.0f),
g(((hex >>  8) & 0xFF)/255.0f),
b(((hex >>  0) & 0xFF)/255.0f),
a(((hex >> 24) & 0xFF)/255.0f)
{
  
}
