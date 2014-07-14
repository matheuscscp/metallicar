/*
 * Logger.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "Util.hpp"

// standard
#include <ctime>
#include <cstdio>

using namespace std;

namespace metallicar {
namespace util {

static const char* levelmsg[] = {
  "Error",
  "Warning"
};

void Logger::log(Level level, const string& msg) {
  string fullmsg;
  fullmsg += levelmsg[level];
  fullmsg += " on ";
  { time_t rawtime = time(nullptr); fullmsg += ctime(&rawtime); }
  fullmsg += ": ";
  fullmsg += msg;
  fullmsg += "\n";
  
  // logging to standard output
  printf("%s", fullmsg.c_str());
  fflush(stdout);
  
  // logging to log file
  FILE* fp = fopen(Path::get("metallicar_log.txt").c_str(), "a");
  fprintf(fp, "%s", fullmsg.c_str());
  fclose(fp);
}

} // namespace util
} // namespace metallicar
