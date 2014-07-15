/*
 * Observer.hpp
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

namespace metallicar {
namespace util {

#define SUBJECT \
protected: \
  metallicar::util::Subject subject; \
public: \
  metallicar::util::Connection connect(int eventType, const std::function<void(const metallicar::util::Event&)>& callback) { \
    return subject.connect(eventType, callback); \
  }

class Connection {
  private:
    std::shared_ptr<void> token;
  public:
    Connection();
    bool connected();
    void disconnect();
};

class Event {
  protected:
    int type;
  public:
    Event(int type);
    int getType() const;
};

struct Observer {
  Connection connection;
  std::function<void(const Event&)> callback;
  Observer(const std::function<void(const Event&)>& callback);
};

class Subject {
  private:
    std::map<int, std::list<Observer>> observers;
  public:
    Connection connect(int eventType, const std::function<void(const Event&)>& callback);
    void broadcast(const Event& event);
};

} // namespace util
} // namespace metallicar

#endif /* OBSERVER_HPP_ */
