/*
 * metallicar_graphics.hpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_GRAPHICS_HPP_
#define METALLICAR_GRAPHICS_HPP_

// standard
#include <functional>

namespace metallicar {

class Graphics {
  public:
    static void initDefault();
    
    static void setFramePreparation(const std::function<void()>& preparation);
    static void setFrameFinalization(const std::function<void()>& finalization);
    
    static void prepareFrame();
    static void finalizeFrame();
};

} // namespace metallicar

#endif /* METALLICAR_GRAPHICS_HPP_ */
