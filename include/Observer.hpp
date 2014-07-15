/*
 * Observer.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

// standard
#include <functional>
#include <memory>
#include <map>
#include <list>

namespace metallicar {
namespace util {

class Event {
  protected:
    int type;
  public:
    Event(int type);
    int getType() const;
};

class Connection {
  private:
    std::shared_ptr<void> token;
  public:
    Connection();
    bool connected();
    void disconnect();
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
