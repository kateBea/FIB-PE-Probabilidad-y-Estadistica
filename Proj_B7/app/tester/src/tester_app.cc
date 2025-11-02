//
// Created by zanet on 10/7/2025.
//


#include <cstdint>
#include <iostream>
#include <filesystem>
#include <ranges>
#include <vector>
#include <unordered_map>

#include <taskflow/taskflow.hpp>

#include <base/logger.hh>
#include <base/scoped_timer.hh>

#include <tester_app.hh>
#include <tree_loader.hh>

namespace tester {

    auto tester_app::run( const std::int32_t , char ** ) -> std::int32_t {
        START_SCOPED_TIMER( "tester_run_timer" );

        base::println(base::console_color::Blue, "Hello from tester app!");

        // Task graph
        tf::Taskflow graph{};
        
        std::string graph_dir{ "../generator/resources" };
        std::unordered_map<std::string, base::gentree<int>> graphs{};

        for ( auto& entry: std::filesystem::directory_iterator( graph_dir ) ) {
            if ( entry.is_regular_file() ) {
                const std::string filepath{ entry.path().string() };

                graphs.try_emplace( filepath, base::gentree<int>{} );
                LOG_INFO("Found graph file: {}", filepath );
            }
        }

        for (const auto& graph_path : graphs | std::views::keys) {
            graph.emplace( [path = graph_path, &graphs](){
                LOG_INFO("Processing graph: {}", path );

                // Me cargo un arbol de disco
                tree_loader loader {};
                auto tree{ loader.load_from_file( path ) };

                tree.set_debug_name( path );
                tree.debug_print();

                graphs[path] = std::move( tree );
            } );
        }

        m_Executor.run( graph ).wait();

        return 0;
    }
}