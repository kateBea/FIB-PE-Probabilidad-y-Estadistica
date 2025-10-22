//
// Created by zanet on 10/7/2025.
//

#include <base/logger.hh>
#include <base/scoped_timer.hh>

#include <iostream>
#include <tester_app.hh>
#include <tree_loader.hh>

namespace tester {

    auto tester_app::run( const std::int32_t argc, char **argv ) -> std::int32_t {
        START_SCOPED_TIMER( "tester_run_timer" );

        base::println(base::console_color::Blue, "Hello from tester app!");

        // Me cargo un arbol de disco
        tree_loader loader {};
        auto tree{ loader.load_from_file( "./tree_test.txt" ) };

        tree.debug_print();

        return 0;
    }
}