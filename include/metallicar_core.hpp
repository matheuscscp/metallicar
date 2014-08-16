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
#include "FieldTable.hpp"

namespace metallicar {

class Game {
  private:
    Assets assets;
  protected:
    observer::Connection quitEventConnection;
    
    Game();
    virtual ~Game();
    virtual void update();
    virtual void render();
    
    static Game& runningInstance();
  private:
    static void changeInstance();
  public:
    static void init();
    static void close();
    static void run();
    
    static void addRenderer(double z, const std::function<void()>& renderer);
    static void quit();
    
    static float dt();
    static void setUpdateInterval(uint32_t updateInterval);
    
    static float fps();
};
typedef Game GameScene;

class GameObject {
  public:
    virtual ~GameObject();
    virtual void update();
    virtual void render();
    virtual bool destroy();
};

class GameObjectScene : public GameScene {
  private:
    std::list<GameObject*> objects;
    std::list<GameObject*> newObjects;
    FieldTable fieldTable;
    
    virtual ~GameObjectScene();
    void update();
    void render();
  public:
    void addObjects(const std::vector<GameObject*>& objects);
    FieldTable& fields();
    
    static GameObjectScene& runningInstance();
};

class ComponentGameObject : public GameObject {
  public:
    class Component {
      friend class ComponentGameObject;
      protected:
        ComponentGameObject* object;
      public:
        virtual ~Component();
        virtual std::string family() const;
        virtual std::vector<std::string> depends() const;
        virtual void init();
        virtual void update();
        virtual bool destroy();
    };
  protected:
    std::list<Component*> components;
    std::list<Component*> newComponents;
    FieldTable fieldTable;
  public:
    ComponentGameObject(const std::vector<Component*>& components);
    virtual ~ComponentGameObject();
    virtual void update();
    virtual void render();
    virtual bool destroy();
    
    virtual void addComponents(const std::vector<Component*>& components);
    virtual FieldTable& fields();
};

class CompositeGameObject : public GameObject {
  protected:
    std::list<std::shared_ptr<GameObject>> objects;
  public:
    virtual ~CompositeGameObject();
    virtual void update();
    virtual void render();
    virtual bool destroy();
};

} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
