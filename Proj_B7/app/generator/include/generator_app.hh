//
// Created by zanet on 10/7/2025.
//

#ifndef GENERATOR_APP_HH
#define GENERATOR_APP_HH

#include <base/application.hh>

namespace gen {

    class generator_app : public base::application {
    public:
        auto run( const std::int32_t argc, char **argv ) -> std::int32_t override;
    };

}// namespace gen

#endif//GENERATOR_APP_HH
