//
// Created by zanet on 10/7/2025.
//

#include <chrono>
#include <iostream>

#include <base/scoped_timer.hh>
#include <base/logger.hh>

namespace base {

    scoped_timer::scoped_timer(std::string_view name, bool print_on_destruction)
        : m_name{ std::move(name) },
          m_print_on_destruction{ print_on_destruction },
          m_start{ std::chrono::high_resolution_clock::now() } {}

    scoped_timer::~scoped_timer() {
        const auto end{ std::chrono::high_resolution_clock::now() };
        const auto duration{ std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count() };

        if (m_print_on_destruction) {
            LOG_INFO( "[Timer] {} took {} ms", (m_name.empty() ? "Scope" : m_name), duration / 1000.0 );
        }
    }

    auto scoped_timer::reset() -> void {
        m_start = std::chrono::high_resolution_clock::now();
    }

    auto scoped_timer::elapsed() const -> double {
        const auto end{ std::chrono::high_resolution_clock::now() };
        const auto duration{ std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count() };
        return duration / 1000.0;
    }

    auto scoped_timer::name() const -> std::string_view {
        return m_name;
    }
}// namespace base