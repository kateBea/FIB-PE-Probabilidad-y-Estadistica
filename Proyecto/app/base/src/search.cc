
#include <queue>
#include <set>
#include <queue>
#include <stack>
#include <unordered_set>

#include <base/search.hh>
#include <base/gentree.hh>

namespace base {

    // ------------------------------------------------------------
    // EXISTS BFS
    // ------------------------------------------------------------
    auto exists_bfs(const gentree<std::int32_t>& tree, std::int32_t target) -> bool {
        const auto& graph{ tree.get_graph() };
        if (graph.empty()) return false;

        std::unordered_set<std::int32_t> visited {};
        std::queue<std::int32_t> q{};

        // Comença des de tots els nodes (per assegurar-se en grafs disconnexos)
        for (const auto& [start, _] : graph) {
            if (visited.contains(start)) continue;

            q.push(start);
            visited.insert(start);

            while (!q.empty()) {
                auto node = q.front();
                q.pop();

                if (node == target)
                    return true;

                if (auto it{ graph.find(node) }; it != graph.end()) {
                    for (const auto& adj : it->second) {
                        if (!visited.contains(adj)) {
                            visited.insert(adj);
                            q.push(adj);
                        }
                    }
                }
            }
        }
        return false;
    }

    // ------------------------------------------------------------
    // EXISTS DFS
    // ------------------------------------------------------------
    auto exists_dfs(const gentree<std::int32_t>& tree, std::int32_t target) -> bool {
        const auto& graph{ tree.get_graph() };
        if (graph.empty()) return false;

        std::unordered_set<std::int32_t> visited{};
        std::stack<std::int32_t> stack{};

        for (const auto& [start, _] : graph) {
            if (visited.contains(start)) continue;

            stack.push(start);
            visited.insert(start);

            while (!stack.empty()) {
                auto node{ stack.top() };
                stack.pop();

                if (node == target)
                    return true;

                if (auto it{ graph.find(node) }; it != graph.end()) {
                    for (const auto& adj : it->second) {
                        if (!visited.contains(adj)) {
                            visited.insert(adj);
                            stack.push(adj);
                        }
                    }
                }
            }
        }
        return false;
    }

    // ------------------------------------------------------------
    // SEARCH BFS (retorna la distància mínima)
    // ------------------------------------------------------------
    auto search_bfs(const gentree<std::int32_t>& tree, std::int32_t startpoint, std::int32_t endpoint) -> std::int32_t {
        if (startpoint == endpoint) return 0;

        const auto& graph{ tree.get_graph() };
        if (!graph.contains(startpoint) || !graph.contains(endpoint))
            return -1;

        std::unordered_set<std::int32_t> visited{};
        std::queue<std::pair<std::int32_t, std::int32_t>> q{}; // node, dist

        q.emplace(startpoint, 0);
        visited.insert(startpoint);

        while (!q.empty()) {
            auto [node, dist] = q.front();
            q.pop();

            if (node == endpoint)
                return dist;

            if (auto it{ graph.find(node) }; it != graph.end()) {
                for (const auto& adj : it->second) {
                    if (!visited.contains(adj)) {
                        visited.insert(adj);
                        q.emplace(adj, dist + 1);
                    }
                }
            }
        }
        return -1; // No hi ha camí
    }

    // ------------------------------------------------------------
    // SEARCH DFS (retorna la longitud del camí si existeix)
    // ------------------------------------------------------------
    auto search_dfs(const gentree<std::int32_t>& tree, std::int32_t startpoint, std::int32_t endpoint) -> std::int32_t {
        if (startpoint == endpoint) return 0;

        const auto& graph{ tree.get_graph() };
        if (!graph.contains(startpoint) || !graph.contains(endpoint))
            return -1;

        std::unordered_set<std::int32_t> visited{};
        std::stack<std::pair<std::int32_t, std::int32_t>> stack{}; // node, depth

        stack.emplace(startpoint, 0);
        visited.insert(startpoint);

        while (!stack.empty()) {
            auto [node, depth] = stack.top();
            stack.pop();

            if (node == endpoint)
                return depth;

            if (auto it{ graph.find(node) }; it != graph.end()) {
                for (const auto& adj : it->second) {
                    if (!visited.contains(adj)) {
                        visited.insert(adj);
                        stack.emplace(adj, depth + 1);
                    }
                }
            }
        }

        return -1; // No hi ha camí
    }
}// namespace base