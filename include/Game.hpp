/*
 * Game.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef GAME_HPP_
#define GAME_HPP_

// standard
#include <string>

namespace metallicar {
namespace engine {

struct ScreenOptions {
  std::string title;
  int width;
  int height;
  bool fullscreen;
  std::string icon;
  ScreenOptions();
  ScreenOptions(const std::string& title, int width, int height, bool fullscreen, const std::string& icon);
};

class GameState {
  public:
    
};

class Game {
  public:
    static void init(const ScreenOptions& screenOptions = ScreenOptions());
    static void close();
    
    static void start(GameState* firstState);
    static void stop();
    
    static void setScreenOptions(const ScreenOptions& screenOptions);
};

} // engine metallicar
} // namespace metallicar

#endif /* GAME_HPP_ */
