#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType
{
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
};

struct LogMessage
{
  LogType type;
  std::string message;
};

class Logger
{
  public: 
    inline static std::vector<LogMessage> messages;
    static void Log(const std::string& message);
    static void Warn(const std::string& message);
    static void Err(const std::string& message);
};

#endif
