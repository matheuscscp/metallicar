/*
 * metallicar_core.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_CORE_HPP_
#define METALLICAR_CORE_HPP_

// standard
#include <string>
#include <list>
#include <map>
#include <functional>

// lib
#include "SDL.h"

// local
#include "metallicar_asset.hpp"
#include "Point.hpp"
#include "Observer.hpp"

namespace metallicar {
namespace engine {

class GameRenderers {
  private:
    std::map<int, std::list<std::function<void()>>> renderers;
  public:
    void add(int z, const std::function<void()>& renderer);
    void render();
};

class GameObject {
  public:
    bool destroy;
    bool frozen;
    bool visible;
  private:
    std::list<GameObject*> objects;
    std::list<GameObject*> newObjects;
  protected:
    GameObject();
  public:
    virtual ~GameObject();
    virtual void update() = 0;
    virtual void render(GameRenderers* renderers) = 0;
    virtual void wakeup(void* args) = 0;
    void add(GameObject* object);
    void updateTree();
    void renderTree(GameRenderers* renderers);
    void wakeupTree(void* args);
};

class GameScene {
  protected:
    Assets assets;
    util::Connection quitEventConnection;
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

class GameObjectScene : public GameScene {
  private:
    std::list<GameObject*> objects;
    std::list<GameObject*> newObjects;
    virtual ~GameObjectScene();
  protected:
    void add(GameObject* object);
  private:
    void update();
    void render();
    void wakeup(void* args);
};

struct WindowOptions {
  std::string title;
  int width;
  int height;
  bool fullscreen;
  std::string icon;
  bool cursor;
  WindowOptions();
  WindowOptions(const std::string& title, int width, int height, bool fullscreen, const std::string& icon, bool cursor);
};

class Game {
  public:
    static void init(const WindowOptions& windowOptions = WindowOptions());
    static void run(GameScene* firstScene);
    
    static void changeScene(GameScene* scene);
    static void pushScene(GameScene* scene);
    static void popScene(void* args);
    static void quit();
    
    static WindowOptions getWindowOptions();
    static void setWindowOptions(WindowOptions& windowOptions);
};

class Input {
  public:
    class KeyEvent : public util::Event {
      private:
        SDL_Keycode keycode;
      public:
        KeyEvent(uint32_t eventType, SDL_Keycode keycode);
        SDL_Keycode key() const;
    };
    
    class ButtonEvent : public util::Event {
      private:
        uint8_t buttoncode;
      public:
        ButtonEvent(uint32_t eventType, uint8_t buttoncode);
        uint32_t button() const;
    };
    
    static void update();
    
    static bool quitRequested();
    static void resetQuitRequest();
    
    static bool key(SDL_Keycode keycode);
    static bool button(uint8_t buttoncode);
    static util::Point mouse();
    static util::Point mouseDown();
    static util::Point mouseUp();
    
    static util::Connection connect(uint32_t eventType, const std::function<void(const util::Event&)>& callback);
};

} // namespace engine
} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
