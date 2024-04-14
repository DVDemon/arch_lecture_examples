#include "metrics.h"

Metrics::Metrics() : reg(), requests_cnt("requests",&reg){

}

Metrics& Metrics::get(){
    static Metrics m;
    return m;
} 
const Poco::Prometheus::Registry &Metrics::registry(){
    return reg;
}

Poco::Prometheus::IntCounter &Metrics::requests(){
    return requests_cnt;
}