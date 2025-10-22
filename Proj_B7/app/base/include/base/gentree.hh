//
// Created by zanet on 10/7/2025.
//

#ifndef GENTREE_HH
#define GENTREE_HH

#include <vector>
#include <ranges>
#include <utility>
#include <unordered_map>

#include <base/logger.hh>
namespace base {

    template<typename T>
    class gentree {
    public:
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

        auto vertices() const -> std::vector<T> {
            std::vector<T> keys{};

            keys.reserve(m_adjacency.size());
            for (auto& key : m_adjacency | std::ranges::views::keys) {
                keys.push_back(key);
            }

            return keys;
        }

        auto edges() const -> std::vector<std::pair<T, T>> {
            std::vector<std::pair<T, T>> result{};

            for (auto& [from, list] : m_adjacency) {
                for (auto& to : list) {
                    result.emplace_back(from, to);
                }
            }

            return result;
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
        std::unordered_map<T, std::vector<T>> m_adjacency{};
    };
}



#endif //GENTREE_HH
