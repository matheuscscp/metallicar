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
#include "observer.hpp"

namespace metallicar {

class GameRenderers {
  private:
    std::map<int, std::list<std::function<void()>>> renderers;
  public:
    void add(int z, const std::function<void()>& renderer);
    void render();
};

class GameArgs {
  protected:
    GameArgs();
  public:
    virtual ~GameArgs();
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
    virtual void render(GameRenderers& renderers) = 0;
    virtual void wakeup(const GameArgs& args) = 0;
    void add(GameObject* object);
    void updateTree();
    void renderTree(GameRenderers& renderers);
    void wakeupTree(const GameArgs& args);
};

class GameScene {
  protected:
    Assets assets;
    observer::Connection quitEventConnection;
  public:
    bool frozen;
    bool visible;
  protected:
    GameScene();
  public:
    virtual ~GameScene();
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void wakeup(const GameArgs& args) = 0;
};

class GameObjectScene : public GameScene {
  private:
    std::list<GameObject*> objects;
    std::list<GameObject*> newObjects;
  protected:
    virtual ~GameObjectScene();
    void add(GameObject* object);
  private:
    void update();
    void render();
    void wakeup(const GameArgs& args);
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
    static void popScene(GameArgs* args);
    static void quit();
    
    static WindowOptions getWindowOptions();
    static void setWindowOptions(WindowOptions& windowOptions);
    
    static uint32_t getUPS();
    static void setUPS(uint32_t ups);
    static float getDT();
    static uint32_t updateID();
};

class Input {
  public:
    class KeyEvent : public observer::Event {
      private:
        SDL_Keycode keycode;
      public:
        KeyEvent(uint32_t eventType, SDL_Keycode keycode);
        SDL_Keycode key() const;
    };
    
    class ButtonEvent : public observer::Event {
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
    static Point mouse();
    static Point mouseDown();
    static Point mouseUp();
    
    static observer::Connection connect(uint32_t eventType, const std::function<void(const observer::Event&)>& callback);
};

} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
