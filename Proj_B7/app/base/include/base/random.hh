//
// Created by zanet on 10/7/2025.
//

#ifndef RANDOM_HH
#define RANDOM_HH

#include <cstdint>

namespace base {

    [[nodiscard]] auto next_int( ) -> std::int32_t;
    [[nodiscard]] auto next_int( std::int32_t lowerBound, std::int32_t upperbound) -> std::int32_t;

    [[nodiscard]] auto next_real( ) -> double;
    [[nodiscard]] auto next_real( double lowerBound, double upperbound) -> double;
}


#endif//RANDOM_HH
