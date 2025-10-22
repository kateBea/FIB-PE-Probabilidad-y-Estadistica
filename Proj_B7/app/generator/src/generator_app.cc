//
// Created by zanet on 10/7/2025.
//
#include <iostream>

#include <base/random.hh>
#include <base/logger.hh>
#include <base/scoped_timer.hh>
#include <generator_app.hh>
namespace gen {

    auto generator_app::run( const std::int32_t argc, char **argv ) -> std::int32_t {
        START_SCOPED_TIMER( "generator_run_timer" );

        

        std::cout << "Hello from generator app!" << " some random real: " << base::next_real() << std::endl;

        return 0;
    }
}