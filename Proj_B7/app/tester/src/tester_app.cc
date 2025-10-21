//
// Created by zanet on 10/7/2025.
//

#include <base/scoped_timer.hh>
#include <iostream>
#include <tester_app.hh>

namespace tester {

    auto tester_app::run( const std::int32_t argc, char **argv ) -> std::int32_t {
        START_SCOPED_TIMER( "tester_run_timer" );

        std::cout << "Hello from tester app!" << std::endl;

        return 0;
    }
}