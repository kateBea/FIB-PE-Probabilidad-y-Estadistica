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

#include <base/random.hh>
#include <base/logger.hh>
#include <base/gentree.hh>
#include <base/scoped_timer.hh>

#include <config_loader.hh>
#include <generator_app.hh>

namespace gen {

    auto generator_app::run( const std::int32_t , char ** ) -> std::int32_t {
        START_SCOPED_TIMER("generator_run_timer");

        // Directory to store generated graphs
        std::filesystem::path output_dir{ "./resources" };
        std::filesystem::create_directories(output_dir);

        LOG_INFO("Starting graph generation...");

        const std::string config_path{ "../../generator/generator_config.toml" };
        generator_config config{};
        config.load(config_path);

        // Parameters
        const std::int32_t num_graphs{ 30 };       // how many graphs to generate
        const std::int32_t min_nodes{ 20 };        // min nodes per graph
        const std::int32_t max_nodes{ 30 };       // max nodes per graph
        const std::int32_t max_adjacencies_per_node{ 10 }; // max adjacencies per node

        tf::Taskflow taskflow{};

        for (std::int32_t g{}; g < num_graphs; ++g) {
            taskflow.emplace([g, min_nodes, max_nodes, max_adjacencies_per_node, output_dir]() -> void {
                // Tree that we will serialize
                base::gentree<std::int32_t> graph{};
                
                std::vector<std::int32_t> nodes{};
                std::int32_t num_nodes{ base::next_int(min_nodes, max_nodes) };
                for (std::int32_t i{}; i < num_nodes; ++i) {
                    graph.add_vertex(i);
                    nodes.push_back(i);
                }

                // Randomly add edges
                for (std::int32_t u{}; u < num_nodes; ++u) {
                    std::int32_t num_edges{ base::next_int(1, max_adjacencies_per_node) };

                    for (std::int32_t e{}; e < num_edges; ++e) {
                        std::int32_t to_node_index{ base::next_int(0, num_nodes - 1) };
                        graph.add_edge(nodes[u], nodes[to_node_index]);
                    }
                }

                // Create file
                std::filesystem::path file_path{ output_dir / ("graph_" + std::to_string(g) + ".txt") };
                std::ofstream file{ file_path };
                if (!file.is_open()) {
                    LOG_ERROR("Failed to open file {}", file_path.string());
                    return;
                }

                graph.serialize(file);

                LOG_INFO("Generated graph {} with {} nodes, {} edges", file_path.string(), num_nodes, graph.edge_count());
            });
        }

        m_Executor.run(taskflow).wait();

        LOG_INFO("Graph generation complete!");
        return 0;
    }
}