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

void Subject::broadcast(const EventBase& event) {
  list<Observer>& eventObservers = observers[event.eventID()];
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
