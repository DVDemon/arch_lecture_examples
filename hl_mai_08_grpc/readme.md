

### Устанавливаем среду для работы с GRPC

git clone
--recurse-submodules -b v1.43.0 https://github.com/grpc/grpc

cd grpc

mkdir -p
cmake/build

pushd
cmake/build

cmake
-DgRPC_INSTALL=ON  -DgRPC_BUILD_TESTS=OFF
../..

make -j

sudo make
install

popd

### Tutorial

[https://grpc.io/docs/languages/cpp/quickstart/](https://grpc.io/docs/languages/cpp/quickstart/)
