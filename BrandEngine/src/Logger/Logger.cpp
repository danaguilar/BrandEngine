#include "Logger.h"
#include <time.h>
#include <string>
#include <iostream>

std::string CurrentDateTimeToString()
{
  time_t currentTime = time(NULL);
  struct tm dateTime;
  localtime_s(&dateTime, &currentTime);
  char output[26];
  strftime(output, sizeof output, "%x %X", &dateTime);
  return std::string(output);
}

void Logger::Log(const std::string& message)
{
  LogMessage logMessage;
  logMessage.type = LOG_INFO;
  logMessage.message = "LOG [" + CurrentDateTimeToString() + "]: " + message;
  std::cout << "\033[92m" << logMessage.message << "\033[0m" << std::endl;
  messages.push_back(logMessage);
}

void Logger::Warn(const std::string& message)
{
  LogMessage logMessage;
  logMessage.type = LOG_INFO;
  logMessage.message = "WARN [" + CurrentDateTimeToString() + "]: " + message;
  std::cout << "\033[93m" << logMessage.message << "\033[0m" << std::endl;
  messages.push_back(logMessage);
}

void Logger::Err(const std::string& message)
{
  LogMessage logMessage;
  logMessage.type = LOG_INFO;
  logMessage.message = "ERR [" + CurrentDateTimeToString() + "]: " + message;
  std::cout << "\033[91m" << logMessage.message << "\033[0m" << std::endl;
  messages.push_back(logMessage);
}
