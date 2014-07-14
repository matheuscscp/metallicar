/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include "metallicar.hpp"

using namespace metallicar::engine;

class FirstScene : public GameScene {
  protected:
    void update() {
      if (Game::quitRequested()) {
        Game::quit();
      }
    }
    
    void render() {
      
    }
    
    void wakeup(void* args) {
      
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Game::run(new FirstScene());
  return 0;
}
