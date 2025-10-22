//
// Created by zanet on 10/7/2025.
//
#include <cstdint>
#include <iostream>

#include <base/random.hh>
#include <base/logger.hh>
#include <base/scoped_timer.hh>
#include <generator_app.hh>
namespace gen {

    auto generator_app::run( const std::int32_t , char ** ) -> std::int32_t {
        START_SCOPED_TIMER( "generator_run_timer" );

        
        std::int32_t upperbound{ 100 };
        std::int32_t lowerbound{ 10 };

        base::println(base::console_color::Green, "Hello from generator app! some random integer between {} and {}: {}", lowerbound, upperbound, base::next_int(lowerbound, upperbound));

        return 0;
    }
}