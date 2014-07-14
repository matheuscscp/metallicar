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

class Logger {
  public:
    enum Level {
      ERROR = 0,
      WARNING
    };
    
    static void log(Level level, const std::string& msg);
};

class String {
  public:
    template <typename T> T to(const std::string& value) {
      std::stringstream ss;
      ss << value;
      T tmp;
      ss >> tmp;
      return tmp;
    }
    
    template <typename T> std::string from(T value) {
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
