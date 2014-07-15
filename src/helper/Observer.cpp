/*
 * Observer.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "Observer.hpp"

using namespace std;

namespace metallicar {
namespace util {

Event::Event(int type) : type(type) {
  
}

int Event::getType() const {
  return type;
}

Connection::Connection() : token(new uint8_t) {
  
}

bool Connection::connected() {
  return !token.unique();
}

void Connection::disconnect() {
  token = nullptr;
}

Observer::Observer(const function<void(const Event&)>& callback) :
callback(callback)
{
  
}

Connection Subject::connect(int eventType, const function<void(const Event&)>& callback) {
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

} // namespace util
} // namespace metallicar

