/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>

#include "metallicar.hpp"

using namespace std;
using namespace metallicar;

class Space2D : public GameObjectComponent {
  private:
    string family() const {
      return "spatial";
    }
    void init() {
      object->fields().write("x", 640.0f);
      object->fields().write("y", 360.0f);
    }
    void update() {
      
    }
    bool destroy() {
      return false;
    }
};

class Renderer : public GameObjectComponent {
  public:
    Sprite bg, spr, spr2;
    Renderer() : bg("asset/background.png"), spr("asset/metallicar.png"), spr2("asset/icon.png") {
      
    }
  private:
    string family() const {
      return "renderer";
    }
    vector<string> depends() const {
      return {"spatial"};
    }
    void init() {
      
    }
    void update() {
      GameRenderers::add(0.0, [this]() {
        bg.render();
        spr.render(object->fields().read<float>("x"), object->fields().read<float>("y"), Corner::CENTER);
        spr2.render();
      });
    }
    bool destroy() {
      return false;
    }
};

class ProjUpdater : public GameObjectComponent {
  public:
    observer::Connection conn;
    ProjUpdater() {
      conn = Input::connect<Input::KeyDownEvent>([this](const observer::EventBase& event) {
        const Input::KeyDownEvent& keyEvent = (const Input::KeyDownEvent&)event;
        switch (keyEvent.key()) {
          case SDLK_a: {
            WindowOptions opts = Window::getOptions();
            opts.width = 480;
            opts.height = 640;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_s: {
            WindowOptions opts = Window::getOptions();
            opts.width = 800;
            opts.height = 600;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_d: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1024;
            opts.height = 768;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_f: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1280;
            opts.height = 720;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_g: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1920;
            opts.height = 1080;
            Window::setOptions(opts);
            break;
          }
          
          default:
            break;
        }
      });
    }
  private:
    string family() const {
      return "projection";
    }
    void init() {
      
    }
    void update() {
      
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Window::init();
  Graphics::initDefault();
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer, new ProjUpdater})
  });
  Game::run();
  Window::close();
  Game::close();
  return 0;
}
