# our base image
FROM ubuntu:18.04
RUN apt-get update -y && apt-get install gcc g++ cmake git sudo libssl-dev zlib1g-dev -y && apt-get clean -y
RUN git clone -b poco-1.11.1 https://github.com/pocoproject/poco.git && cd poco && mkdir cmake-build && cd cmake-build && cmake .. && cmake --build . --config Release && cmake --build . --target install && cd .. && rm -r /poco/*
RUN git clone https://github.com/DVDemon/hl_mai_03_server.git && cd hl_mai_03_server && mkdir CMakeFiles && cd CMakeFiles && cmake .. && cmake  --build . --config Release && mkdir content && cp ../content/* content/ && cd .. && chmod +x start.sh
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
WORKDIR /hl_mai_03_server/CMakeFiles
EXPOSE 8080
CMD ["./server"]

#docker build . -t my_server