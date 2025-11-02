//
// Created by zanet on 10/7/2025.
//

#ifndef TESTER_APP_HH
#define TESTER_APP_HH

#include <taskflow/taskflow.hpp>

#include <base/application.hh>

namespace tester {

    class tester_app : public base::application {
    public:
        auto run( const std::int32_t argc, char **argv ) -> std::int32_t override;

    private:
        tf::Executor m_Executor{};
    };
}



#endif //TESTER_APP_HH
