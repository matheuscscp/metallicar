/*
 * Observer.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "observer.hpp"

using namespace std;

namespace observer {

Observer::Observer(const function<void(const Event&)>& callback) :
callback(callback)
{
  
}

} // namespace observer
