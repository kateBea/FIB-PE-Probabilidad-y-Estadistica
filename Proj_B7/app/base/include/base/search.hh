
#ifndef SEARCH_HH
#define SEARCH_HH

#include <cstdint>
#include <base/gentree.hh>

namespace base {

    auto exists_bfs(const gentree<std::int32_t> tree, std::int32_t target) -> bool;
    auto exists_dfs(const gentree<std::int32_t>& tree, std::int32_t target) -> bool;

    auto search_bfs(const gentree<std::int32_t>& tree, std::int32_t startpoint, std::int32_t endpoint) -> bool;
    auto search_dfs(const gentree<std::int32_t>& tree, std::int32_t tartpoint, std::int32_t endpoint) -> bool;
}

#endif //SEARCH_HH