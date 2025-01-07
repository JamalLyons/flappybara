//
// Created by codingwithjamal on 1/7/2025.
//

#include "Logger.hpp"
#include <iostream>
#include <iomanip>

// Singleton instance getter
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Constructor
Logger::Logger() : consoleLoggingEnabled(true) {
    setLogFile("log.txt");
}

// Destructor
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Set log file name
void Logger::setLogFile(const std::string& filename) {
    std::lock_guard lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

// Log a message
void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard lock(logMutex);

    const std::string timestamp = getTimestamp();
    const std::string levelStr = logLevelToString(level);
    const std::string logEntry = "[" + timestamp + "] [" + levelStr + "] " + message;

    // Write to log file
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
    }

    // Optionally write to console
    if (consoleLoggingEnabled) {
        std::cout << logEntry << std::endl;
    }
}

// Get current timestamp
std::string Logger::getTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm localTime;

    #ifdef _WIN32
        // Windows platform: Use localtime_s
        localtime_s(&localTime, &now);
    #else
        // POSIX platform: Use localtime_r
        localtime_r(&now, &localTime);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Convert log level to string
std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}