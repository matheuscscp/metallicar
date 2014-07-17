/*
 * observer.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

// standard
#include <memory>
#include <functional>
#include <map>
#include <list>

namespace observer {

#define SUBJECT \
protected: \
  observer::Subject subject; \
public: \
  template <class T> \
  observer::Connection connect(const std::function<void(const observer::EventBase&)>& callback) { \
    return subject.connect<T>(callback); \
  }

class Connection {
  private:
    std::shared_ptr<void> token;
  public:
    Connection();
    bool connected();
    void disconnect();
};

class EventBase {
  private:
    static int nextID;
  public:
    virtual ~EventBase();
    virtual int eventID() const = 0;
    template <class T> static int ID() {
      static int id = nextID++;
      return id;
    }
};

template <class T>
class Event : public EventBase {
  public:
    int eventID() const {
      return ID<T>();
    }
};

struct Observer {
  Connection connection;
  std::function<void(const EventBase&)> callback;
  Observer(const std::function<void(const EventBase&)>& callback);
};

class Subject {
  private:
    std::map<int, std::list<Observer>> observers;
  public:
    template <class T>
    Connection connect(const std::function<void(const EventBase&)>& callback) {
      auto& eventObservers = observers[EventBase::ID<T>()];
      eventObservers.emplace_back(callback);
      return eventObservers.back().connection;
    }
    void broadcast(const EventBase& event);
};

} // namespace observer

#endif /* OBSERVER_HPP_ */
