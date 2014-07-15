/*
 * Log.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "Log.hpp"

// standard
#include <ctime>
#include <cstdio>

// local
#include "Path.hpp"

using namespace std;

static const char* levelmsg[] = {
  "Error",
  "Warning"
};

void Log::message(MessageLevel level, const string& msg) {
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
