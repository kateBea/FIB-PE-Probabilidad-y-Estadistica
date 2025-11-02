//
// Created by zanet on 10/7/2025.
//

#ifndef CONFIG_LOADER_HH
#define CONFIG_LOADER_HH

#include <cstdint>
#include <string>

/**
 * Can disable exceptions in
 * compiler flags and/or explicitly disable the library's use of them by setting the option
 * #TOML_EXCEPTIONS to 0. In either case, the parsing functions return a
 * toml::parse_result instead of a toml::table:
 *
 *  only necessary if you've left them enabled in your compiler #include <toml++/toml.hpp>
 * */
#define TOML_EXCEPTIONS 0

#include <any>
#include <unordered_map>
#include <filesystem>

#include <toml++/toml.hpp>

namespace gen {

    class config {
    public:
        virtual ~config() = default;

        virtual auto load( const std::filesystem::path& filePath ) -> void = 0;

        template<typename T>
        auto Get( const std::string& key, const T& defaultValue = {} ) -> T {
            const auto it{ m_data.find( key ) };
            if ( it != m_data.end() ) {
                if ( auto* val{ std::any_cast<T>( &it->second ) } ) {
                    return *val;
                }
            }

            return defaultValue;
        }

    protected:
        std::unordered_map<std::string, std::any> m_data{};
    };

} // namespace gen

#endif // CONFIG_LOADER_HH
