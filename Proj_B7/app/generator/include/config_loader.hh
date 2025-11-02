//
// Created by zanet on 10/7/2025.
//

#ifndef GENERATOR_CONFIG_HH
#define GENERATOR_CONFIG_HH

#include <filesystem>

#include <base/config_loader.hh>

namespace gen {

    class generator_config final : public config {
    public:
        generator_config() = default;
        ~generator_config() override = default;

        auto load(const std::filesystem::path& filePath) -> void override;

    private:
        auto set_defaults() -> void;
    };

} // namespace gen

#endif // GENERATOR_CONFIG_HH
