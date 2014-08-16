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
#include <cstdio>

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
      return ss.str();
    }
    
    template <typename... Args>
    static std::string from(const char* fmt, Args&&... args) {
      for (int scale = 1; ; scale *= 2) {
        size_t size = 512*scale;
        char* buf = new char[size];
        if (snprintf(buf, size, fmt, std::forward<Args>(args)...) < size) {
          std::string tmp(buf);
          delete[] buf;
          return tmp;
        }
        else {
          delete[] buf;
        }
      }
    }
};

#endif /* STRING_HPP_ */
