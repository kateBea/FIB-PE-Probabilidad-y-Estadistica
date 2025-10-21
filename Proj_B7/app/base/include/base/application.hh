//
// Created by zanet on 10/7/2025.
//

#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <cstdint>

namespace base {

    class application {
    public:
        virtual auto run( const std::int32_t argc, char** argv ) -> std::int32_t = 0;
        virtual ~application() = default;
    };
}

#endif //APPLICATION_HH
