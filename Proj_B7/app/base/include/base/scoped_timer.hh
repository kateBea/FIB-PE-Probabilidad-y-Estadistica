//
// Created by zanet on 10/7/2025.
//

#ifndef SCOPED_TIMER_HH
#define SCOPED_TIMER_HH

#include <chrono>
#include <string>
#include <string_view>

namespace base {

    class scoped_timer {
    public:
        explicit scoped_timer(std::string_view name = "", bool print_on_destruction = false);
        ~scoped_timer();

        // Non-copyable
        scoped_timer(const scoped_timer&) = delete;
        scoped_timer& operator=(const scoped_timer&) = delete;

        // Movable
        scoped_timer(scoped_timer&&) noexcept = default;
        scoped_timer& operator=(scoped_timer&&) noexcept = default;

        // Timer management
        auto reset() -> void;
        auto elapsed() const -> double;
        auto name() const -> std::string_view;


    private:
        std::string m_name{};
        bool m_print_on_destruction{};
        std::chrono::high_resolution_clock::time_point m_start{};
    };

#define START_SCOPED_TIMER(name) base::scoped_timer timer##__LINE__{name, true};

}



#endif //SCOPED_TIMER_HH
