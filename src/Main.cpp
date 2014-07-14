/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include "Game.hpp"

class FirstScene : public metallicar::engine::GameScene {
  public:
    
};

int main(int argc, char* argv[]) {
  metallicar::engine::Game::init();
  metallicar::engine::Game::run(new FirstScene());
  return 0;
}
