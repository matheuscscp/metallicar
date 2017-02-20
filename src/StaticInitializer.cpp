/*
 * StaticInitializer.cpp
 *
 *  Created on: Aug 9, 2014
 *      Author: Pimenta
 */

// this
#include "StaticInitializer.hpp"

StaticInitializer::StaticInitializer(const std::function<void()>& callback) {
  callback();
}
