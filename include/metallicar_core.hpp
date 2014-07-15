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

// local
#include "metallicar_asset.hpp"

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
    
    static WindowOptions getWindowOptions();
    static void setWindowOptions(WindowOptions& windowOptions);
    
    static bool quitRequested();
    static void resetQuitRequest();
};

} // namespace engine
} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */