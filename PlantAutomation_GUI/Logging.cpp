// Logging.cpp

#include <Logging.h>

/**
 * @brief Constructs a Logger instance with an optional file name.
 * @param fileName The name of the log file. Defaults to "log.csv".
 */
Logger::Logger(const std::string& fileName) : file(fileName, std::ios::app) {}

/**
 * @brief Logs an event with the specified log level, event name, and details.
 * @param logLevel The log level of the event.
 * @param eventName The name of the event.
 * @param details Additional details about the event.
 * @return The constructed log entry as a string.
 */
std::string Logger::logEvent(const std::string& logLevel, const std::string& eventName, const std::string& details) {
    if (file.is_open()) {

        // Get the current time
        std::time_t currentTime = std::time(nullptr);
        struct std::tm* timeInfo = std::localtime(&currentTime);

        // Format the log entry as a comma-separated value
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        std::string logEntry = buffer;

        logEntry += "," + logLevel + "," + eventName + "," + details + "\r\n";

        // Write the log entry to the file
        file << logEntry;

        // Flush the stream to make sure the data is written immediately
        file.flush();

        // Return the constructed log entry
        return logEntry;

    } else {

        // Handle the case where the file could not be opened
        std::cout << "Failed to open log file for writing\n";
        return ""; // or some other appropriate value if needed
    }
}

