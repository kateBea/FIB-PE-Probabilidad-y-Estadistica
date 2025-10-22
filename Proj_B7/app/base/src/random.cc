//
// Created by zanet on 10/7/2025.
//
#include <random>
#include <stdexcept>
#include <limits>
#include <cstdint>
#include <numeric>
#include <limits>

#include "base/random.hh"

namespace base {

    [[nodiscard]] inline auto get_seed() -> std::random_device& {
        thread_local std::random_device seed{};
        return seed;
    }

    static auto next_int( std::random_device& seed ) -> std::int32_t {
        thread_local std::mt19937_64 mt{ seed() };

        return std::uniform_int_distribution<std::int32_t>{
            (std::numeric_limits<std::int32_t>::min)(),
            (std::numeric_limits<std::int32_t>::max)(),
        }(mt);
    }

    static auto next_real( std::random_device& seed ) -> double {
        thread_local std::mt19937_64 mt{ seed() };

        return std::uniform_real_distribution<double>{
            (std::numeric_limits<double>::min)(),
            (std::numeric_limits<double>::max)(),
        }(mt);
    }

    auto next_int( ) -> std::int32_t {
        return next_int( get_seed() );
    }

    auto next_real( ) -> double {
        return next_real( get_seed() );
    }

    static auto next_int( std::int32_t lowerBound, std::int32_t upperbound, auto& seed ) -> std::int32_t {
        if ( lowerBound > upperbound )
            throw std::runtime_error( "Invalid range for random integer generation" );

        thread_local std::mt19937 mt{ seed() };
        return std::uniform_int_distribution{ lowerBound, upperbound }( mt );
    }

    auto next_int( std::int32_t lowerBound, std::int32_t upperbound ) -> std::int32_t {
        return next_int( lowerBound, upperbound, get_seed() );
    }

    static auto next_real( double lowerBound, double upperbound, auto& seed ) -> double {
        if ( lowerBound > upperbound )
            throw std::runtime_error( "Invalid range for random real generation" );

        thread_local std::mt19937 mt{ seed() };
        return std::uniform_real_distribution{ lowerBound, upperbound }( mt );
    }

    auto next_real( double lowerBound, double upperbound ) -> double {
        return next_real( lowerBound, upperbound, get_seed() );
    }
}