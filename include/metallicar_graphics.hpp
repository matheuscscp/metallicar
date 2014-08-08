/*
 * metallicar_graphics.hpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_GRAPHICS_HPP_
#define METALLICAR_GRAPHICS_HPP_

#include <functional>

namespace metallicar {

class Graphics {
  public:
    static void setTransformations(const std::function<void()>& transfs);
    static void performTransformations();
};

} // namespace metallicar

#endif /* METALLICAR_GRAPHICS_HPP_ */
