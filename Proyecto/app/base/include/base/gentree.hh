//
// Created by zanet on 10/7/2025.
//

#ifndef GENTREE_HH
#define GENTREE_HH

#include <vector>
#include <ranges>
#include <utility>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>

#include <base/logger.hh>
namespace base {

    static inline std::mutex g_print_mutex{};

    template<typename T>
    class gentree {
    public:
        using value_type = T;
        using edge_type = std::pair<value_type, value_type>;
        using vertex_list_type = std::unordered_set<value_type>;
        using adjacency_list_type = std::unordered_map<T, vertex_list_type>;

        gentree() = default;
        ~gentree() = default;

        // añade un nuevo vertice
        auto add_vertex(const T& v) -> void {
            m_adjacency[v]; 
        }

        // Si from no existe se crea primero
        auto add_edge(const T& from, const T& to) -> void {
            if ( from == to ) {
                //LOG_WARN("Ignoring self-loop edge from {} to {}", from, to );
                return;
            }

            m_adjacency[from].insert(to);

            // Aseguro que el nodo destino también existe
            if ( !m_adjacency.contains(to)) {
                m_adjacency[to] = vertex_list_type{};
            }
        }

        auto vertices() const -> vertex_list_type {
            vertex_list_type keys{};

            keys.reserve(m_adjacency.size());
            for (auto& key : m_adjacency | std::ranges::views::keys) {
                keys.push_back(key);
            }

            return keys;
        }

        auto edges() const -> vertex_list_type {
            vertex_list_type result{};

            for (auto& [from, list] : m_adjacency) {
                for (auto& to : list) {
                    result.emplace_back(from, to);
                }
            }

            return result;
        }

        auto node_count() const -> std::size_t {
            return m_adjacency.size();
        }

        auto edge_count() const -> std::size_t {
            std::size_t count{ 0 };

            for (const auto& adjacent_nodes : m_adjacency | std::views::values ) {
                count += adjacent_nodes.size();
            }

            return count;
        }

        auto density() const -> double {
            if (m_adjacency.empty()) {
                return 0.0;
            }

            auto v_count{ static_cast<double>(m_adjacency.size()) };
            auto e_count{ 0.0 };

            for (const auto& edges : m_adjacency | std::views::values ) {
                e_count += edges.size();
            }

            // Multiplico por 1 porque el grafo es no dirigido
            return static_cast<double>(e_count) * 1 / (v_count * (v_count - 1.0));
        }

        auto debug_print() const {
            // This uses satd::cout which is not thread safe
            //std::lock_guard lock{ g_print_mutex };

            if ( m_debug_name.empty() ) {
                print("\nGraph Debug Print:\n");
            } else {
                print("\nGraph Debug Print [{}]:\n", m_debug_name );
            }

            for (const auto& [v, edges] : m_adjacency) {
                print("{} -> ", v);

                for (const auto& e : edges) {
                    print("{} ", e);
                }
                
                print("\n");
            }
        }

        auto serialize( std::ostream& os ) const -> void {
            // Write vertices
            for (const auto& [v, _] : m_adjacency) {
                os << v << "\n";
            }

            // Write edges
            for (const auto& [from, adjancecy_list] : m_adjacency) {
                for (const auto& to : adjancecy_list) {
                    os << from << " " << to << "\n";
                }
            }
        }

        auto set_debug_name( std::string_view name ) -> void {
            m_debug_name = name;
        }

        auto get_debug_name() const -> const std::string& {
            return m_debug_name;
        }

        auto get_graph() const -> const adjacency_list_type& {
            return m_adjacency;
        }

        auto get_nodes() const -> vertex_list_type {
            vertex_list_type keys{};

            keys.reserve(m_adjacency.size());

            for (const auto& key : m_adjacency | std::views::keys) {
                keys.insert(key);
            }

            return keys;
        }

    private:
        std::string m_debug_name{};

        adjacency_list_type m_adjacency{};
    };
}



#endif //GENTREE_HH
