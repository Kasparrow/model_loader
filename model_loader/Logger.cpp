#include "Logger.h"

LogWindow Logger::_window;

const std::map<LogType, std::string> Logger::_log_labels = {
    { LogType::ERROR, "error"},
    { LogType::WARNING, "warning" },
    { LogType::INFO, "info"}
};

void Logger::add_entry(LogType type, const std::string& message)
{
    std::string entry = "[" + _log_labels.at(type) + "] " + message + "\n";
    
    _window.add_line(entry.c_str());
}

void Logger::render_ui()
{
    _window.draw();
}