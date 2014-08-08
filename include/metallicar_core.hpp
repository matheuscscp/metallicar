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
#include <functional>
#include <vector>

// local
#include "metallicar_asset.hpp"
#include "metallicar_io.hpp"
#include "FieldTable.hpp"

namespace metallicar {

class Game {
  public:
    static void init(const WindowOptions& windowOptions = WindowOptions());
    static void run();
    
    static void quit();
    
    static uint32_t getUPS();
    static void setUPS(uint32_t ups);
    static float getDT();
    static uint32_t updateID();
};

class GameScene {
  private:
    Assets assets;
  protected:
    observer::Connection quitEventConnection;
    
    GameScene();
  public:
    virtual ~GameScene();
    virtual void update() = 0;
    virtual void render() = 0;
    
    static GameScene& instance();
    static void change();
    static void close();
};

class GameRenderers {
  public:
    static void add(double order, const std::function<void()>& renderer);
    static void render();
    static void clear();
};

class GameObject {
  public:
    virtual ~GameObject();
    virtual void update() = 0;
    virtual void render() = 0;
    virtual bool destroy() = 0;
};

class GameObjectScene : public GameScene {
  protected:
    std::list<GameObject*> objects;
    std::list<GameObject*> newObjects;
    FieldTable fieldTable;
  public:
    virtual ~GameObjectScene();
    virtual void update();
    virtual void render();
    
    virtual void addObjects(const std::vector<GameObject*>& objects);
    virtual FieldTable& fields();
    
    static GameObjectScene& instance();
};

class CompositeGameObject;
class GameObjectComponent {
  friend class CompositeGameObject;
  protected:
    CompositeGameObject* object;
  public:
    virtual ~GameObjectComponent();
    virtual std::string family() const = 0;
    virtual std::vector<std::string> depends() const;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual bool destroy() = 0;
};

class CompositeGameObject : public GameObject {
  protected:
    std::list<GameObjectComponent*> components;
    std::list<GameObjectComponent*> newComponents;
    FieldTable fieldTable;
  public:
    CompositeGameObject(const std::vector<GameObjectComponent*>& components);
    virtual ~CompositeGameObject();
    virtual void update();
    virtual void render();
    virtual bool destroy();
    
    virtual void addComponents(
      const std::vector<GameObjectComponent*>& components
    );
    virtual FieldTable& fields();
};

} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
