/*
 * core.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef GAME_HPP_
#define CORE_HPP_

// standard
#include <string>

// local
#include "asset.hpp"

namespace metallicar {
namespace engine {

class GameScene {
  protected:
    Assets assets;
  public:
    bool frozen;
    bool visible;
  protected:
    GameScene();
  public:
    virtual ~GameScene();
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void wakeup(void* args) = 0;
};

struct WindowOptions {
  std::string title;
  int width;
  int height;
  bool fullscreen;
  std::string icon;
  WindowOptions();
  WindowOptions(const std::string& title, int width, int height, bool fullscreen, const std::string& icon);
};

class Game {
  public:
    static void init(const WindowOptions& windowOptions = WindowOptions());
    static void run(GameScene* firstScene);
    
    static void changeScene(GameScene* scene);
    static void pushScene(GameScene* scene);
    static void popScene(void* args);
    static void quit();
    
    static void setWindowOptions(const WindowOptions& windowOptions);
    
    static bool quitRequested();
    static void resetQuitRequest();
};

} // namespace engine
} // namespace metallicar

#endif /* CORE_HPP_ */
