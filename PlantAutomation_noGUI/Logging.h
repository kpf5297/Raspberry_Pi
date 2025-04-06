#ifndef LOGGING_H
#define LOGGING_H

#include <fstream>
#include <ctime>
#include <iostream>
#include <string>

class Logger {
public:
    Logger(const std::string& fileName = "log.csv");
    void logEvent(const std::string& eventName, const std::string& details);

private:
    std::ofstream file;
};

#endif // LOGGING_H
