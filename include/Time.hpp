/*
 * Time.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef TIME_HPP_
#define TIME_HPP_

// standard
#include <cstdint>

namespace metallicar {
namespace engine {

class Time {
  public:
    static uint32_t get();
};

class DeltaTime {
  public:
    static uint32_t getUPS();
    static void setUPS(uint32_t ups);
    static float getDT();
    static uint32_t updateID();
    
    /// engine's private use
    static void init();
    /// engine's private use
    static void update();
    /// engine's private use
    static bool periodReached();
    /// engine's private use
    static void accumulate();
};

} // engine metallicar
} // namespace metallicar

#endif /* TIME_HPP_ */
