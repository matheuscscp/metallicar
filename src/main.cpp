/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>
#include <cmath>
#include <list>

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
    shared_ptr<Audio> audio;
    observer::Connection keydown;
    void init() {
      audio = Audio::playBGM("asset/bird.ogg");
      object->fields().write("pos", Point2(640.0f, 360.0f));
      keydown = Input::connect<Input::KeyDownEvent>([this](const observer::EventBase& event) {
        auto& keyevent = (Input::KeyDownEvent&)event;
        switch (keyevent.key()) {
          case SDLK_p:
            audio->pause();
            break;
            
          case SDLK_r:
            audio->resume();
            break;
            
          case SDLK_t:
            audio->stop();
            break;
            
          case SDLK_UP:
            audio->setVolume(audio->getVolume() + 0.1);
            printf("vol: %.2f\n", audio->getVolume());
            fflush(stdout);
            break;
            
          case SDLK_DOWN:
            audio->setVolume(audio->getVolume() - 0.1);
            printf("vol: %.2f\n", audio->getVolume());
            fflush(stdout);
            break;
            
          default:
            break;
        }
      });
    }
};

class Renderer : public ComponentGameObject::Component {
  public:
    TextureRenderer2D bg, spr, spr2, texto;
    observer::Connection keyDownConn;
    bool flipH, flipV;
    float angle;
    Renderer() :
    bg(Image::getTexture("asset/background.png")),
    spr(Image::getTexture("asset/metallicar.png")),
    spr2(Image::getTexture("asset/icon.png")),
    texto(Font::getFont("asset/font.ttf", 70)->render("HUE", 0, true)),
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
    list<Point2> positions;
    void update() {
      const int max = 20;
      if (positions.size() == 0) {
        for (int i = 0; i < max; i++) {
          positions.push_back(Input::mouse());
        }
      }
      positions.pop_front();
      positions.push_back(Input::mouse());
      
      bg.render(0);
      spr.setPosition(object->fields().read<Point2>("pos"));
      spr.render(0);
      int i = 1;
      for (auto& pos : positions) {
        spr2.setOpacity(pow((i++)/float(max), 4));
        spr2.setPosition(pos);
        spr2.render(0);
      }
      texto.setPosition(Point2(50,50));
      texto.setColor(Color(0x7F0000));
      texto.setOpacity(0.5);
      texto.render(0);
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

bool testeSem() {
  Semaphore s(0);
  Mutex printMutex;
  
  Thread ts[4];
  for (auto& t : ts) {
    t = Thread([&]() {
      printMutex.run([]() { printf("entrou\n"); });
      s.wait();
      printMutex.run([]() { printf("saiu\n"); });
    });
    t.start();
  }
  
  int total = 0;
  while (true) {
    int tmp;
    scanf("%d", &tmp);
    if (tmp == 0) {
      s.post();
      total++;
      if (total == 4) {
        break;
      }
    }
  }
  
  for (auto& t : ts) {
    t.join();
  }
  
  return true;
}

bool testeCond() {
  Condition c;
  Mutex printMutex;
  
  Thread ts[4];
  for (auto& t : ts) {
    t = Thread([&]() {
      printMutex.run([]() { printf("entrou\n"); });
      c.wait();
      printMutex.run([]() { printf("saiu\n"); });
    });
    t.start();
  }
  
  while (true) {
    int tmp;
    scanf("%d", &tmp);
    if (tmp == 0) {
      c.signal();
      break;
    }
  }
  
  for (auto& t : ts) {
    t.join();
  }
  
  return true;
}

int main(int argc, char* argv[]) {
  Game::init();
  if (testeCond()) {
    Game::close();
    return 0;
  }
  
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
