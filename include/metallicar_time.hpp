/*
 * metallicar_time.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_TIME_HPP_
#define METALLICAR_TIME_HPP_

// standard
#include <cstdint>

namespace metallicar {

class Time {
  public:
    static uint32_t get();
};

class Stopwatch {
  private:
    bool started;
    bool paused;
    uint32_t initialTime;
    uint32_t pauseTime;
  public:
    Stopwatch();
    void start();
    void pause();
    void resume();
    void reset();
    uint32_t time();
};

} // namespace metallicar

#endif /* METALLICAR_TIME_HPP_ */
