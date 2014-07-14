/*
 * Log.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef LOG_HPP_
#define LOG_HPP_

// standard
#include <string>

namespace metallicar {
namespace util {

class Log {
  public:
    enum MessageLevel {
      Error = 0,
      Warning
    };
    
    static void message(MessageLevel level, const std::string& msg);
};

} // namespace util
} // namespace metallicar

#endif /* LOG_HPP_ */
