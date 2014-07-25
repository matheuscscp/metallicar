/*
 * metallicar_core.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_CORE_HPP_
#define METALLICAR_CORE_HPP_

// standard
#include <list>
#include <map>
#include <functional>

// local
#include "metallicar_asset.hpp"
#include "metallicar_io.hpp"

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

class Game {
  public:
    static void init(const WindowOptions& windowOptions = WindowOptions());
    static void run(GameScene* firstScene);
    
    static void changeScene(GameScene* scene);
    static void pushScene(GameScene* scene);
    static void popScene(GameArgs* args);
    static void quit();
    
    static uint32_t getUPS();
    static void setUPS(uint32_t ups);
    static float getDT();
    static uint32_t updateID();
};

} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
