#include <Logging.h>

Logger::Logger(const std::string& fileName) : file(fileName, std::ios::app) {}

void Logger::logEvent(const std::string& eventName, const std::string& details) {
    if (file.is_open()) {
        // Get the current time
        std::time_t currentTime = std::time(nullptr);
        struct std::tm* timeInfo = std::localtime(&currentTime);

        // Format the log entry as a comma-separated value
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        std::string logEntry = buffer;

        logEntry += "," + eventName + "," + details + "\n";

        // Write the log entry to the file
        file << logEntry;

        // Flush the stream to make sure the data is written immediately
        file.flush();
    } else {
        // Handle the case where the file could not be opened
        std::cout << "Failed to open log file for writing\n";
    }
}
