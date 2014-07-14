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
    static void update();
    static bool periodReached();
    static void accumulate();
};

} // engine metallicar
} // namespace metallicar

#endif /* TIME_HPP_ */
