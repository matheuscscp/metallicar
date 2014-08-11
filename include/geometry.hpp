/*
 * geometry.hpp
 *
 *  Created on: Aug 10, 2014
 *      Author: Pimenta
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

namespace geometry {

struct Point2 {
  float x, y;
  Point2(float x = 0.0f, float y = 0.0f);
};

struct Rectangle {
  enum Spot { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER };
  float x, y, w, h;
  Rectangle(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f);
};

} // namespace geometry

#endif /* GEOMETRY_HPP_ */
