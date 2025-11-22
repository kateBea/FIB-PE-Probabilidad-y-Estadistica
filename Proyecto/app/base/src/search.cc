
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

    #if 0
    using Graf = gentree<std::int32_t>::adjacency_list_type;
    using Arestes = gentree<std::int32_t>::vertex_list_type;

    auto bfs_aux( std::int32_t orig, std::int32_t dest, const Graf &g ) -> std::int32_t {

        if ( orig == dest ) {
            return 0;
        }

        std::int32_t level{ 1 };
        std::int32_t tam_level{ 1 };

        std::queue<std::int32_t> nodes;
        nodes.push( orig );

        std::set<std::int32_t> consultados{ orig };

        while ( !nodes.empty() ) {

            std::int32_t fin{ tam_level };
            tam_level = 0;

            for ( std::int32_t i{ 0 }; i < fin; ++i ) {

                Graf::const_iterator it = g.find( nodes.front() );
                if ( it != g.end() ) {

                    Arestes::const_iterator it2{ ( *it ).second.begin() };
                    Arestes::const_iterator end{ ( *it ).second.end() };

                    while ( it2 != end ) {

                        if ( *it2 == dest ) return level;

                        if ( consultados.find( *it2 ) == consultados.end() ) {

                            consultados.insert( *it2 );
                            nodes.push( *it2 );
                            ++tam_level;
                        }
                        ++it2;
                    }
                }
                nodes.pop();
            }
            ++level;
        }

        return -1;
    }

    auto dfs_aux( std::int32_t orig, std::int32_t dest, const Graf &g, std::int32_t level, std::int32_t &min_cami, std::set<int> &cons ) -> std::int32_t {

        if ( cons.find( orig ) != cons.end() ) {
            return -1;
        }
        cons.insert( orig );

        Graf::const_iterator it = g.find( orig );
        if ( it != g.end() ) {

            Arestes::const_iterator it2{ ( *it ).second.begin() };
            Arestes::const_iterator end{ ( *it ).second.end() };

            while ( it2 != end ) {

                if ( min_cami == -1 || level < min_cami ) {
                    /*
                Solo comprobamos los nodos si, o bien no hemos encontrado ningún camino hacia dest, o el
                camino que estamos comprobando es más corto que el mínimo que tenemos.

                Por ejemplo, si de momento tenemos que el camino más corto de orig a dest mide 3, no nos
                sirve de nada seguir por otro camino cuya longitud sea mayor o igual a 3, no cambiaremos
                el camino mínimo.
                */
                    if ( *it2 == dest ) {
                        cons.erase( orig );
                        return level;
                    }

                    std::int32_t cami_provis{ dfs_aux( ( *it2 ), dest, g, level + 1, min_cami, cons ) };
                    if ( cami_provis != -1 ) {
                        if ( min_cami == -1 || cami_provis < min_cami ) {
                            min_cami = cami_provis;
                        }
                    }
                }
                ++it2;
            }
        }
        cons.erase( orig );
        return min_cami;
    }

    auto exists_bfs( const gentree<std::int32_t> tree, std::int32_t target ) -> bool {
        return true;
    }

    auto exists_dfs( const gentree<std::int32_t> &tree, std::int32_t target ) -> bool {
        return true;
    }

    auto search_bfs( const gentree<std::int32_t> &tree, std::int32_t startpoint, std::int32_t endpoint ) -> std::int32_t {
        if ( startpoint == endpoint ) {
            return 0;
        }

        return bfs_aux( startpoint, endpoint, tree.get_graph() );
    }

    auto search_dfs( const gentree<std::int32_t> &tree, std::int32_t startpoint, std::int32_t endpoint ) -> std::int32_t {
        if ( startpoint == endpoint ) {
            return 0;
        }

        std::set<int> consultados{};
        std::int32_t min_cami{ -1 };

        return dfs_aux( startpoint, endpoint, tree.get_graph(), 1, min_cami, consultados );
    }

    #endif
}// namespace base