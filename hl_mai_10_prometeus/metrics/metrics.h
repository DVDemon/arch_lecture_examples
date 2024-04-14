#pragma once

#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/IntCounter.h"

class Metrics{
    private:
        Metrics();
        Poco::Prometheus::Registry reg;
        Poco::Prometheus::IntCounter requests_cnt;
    public:
        static Metrics& get();  
        const Poco::Prometheus::Registry &registry();
        Poco::Prometheus::IntCounter &requests();
};