/*
 * Color.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

#ifndef COLOR_HPP_
#define COLOR_HPP_

// standard
#include <cstdint>

struct Color {
  float r, g, b, a;
  
  static const Color BLACK;
  static const Color GRAY;
  static const Color WHITE;
  static const Color RED;
  static const Color GREEN;
  static const Color BLUE;
  static const Color CYAN;
  static const Color MAGENTA;
  static const Color YELLOW;
  
  Color(float r, float g, float b, float a = 1.0f);
  Color(int r, int g, int b, int a = 255);
  Color(uint32_t hex);
};

#endif /* COLOR_HPP_ */
