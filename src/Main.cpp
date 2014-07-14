/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include "Game.hpp"

class FirstState : public metallicar::engine::GameState {
  public:
    
};

int main(int argc, char* argv[]) {
  metallicar::engine::Game::init();
  metallicar::engine::Game::start(new FirstState());
  metallicar::engine::Game::close();
  return 0;
}
