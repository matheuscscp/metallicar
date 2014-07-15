/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>
#include "metallicar.hpp"

class FirstScene : public metallicar::GameScene {
  public:
    FirstScene() {
      
    }
  protected:
    void update() {
      
    }
    
    void render() {
      
    }
    
    void wakeup(void* args) {
      
    }
};

int main(int argc, char* argv[]) {
  metallicar::Game::init();
  metallicar::Game::run(new FirstScene());
  return 0;
}
