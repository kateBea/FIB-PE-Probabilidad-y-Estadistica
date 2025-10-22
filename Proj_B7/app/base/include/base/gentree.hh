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

#include <base/logger.hh>
namespace base {

    template<typename T>
    class gentree {
    public:
        using value_type = T;
        using edge_type = std::pair<value_type, value_type>;
        using vertex_list_type = std::vector<value_type>;
        using adjacency_list_type = std::unordered_map<T, vertex_list_type>;

        gentree() = default;
        ~gentree() = default;

        // añade un nuevo vertice
        auto add_vertex(const T& v) -> void {
            m_adjacency[v]; 
        }

        // Si from no existe se crea primero
        auto add_edge(const T& from, const T& to) -> void {
            m_adjacency[from].push_back(to);
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
            if (m_adjacency.empty) {
                return 0.0;
            }

            auto v_count{ static_cast<double>(m_adjacency.size()) };
            auto e_count{ 0.0 };

            for (const auto& edges : m_adjacency | std::views::values ) {
                e_count += edges.size();
            }

            // Multiplico por dos porque el grafo es dirigido
            return static_cast<double>(e_count) * 2 / (v_count * (v_count - 1.0));
        }

        auto debug_print() const {
            for (const auto& [v, edges] : m_adjacency) {
                print("{} -> ", v);

                for (const auto& e : edges) {
                    print("{} ", e);
                }
                
                print("\n");
            }
        }

    private:
        adjacency_list_type m_adjacency{};
    };
}



#endif //GENTREE_HH
