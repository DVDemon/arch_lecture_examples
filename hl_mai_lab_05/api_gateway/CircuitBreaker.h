#pragma once

#include <string>
#include <map>
#include <chrono>
#include <iostream>

enum class State
{
    close = 0,
    open = 1 ,
    semi_open = 2
};

#define FAIL_COUNT 5
#define TIME_LIMIT 5
#define SUCCESS_LIMIT 5

struct ServiceState
{
    State state{State::close};
    std::string service;
    size_t fail_count{0};
    size_t success_count{0};
    std::chrono::_V2::system_clock::time_point state_time;
};

class CircuitBreaker
{
    std::map<std::string, ServiceState> services;

public:
    bool check(const std::string &service_name)
    {
        if (services.find(service_name) == std::end(services))
            return true;

        ServiceState &ss = services[service_name];

        std::cout << "# circuit breaker: state ["<< ((int)ss.state)<<"] fail ["<< ss.fail_count <<"] success ["<< ss.success_count<<"]" << std::endl;

        switch (ss.state)
        {
        case State::close:
            return true;

        case State::semi_open:
            return true;

        case State::open:
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed_seconds = std::chrono::duration<double>(end - ss.state_time).count();

            if (elapsed_seconds >= TIME_LIMIT)
            {
                std::cout << "# circuit breaker: time limit reached" << std::endl;
                ss.state = State::semi_open;
                ss.success_count = 0;
                ss.fail_count = 0;
                return true;
            }

            return false;
        }
        return false;
    }

    void fail(const std::string &service_name)
    {
        if (services.find(service_name) == std::end(services))
        {
            ServiceState ss;
            ss.service = service_name;
            ss.state = State::close;
            ss.fail_count = 1;
            services[service_name] = ss;
        }
        else
        {
            ServiceState &ss = services[service_name];
            if (ss.state == State::close)
            {
                ss.state_time = std::chrono::high_resolution_clock::now();
                ++ss.fail_count;
                if (ss.fail_count > FAIL_COUNT){
                    std::cout << "# circuit breaker: error limit reached" << std::endl;
                    ss.state = State::open;
                }
            } else
            if (ss.state == State::semi_open)
            {
                ss.state = State::open;
                ss.state_time = std::chrono::high_resolution_clock::now();
                ss.success_count = 0;
            }
        }
    }

    void success(const std::string &service_name)
    {
        if (services.find(service_name) != std::end(services))
        {
            ServiceState &ss = services[service_name];
            if (ss.state == State::semi_open)
            {
                ++ss.success_count;
                if (ss.success_count > SUCCESS_LIMIT)
                {
                    std::cout << "# circuit breaker: success limit reached" << std::endl;
                    ss.state = State::close;
                    ss.success_count = 0;
                    ss.fail_count = 0;
                }
            } 
        }
    }
};