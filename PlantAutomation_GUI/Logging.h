#ifndef LOGGING_H
#define LOGGING_H

#include <fstream>
#include <ctime>
#include <iostream>
#include <string>

#pragma once

/**
 * @brief The Logger class provides a simple logging mechanism.
*/
class Logger {
public:

    /**
     * @brief Constructs a Logger instance with an optional file name.
     * @param fileName The name of the log file. Defaults to "log.csv".
     */
    Logger(const std::string& fileName = "log.csv");

    /**
     * @brief Logs an event with the specified log level, event name, and details.
     * @param logLevel The log level of the event.
     * @param eventName The name of the event.
     * @param details Additional details about the event.
     * @return The constructed log entry as a string.
     */
    std::string logEvent(const std::string& logLevel, const std::string& eventName, const std::string& details);

private:

    /**
     * @brief The file stream used for logging.
     */
    std::ofstream file;
};

extern Logger logger;

#endif // LOGGING_H


