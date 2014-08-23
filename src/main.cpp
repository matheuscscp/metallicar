/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>
#include <cmath>

#include "metallicar.hpp"
#include "String.hpp"

using namespace std;
using namespace metallicar;
using namespace geometry;

class Space2D : public ComponentGameObject::Component {
  public:
    string family() const {
      return "spatial";
    }
    void init() {
      object->fields().write("pos", Point2(640.0f, 360.0f));
      object->fields().write("spr2.pos", Point2(640.0f, 360.0f));
    }
};

class Renderer : public ComponentGameObject::Component {
  public:
    TextureRenderer2D bg, spr, spr2;
    observer::Connection keyDownConn;
    bool flipH, flipV;
    float angle;
    Renderer() :
    bg(Image::createTexture("asset/background.png")),
    spr(Image::createTexture("asset/metallicar.png")),
    spr2(Image::createTexture("asset/icon.png")),
    flipH(false),
    flipV(false),
    angle(0.0f)
    {
      spr.setSpot(geometry::Rectangle::Spot::CENTER);
      spr2.setSpot(geometry::Rectangle::Spot::BOTTOM_RIGHT);
      spr2.setScale(Point2(0.2,0.2));
      spr2.setAngle(45);
      keyDownConn = Input::connect<Input::KeyDownEvent>([this](const observer::EventBase& event) {
        Input::KeyDownEvent& keyEvent = (Input::KeyDownEvent&)event;
        if (keyEvent.key() == SDLK_h) {
          flipH = !flipH;
          spr2.setFlip(flipH, flipV);
        }
        else if (keyEvent.key() == SDLK_v) {
          flipV = !flipV;
          spr2.setFlip(flipH, flipV);
        }
        else if (keyEvent.key() == SDLK_z) {
          angle += 10.0f;
          spr2.setAngle(angle);
        }
      });
    }
    vector<string> depends() const {
      return {"spatial"};
    }
    void update() {
      spr.setPosition(object->fields().read<Point2>("pos"));
      Point2 spr2pos = object->fields().read<Point2>("spr2.pos");
      Point2 spr2speed;
      if (Input::key(SDLK_UP)) {
        spr2speed.y -= 1.0f;
      }
      if (Input::key(SDLK_DOWN)) {
        spr2speed.y += 1.0f;
      }
      if (Input::key(SDLK_LEFT)) {
        spr2speed.x -= 1.0f;
      }
      if (Input::key(SDLK_RIGHT)) {
        spr2speed.x += 1.0f;
      }
      float speedLength = sqrt(spr2speed.x*spr2speed.x + spr2speed.y*spr2speed.y);
      if (speedLength > 0.0f) {
        spr2speed.x /= speedLength;
        spr2speed.y /= speedLength;
      }
      float speed = 150.0f;
      spr2pos.x += speed*Game::dt()*spr2speed.x;
      spr2pos.y += speed*Game::dt()*spr2speed.y;
      spr2.setPosition(spr2pos);
      object->fields().write("spr2.pos", spr2pos);
      Game::addRenderer(0.0, [this]() {
        bg.render();
        spr.render();
        spr2.render();
      });
    }
};

class ProjUpdater : public ComponentGameObject::Component {
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
            opts.fullscreen = false;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_s: {
            WindowOptions opts = Window::getOptions();
            opts.width = 800;
            opts.height = 600;
            opts.title = "Vish";
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_d: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1024;
            opts.height = 768;
            opts.title = "Ueh";
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
            opts.fullscreen = true;
            Window::setOptions(opts);
            break;
          }
          
          default:
            break;
        }
      });
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Window::init();
  Graphics::initDefaultFunctions();
  
  GameObjectScene* scene = new GameObjectScene();
  
  shared_ptr<GameObject> player(new ComponentGameObject({
    new Space2D, new Renderer, new ProjUpdater
  }));
  shared_ptr<GameObject> moon(new CompositeGameObject({player}));
  scene->addObjects({player, moon});
  
  Game::run();
  
  Window::close();
  Game::close();
  return 0;
}
