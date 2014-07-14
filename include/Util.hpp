/*
 * Util.hpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

// standard
#include <string>
#include <sstream>

namespace metallicar {
namespace util {

class Path {
  public:
    static std::string get();
    static std::string get(const std::string& subpath);
    static void set(const std::string& path);
};

class Log {
  public:
    enum MessageLevel {
      Error = 0,
      Warning
    };
    
    static void message(MessageLevel level, const std::string& msg);
};

class String {
  public:
    template <typename T> static T to(const std::string& value) {
      std::stringstream ss;
      ss << value;
      T tmp;
      ss >> tmp;
      return tmp;
    }
    
    template <typename T> static std::string from(T value) {
      std::stringstream ss;
      ss << value;
      std::string tmp;
      ss >> tmp;
      return tmp;
    }
};

} // util metallicar
} // namespace metallicar

#endif /* UTIL_HPP_ */
