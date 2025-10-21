//
// Created by zanet on 10/7/2025.
//

#include "base/scoped_timer.hh"
#include <chrono>
#include <iostream>

namespace base {

    scoped_timer::scoped_timer(std::string_view name)
        : m_name{ std::move(name) },
          m_start{ std::chrono::high_resolution_clock::now() } {}

    scoped_timer::~scoped_timer() {
        const auto end{ std::chrono::high_resolution_clock::now() };
        const auto duration{ std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count() };

        std::cout << "[Timer] "
                  << (m_name.empty() ? "Scope" : m_name)
                  << " took " << duration / 1000.0 << " ms" << std::endl;
    }

} // namespace abse