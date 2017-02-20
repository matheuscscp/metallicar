/*
 * StaticInitializer.hpp
 *
 *  Created on: Aug 9, 2014
 *      Author: Pimenta
 */

#ifndef STATICINITIALIZER_HPP_
#define STATICINITIALIZER_HPP_

// standard
#include <functional>

class StaticInitializer {
  public:
    StaticInitializer(const std::function<void()>& callback);
};

#endif /* STATICINITIALIZER_HPP_ */
