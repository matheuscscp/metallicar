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
#include "FieldTable.hpp"

namespace metallicar {

class Game {
  protected:
    observer::Connection quitEventConnection;
    
    Game();
    virtual ~Game();
    virtual void update();
    virtual void render();
    
    static Game& runningInstance();
  public:
    static void init();
    static void close();
    static void run();
    
    static void addRenderer(double z, const std::function<void()>& renderer);
    static void quit();
    
    static float dt();
    static void setUpdateStep(uint32_t step);
    
    static float fps();
};

class GameObject {
  protected:
    GameObject();
  public:
    virtual ~GameObject();
    virtual void update();
    virtual void render();
    virtual bool dead();
};

class GameObjectScene : public Game {
  private:
    std::list<std::shared_ptr<GameObject>> objects;
    std::list<std::shared_ptr<GameObject>> newObjects;
    FieldTable fieldTable;
  protected:
    ~GameObjectScene();
  private:
    void update();
    void render();
  public:
    void addObjects(const std::vector<std::shared_ptr<GameObject>>& objects);
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
      protected:
        virtual std::string family() const;
        virtual std::vector<std::string> depends() const;
        virtual void init();
        virtual void update();
        virtual bool destroy();
    };
  private:
    std::list<std::unique_ptr<Component>> components;
    std::list<std::unique_ptr<Component>> newComponents;
    FieldTable fieldTable;
  public:
    ComponentGameObject(const std::vector<Component*>& components);
    ~ComponentGameObject();
    void update();
    void render();
    bool dead();
    
    void addComponents(const std::vector<Component*>& components);
    FieldTable& fields();
};

class CompositeGameObject : public GameObject {
  private:
    std::list<std::shared_ptr<GameObject>> objects;
    std::list<std::shared_ptr<GameObject>> newObjects;
    FieldTable fieldTable;
  public:
    CompositeGameObject(
      const std::vector<std::shared_ptr<GameObject>>& objects
    );
    ~CompositeGameObject();
    void update();
    void render();
    bool dead();
    
    void addObjects(const std::vector<std::shared_ptr<GameObject>>& objects);
    FieldTable& fields();
};

} // namespace metallicar

#endif /* METALLICAR_CORE_HPP_ */
