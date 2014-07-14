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

class GameScene {
  public:
    
};

class Game {
  public:
    static void init(const ScreenOptions& screenOptions = ScreenOptions());
    static void run(GameScene* firstScene);
    
    static void changeScene(GameScene* scene);
    static void pushScene(GameScene* scene);
    static void popScene(void* args);
    static void quit();
    
    static void setScreenOptions(const ScreenOptions& screenOptions);
    
    static bool quitRequested();
    static void resetQuitRequest();
};

} // engine metallicar
} // namespace metallicar

#endif /* GAME_HPP_ */
