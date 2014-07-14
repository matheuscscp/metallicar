/*
 * Game.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef GAME_HPP_
#define GAME_HPP_

namespace metallicar {
namespace engine {

class Game {
  public:
    Game();
    virtual ~Game();
    
    void run();
};

} // engine metallicar
} // namespace metallicar

#endif /* GAME_HPP_ */
