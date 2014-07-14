/*
 * String.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef STRING_HPP_
#define STRING_HPP_

// standard
#include <string>
#include <sstream>

namespace metallicar {
namespace util {

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

} // namespace util
} // namespace metallicar

#endif /* STRING_HPP_ */
