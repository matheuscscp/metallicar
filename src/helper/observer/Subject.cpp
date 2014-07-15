/*
 * Subject.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "observer.hpp"

using namespace std;

namespace observer {

Connection Subject::connect(uint32_t eventType, const function<void(const Event&)>& callback) {
  list<Observer>& eventObservers = observers[eventType];
  eventObservers.emplace_back(callback);
  return eventObservers.back().connection;
}

void Subject::broadcast(const Event& event) {
  list<Observer>& eventObservers = observers[event.getType()];
  for (auto it = eventObservers.begin(); it != eventObservers.end();) {
    if (it->connection.connected()) {
      it->callback(event);
      it++;
    }
    else {
      eventObservers.erase(it++);
    }
  }
}

} // namespace observer
