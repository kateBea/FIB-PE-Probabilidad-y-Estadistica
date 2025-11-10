
#ifndef SEARCH_HH
#define SEARCH_HH

#include <cstdint>
#include <base/gentree.hh>

namespace base {

    // devuelve true si el target existe en el árbol usando BFS
    auto exists_bfs(const gentree<std::int32_t> tree, std::int32_t target) -> bool;

    // devuelve true si el target existe en el árbol usando DFS
    auto exists_dfs(const gentree<std::int32_t>& tree, std::int32_t target) -> bool;

    // devuelve -1 si no existe camino, 0 si startpoint == endpoint, o la longitud del camino entre ambos nodos usando BFS
    auto search_bfs(const gentree<std::int32_t>& tree, std::int32_t startpoint, std::int32_t endpoint) -> std::int32_t;

    // devuelve -1 si no existe camino, 0 si startpoint == endpoint, o la longitud del camino entre ambos nodos usando DFS
    auto search_dfs(const gentree<std::int32_t>& tree, std::int32_t startpoint, std::int32_t endpoint) -> std::int32_t;
}

#endif //SEARCH_HH