//
// Created by zanet on 10/7/2025.
//

#ifndef TESTER_APP_HH
#define TESTER_APP_HH

#include <unordered_map>
#include <string>

#include <taskflow/taskflow.hpp>

#include <base/application.hh>
#include <base/gentree.hh>

namespace tester {

    /**
     * Nodo de inicio
     * Nodo destino
     * Tiempo requerido con DFS
     * Tiempo requerido con BFS
     * Distancia camino con DFS (hay camino)
     * Distancia camino con BFS (hay camino)
     * Estructuras de datos usadas para BFS
     * Densidad del grafo
     * Número de nodos del grafo
     * Número de aristas del grafo
     */
    struct test_report {
        std::string graph_name{};
        std::int32_t node_start{};
        std::int32_t node_end{};

        double dfs_time_ms{};
        double bfs_time_ms{};

        std::int32_t dfs_path_length{};
        std::int32_t bfs_path_length{};

        double graph_density{};
        std::int32_t graph_nodes{};
        std::int32_t graph_edges{};
    };

    class tester_app : public base::application {
    public:
        auto run( const std::int32_t argc, char **argv ) -> std::int32_t override;

    private:
        auto load_graphs() -> void;
        auto run_tests() -> void;

        tf::Executor m_Executor{};

        std::unordered_map<std::string, base::gentree<int>> m_graphs{};
        std::unordered_map<std::string, test_report> m_reports{};
    };
}



#endif //TESTER_APP_HH
