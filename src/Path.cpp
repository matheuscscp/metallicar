/*
 * Path.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "Path.hpp"

using namespace std;

static string path;

string Path::get() {
  return path;
}

string Path::get(const string& subpath) {
  if (path.size() && subpath[0] != '/')
    return (path + "/") + subpath;
  return path + subpath;
}

void Path::set(const string& path) {
  ::path = path;
}
