//
// Created by zanet on 10/7/2025.
//

#include <thread>
#include <functional> // for std::hash

#include "base/logger.hh"

namespace base {
    static std::mutex g_log_mutex;

    auto logger::log(log_level level, std::string_view message) -> void {
        std::lock_guard lock{ g_log_mutex };

        // Formatear tiempo
        const auto now{ std::chrono::system_clock::now() };
        const auto time{ std::chrono::system_clock::to_time_t(now) };
        std::tm local_time{};

    #if defined(_WIN32)
        localtime_s(&local_time, &time);
    #else
        localtime_r(&time, &local_time);
    #endif

        const std::string timestamp{ std::format("{:02}:{:02}:{:02}",
            local_time.tm_hour, local_time.tm_min, local_time.tm_sec) };

        // Formatear nivel traza
        std::string_view level_str{};
        std::string_view color{};

        // Colores consola:
        // https://medium.com/@vitorcosta.matias/print-coloured-texts-in-console-a0db6f589138
        switch (level) {
            case log_level::trace:    level_str = "TRACE";    color = "\033[90m"; break;
            case log_level::info:     level_str = "INFO";     color = "\033[32m"; break;
            case log_level::warn:     level_str = "WARN";     color = "\033[33m"; break;
            case log_level::error:    level_str = "ERROR";    color = "\033[31m"; break;
            case log_level::critical: level_str = "CRITICAL"; color = "\033[1;31m"; break;
        }

        const std::thread::id& tid{ std::this_thread::get_id() };
        std::uint16_t thread_id{ static_cast<std::uint16_t>( std::hash<std::thread::id>{}( tid ) ) };

        println("Thread [{}] {}[{}] [{}] {}{}", thread_id, color, timestamp, level_str, message, "\033[0m");
    }

}
