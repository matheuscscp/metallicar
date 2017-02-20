/*
 * Path.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef PATH_HPP_
#define PATH_HPP_

// standard
#include <string>

class Path {
  public:
    static std::string get();
    static std::string get(const std::string& subpath);
    static void set(const std::string& path);
};

#endif /* PATH_HPP_ */
