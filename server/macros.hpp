#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <format>      // C++20 std::format
#include <ctime>
#include <filesystem>
#include <mutex>

// ANSI color codes namespace
namespace Colors {
    constexpr auto Reset   = "\033[0m";
    constexpr auto Red     = "\033[31m";
    constexpr auto Green   = "\033[32m";
    constexpr auto Yellow  = "\033[33m";
    constexpr auto Blue    = "\033[34m";
    constexpr auto Magenta = "\033[35m";
    constexpr auto Cyan    = "\033[36m";
    constexpr auto White   = "\033[37m";
}

inline std::mutex log_mutex;

// Extract only filename using std::filesystem
inline std::string get_filename(const char* full_path) {
    return std::filesystem::path(full_path).filename().string();
}

// Format time into "HH:MM:SS.mmm"
inline std::string current_time() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto time_t_now = system_clock::to_time_t(now);

    std::tm time_info{};
    localtime_s(&time_info, &time_t_now);

    std::ostringstream oss;
    oss << std::put_time(&time_info, "%H:%M:%S")
        << "." << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}

// Core logging function
template<typename... Args>
inline void log_base(const char* level, const char* color, std::ostream& stream,
                     const char* file, int line, std::string_view fmt, Args&&... args) 
{
    std::scoped_lock lock(log_mutex);

    stream << current_time()
           << " <" << color << level << ":" << get_filename(file) << ":" << line << Colors::Reset << ">"
           << " " << std::vformat(fmt, std::make_format_args(args...))
           << std::endl;
}

// Convenience macros
#define LOG_INFO(fmt, ...)    log_base("INFO",    Colors::Green,   std::cout, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) log_base("WARNING", Colors::Yellow,  std::cout, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   log_base("ERROR",   Colors::Red,     std::cerr, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)   log_base("DEBUG",   Colors::Blue,    std::cout, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...)   log_base("TRACE",   Colors::Magenta, std::cout, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
