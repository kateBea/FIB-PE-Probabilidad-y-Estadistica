//
// Created by zanet on 10/7/2025.
//

#include <base/logger.hh>
#include <base/random.hh>
#include <base/scoped_timer.hh>
#include <base/search.hh>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <taskflow/taskflow.hpp>
#include <tester_app.hh>
#include <tree_loader.hh>
#include <unordered_map>
#include <vector>

namespace tester {

    auto tester_app::run( const std::int32_t, char ** ) -> std::int32_t {
        START_SCOPED_TIMER( "tester_run_timer" );

        base::println( base::console_color::Blue, "Hello from tester app!" );

        load_graphs();

        run_tests();

        return 0;
    }

    auto tester_app::load_graphs() -> void {
        tf::Taskflow task_graph{};
        std::string graph_dir{ "../generator/resources" };

        try {
            for ( auto &entry: std::filesystem::directory_iterator( graph_dir ) ) {
                if ( entry.is_regular_file() ) {
                    const std::string filepath{ entry.path().string() };

                    m_graphs.try_emplace( filepath, base::gentree<int>{} );
                    LOG_INFO( "Found graph file: {}", filepath );
                }
            }
        } catch ( const std::exception &e ) {
            LOG_ERROR( "Error accessing graph directory {}: {}", graph_dir, e.what() );
            return;
        }

        for ( const auto &graph_path: m_graphs | std::views::keys ) {
            task_graph.emplace( [path = graph_path, this]() {
                LOG_INFO( "Processing graph: {}", path );

                // Me cargo un arbol de disco
                tree_loader loader{};
                auto tree{ loader.load_from_file( path ) };

                tree.set_debug_name( path );
                tree.debug_print();

                m_graphs[path] = std::move( tree );
            } );
        }

        m_Executor.run( task_graph ).wait();

        for ( const auto &[path, graph]: m_graphs ) {
            m_reports[path] = test_report{
                .graph_name = path,
                .graph_density = graph.density(),
                .graph_nodes = static_cast<std::int32_t>( graph.node_count() ),
                .graph_edges = static_cast<std::int32_t>( graph.edge_count() ),
            };
        }
    }

    auto tester_app::run_tests() -> void {
        tf::Taskflow task_graph{};

        // thread concurrency para ejecutar tests en paralel
        std::int32_t thread_count{ static_cast<std::int32_t>( std::thread::hardware_concurrency() ) };
        std::int32_t block_size{ static_cast<std::int32_t>( m_graphs.size() / thread_count ) };

        LOG_INFO( "Running tests with {} threads, block size: {} for element count {} (NUMBER OG GRAPHS)", thread_count, block_size, m_graphs.size() );

        for ( const auto &[path, current_graph]: m_graphs ) {

            LOG_INFO( "Running test on graph: {}", path );

            // Selecciono nodos aleatorios para buscar
            const auto &nodes{ current_graph.get_nodes() };

            if ( nodes.empty() ) {
                LOG_WARN( "Graph {} does not have enough nodes to perform search tests.", current_graph.get_debug_name() );
                return;
            }

            auto it_start{ std::next( std::begin( nodes ), base::next_int() % nodes.size() ) };
            auto it_end{ std::next( std::begin( nodes ), base::next_int() % nodes.size() ) };

            std::int32_t node_start{ *nodes.begin() };
            std::int32_t node_end{ *nodes.begin() };

            if ( it_start != nodes.end() && it_end != nodes.end() ) {
                node_start = ( std::int32_t )*it_start;
                node_end = ( std::int32_t )*it_end;
            }

            double dfs_time_ms{};
            double bfs_time_ms{};

            base::scoped_timer timer_bfs{ "run_bfs" };
            std::int32_t result_bfs{ base::search_bfs( current_graph, node_start, node_end ) };
            bfs_time_ms = timer_bfs.elapsed();

            base::scoped_timer timer_dfs{ "run_dfs" };
            std::int32_t result_dfs{ base::search_dfs( current_graph, node_start, node_end ) };
            dfs_time_ms = timer_dfs.elapsed();

            m_reports[path].dfs_time_ms = dfs_time_ms;
            m_reports[path].bfs_time_ms = bfs_time_ms;
            m_reports[path].dfs_path_length = result_dfs;
            m_reports[path].bfs_path_length = result_bfs;

            m_reports[path].node_start = node_start;
            m_reports[path].node_end = node_end;

            LOG_INFO( "Graph {} has BFS result: {} for nodes ({}, {})", current_graph.get_debug_name(), result_bfs, node_start, node_end );
            LOG_INFO( "Graph {} has DFS result: {} for nodes ({}, {})", current_graph.get_debug_name(), result_dfs, node_start, node_end );
        }

        m_Executor.run( task_graph ).wait();

        // abrir archivo CSV para escribir resultados
        std::ofstream csv_file{ "test_report.csv" };
        if ( csv_file.is_open() ) {
            csv_file << "Graph Name,Density,Nodes,Edges,DFS Time (ms),BFS Time (ms),DFS Path Length,BFS Path Length,Start Node,End Node\n";
        }

        for ( const auto &[path, report]: m_reports ) {
            // Para depuración pero no es necesario
            LOG_INFO( "Test Report for Graph: {}", path );
            LOG_INFO( "  Density: {}", report.graph_density );
            LOG_INFO( "  Nodes: {}", report.graph_nodes );
            LOG_INFO( "  Edges: {}", report.graph_edges );
            LOG_INFO( "  DFS Time (ms): {}", report.dfs_time_ms );
            LOG_INFO( "  BFS Time (ms): {}", report.bfs_time_ms );
            LOG_INFO( "  DFS Path Length: {}", report.dfs_path_length );
            LOG_INFO( "  BFS Path Length: {}", report.bfs_path_length );

            LOG_INFO( "  BFS Path Length: {}", report.bfs_path_length );
            LOG_INFO( "  DFS Path Length: {}", report.dfs_path_length );

            LOG_INFO( "  BFS start: {}", report.node_start );
            LOG_INFO( "  DFS end: {}", report.node_end );

            // Serializar a CSV
            if ( csv_file.is_open() ) {
                csv_file << std::format( "{},{},{},{},{},{},{},{},{},{}\n",
                                         report.graph_name,
                                         report.graph_density,
                                         report.graph_nodes,
                                         report.graph_edges,
                                         report.dfs_time_ms,
                                         report.bfs_time_ms,
                                         report.dfs_path_length,
                                         report.bfs_path_length,
                                        report.node_start,
                                        report.node_end );
            }
        }
    }
}// namespace tester