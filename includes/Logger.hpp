//
// Created by codingwithjamal on 1/7/2025.
//

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <ctime>

#include "constants.hpp"

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
public:
    // Singleton instance getter
    static Logger& getInstance();

    // Delete copy constructor and assignment operator to enforce singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Logging function
    void log(LogLevel level, const std::string& message);

    // Set log file name (default is "log.txt")
    void setLogFile(const std::string& filename);

private:
    Logger(); // Private constructor for singleton
    ~Logger();

    std::ofstream logFile;
    std::mutex logMutex; // To make logging thread-safe
    bool consoleLoggingEnabled = Config::disableConsoleLogging;

    std::string getTimestamp() const;
    std::string logLevelToString(LogLevel level) const;
};