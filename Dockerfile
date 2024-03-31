FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-22.04

RUN sudo apt-get update && sudo apt-get install -y git python3 pip iputils-ping gcc-12 libpq-dev postgresql-client wrk libssl-dev zlib1g-dev librdkafka-dev mysql-client libmysqlclient-dev libboost-all-dev\
    && sudo apt-get clean


RUN git clone -b poco-1.12.4-release https://github.com/pocoproject/poco.git &&\
    cd poco &&\
    mkdir cmake-build &&\
    cd cmake-build &&\
    cmake .. &&\
    cmake --build . --config Release &&\
    sudo cmake --build . --target install &&\
    cd && rm poco/* -rf 

RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git &&\
    cd AMQP-CPP &&\
    make &&\
    sudo make install &&\
    cd && rm AMQP-CPP/* -rf

RUN git clone https://github.com/edenhill/librdkafka.git &&\
    cd librdkafka &&\
    ./configure  &&\
    make &&\
    sudo make install &&\
    cd && rm librdkafka/* -rf
 
RUN git clone https://github.com/mfontanini/cppkafka &&\
    cd cppkafka &&\
    mkdir build &&\
    cd build &&\ 
    cmake .. &&\ 
    make &&\ 
    sudo make install &&\
    cd && rm cppkafka/* -rf

RUN git clone https://github.com/tdv/redis-cpp.git &&\
    cd redis-cpp &&\
    mkdir build &&\ 
    cd build &&\ 
    cmake .. &&\ 
    make &&\ 
    sudo make install &&\
    cd .. 

RUN git clone --recurse-submodules -b v1.43.0 https://github.com/grpc/grpc &&\
    cd grpc &&\
    mkdir -p cmake/build &&\
    cd cmake/build &&\
    cmake -DgRPC_INSTALL=ON  -DgRPC_BUILD_TESTS=OFF ../.. &&\
    make &&\
    make install &&\
    cd .. &&\
    cd ..

RUN git clone https://github.com/Tencent/rapidjson.git &&\
    cd rapidjson &&\
    mkdir build &&\
    cd build &&\
    cmake .. &&\
    make &&\
    make install &&\
    cd .. &&\
    cd ..

RUN git clone --recurse-submodules https://github.com/taocpp/PEGTL.git &&\
    cd PEGTL &&\
    mkdir build &&\
    cd build &&\
    cmake .. &&\
    make &&\
    make install &&\
    cd .. &&\
    cd ..

RUN git clone --recurse-submodules https://github.com/microsoft/cppgraphqlgen.git &&\
    cd cppgraphqlgen &&\
    mkdir build &&\
    cd build &&\
    cmake -D GRAPHQL_BUILD_TESTS=OFF .. &&\
    make &&\
    make install &&\
    cd .. &&\
    cd ..

RUN ldconfig

WORKDIR /opt/arch

ENTRYPOINT ["/usr/bin/bash"]
