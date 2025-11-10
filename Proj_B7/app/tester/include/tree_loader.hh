
#ifndef TREE_LOADER_HH
#define TREE_LOADER_HH

#include <filesystem>

#include <base/gentree.hh>

namespace tester {
    class tree_loader {
    public:
        tree_loader() = default;
        ~tree_loader() = default;

        auto load_from_file(const std::filesystem::path uri) -> base::gentree<int>;
    };
}

#endif