//
// Created by zanet on 10/7/2025.
//

#include <base/logger.hh>
#include <config_loader.hh>
#include <filesystem>

namespace gen {

    auto generator_config::load( const std::filesystem::path &filePath ) -> void {
        namespace fs = std::filesystem;

        if ( !fs::exists( filePath ) ) {
            LOG_WARN( "Config file '{}' not found. Using default values.", filePath.string() );
            set_defaults();
            return;
        }

        auto result{ toml::parse_file( filePath.string() ) };
        if ( !result ) {
            LOG_ERROR( "Failed to parse TOML config '{}': {}", filePath.string(), result.error().description() );
            set_defaults();
            return;
        }

        const toml::table &tbl{ result.table() };

        LOG_INFO( "Successfully parsed TOML config '{}'.", filePath.string() );

        // Load each key with defaults
        m_data["num_graphs"] = tbl["parameters"]["num_graphs"].value_or( 30 );
        m_data["min_nodes"] = tbl["parameters"]["min_nodes"].value_or( 20 );
        m_data["max_nodes"] = tbl["parameters"]["max_nodes"].value_or( 30 );
        m_data["max_adjacencies_per_node"] = tbl["generator"]["max_adjacencies_per_node"].value_or( 10 );

        LOG_INFO( "Loaded generator config from '{}': graphs={}, nodes=[{}, {}], max_adj={}",
                  filePath.string(),
                  std::any_cast<std::int32_t>( m_data["num_graphs"] ),
                  std::any_cast<std::int32_t>( m_data["min_nodes"] ),
                  std::any_cast<std::int32_t>( m_data["max_nodes"] ),
                  std::any_cast<std::int32_t>( m_data["max_adjacencies_per_node"] ) );
    }

    auto generator_config::set_defaults() -> void {
        m_data["num_graphs"] = 30;
        m_data["min_nodes"] = 20;
        m_data["max_nodes"] = 30;
        m_data["max_adjacencies_per_node"] = 10;

        LOG_WARN( "Using default generator configuration." );
    }

}// namespace gen
