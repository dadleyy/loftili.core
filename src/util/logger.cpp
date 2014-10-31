#include "util/logger.h"

namespace loftili {

bool Logger::use_file = false;
std::ofstream Logger::log_stream;

Logger::Logger(std::string n) {
  name = n;
}

Logger::Logger(Loggable* loggable) { 
  name = loggable->logName();
}

Logger::~Logger() { 
}

void Logger::info(std::string msg) {
  send(msg, "INFO");
}

void Logger::fatal(std::string msg) {
  send(msg, "FATAL");
}

void Logger::send(std::string msg, std::string level) {
  time(&timer);
  std::string time_str = ctime(&timer);
  std::string cleaned = time_str.substr(0, time_str.length() -1);
  if(!use_file) {
    printf("[%s] [%s] [%s]: %s \n", name.c_str(), level.c_str(), cleaned.c_str(), msg.c_str());
  } else {
    int log_length = name.size() + level.size() + cleaned.size() + msg.size() + 11;
    char* buffer = (char*) malloc(sizeof(char) * log_length);
    sprintf(buffer, "[%s] [%s] [%s]: %s \n", name.c_str(), level.c_str(), cleaned.c_str(), msg.c_str());
    log_stream << buffer;
    log_stream.flush();
    free(buffer);
  }
}

LOG_STATE Logger::use(std::string file_name) {
  use_file = true;
  log_stream.open(file_name, std::ofstream::out | std::ofstream::app);
  return log_stream.is_open() && log_stream.good() ? LOG_STATE_READY : LOG_STATE_BAD;
}

void Logger::close() {
  log_stream.close();
}

}

