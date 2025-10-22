//
// Created by zanet on 10/7/2025.
//

#ifndef LOGGER_HH
#define LOGGER_HH

#include <iostream>
#include <string>
#include <string_view>
#include <chrono>
#include <format>
#include <mutex>

namespace base {

    enum class console_color {
        Default,
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BrightBlack,
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite
    };

    inline const char* get_ansi_color(console_color color) {
        switch (color) {
            case console_color::Black:         return "\033[30m";
            case console_color::Red:           return "\033[31m";
            case console_color::Green:         return "\033[32m";
            case console_color::Yellow:        return "\033[33m";
            case console_color::Blue:          return "\033[34m";
            case console_color::Magenta:       return "\033[35m";
            case console_color::Cyan:          return "\033[36m";
            case console_color::White:         return "\033[37m";
            case console_color::BrightBlack:   return "\033[90m";
            case console_color::BrightRed:     return "\033[91m";
            case console_color::BrightGreen:   return "\033[92m";
            case console_color::BrightYellow:  return "\033[93m";
            case console_color::BrightBlue:    return "\033[94m";
            case console_color::BrightMagenta: return "\033[95m";
            case console_color::BrightCyan:    return "\033[96m";
            case console_color::BrightWhite:   return "\033[97m";
            default:                   return "\033[0m";
        }
    }

    enum class log_level {
        trace,
        info,
        warn,
        error,
        critical
    };

    template<typename... Args>
    inline void println(const std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...)) << '\n';
    }

    template<typename... Args>
    inline void print(const std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...));
    }

    template<typename... Args>
    inline void print(console_color color, const std::format_string<Args...> fmt, Args&&... args) {
        std::string_view ansi{ get_ansi_color(color) };
        std::cout << ansi
            << std::vformat(fmt.get(), std::make_format_args(args...))
            << "\033[0m"; // Resetear color de la consola
    }

    template<typename... Args>
    inline void println(console_color color, const std::format_string<Args...> fmt, Args&&... args) {
        std::string_view ansi{ get_ansi_color(color) };
        std::cout << ansi
            << std::vformat(fmt.get(), std::make_format_args(args...))
            << "\033[0m" << '\n'; // Resetear color de la consola
    }

    class logger {
    public:
        // Loggea el mensaje sin cambios
        static auto log(log_level level, std::string_view message) -> void;

        // Permite formatear el mensaje antes de loggear
        // cppreference para ver formato de std::format: https://en.cppreference.com/w/cpp/utility/format/format
        template<typename... Args>
        static auto logf(log_level level, std::string_view fmt, Args&&... args) -> void {
            log(level, std::vformat(fmt, std::make_format_args(args...)));
        }
    };
}

#define LOG_TRACE(...)    base::logger::logf(base::log_level::trace, __VA_ARGS__)
#define LOG_INFO(...)     base::logger::logf(base::log_level::info, __VA_ARGS__)
#define LOG_WARN(...)     base::logger::logf(base::log_level::warn, __VA_ARGS__)
#define LOG_ERROR(...)    base::logger::logf(base::log_level::error, __VA_ARGS__)
#define LOG_CRITICAL(...) base::logger::logf(base::log_level::critical, __VA_ARGS__)


#endif //LOGGER_HH
