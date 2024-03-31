/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <grpcpp/grpcpp.h>
#include <chrono>
#include <future>
#include <vector>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "author.grpc.pb.h"
#endif

using author::Author;
using author::AuthorReply;
using author::AuthorRequest;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class AuthorClient
{
public:
  AuthorClient(std::shared_ptr<Channel> channel)
      : stub_(Author::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::tuple<int, std::string, std::string, std::string, std::string> GetAuthor(const int id)
  {
    // Data we are sending to the server.
    AuthorRequest request;
    request.set_id(id);

    // Container for the data we expect from the server.
    AuthorReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetAuthor(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return std::tuple<int, std::string, std::string, std::string, std::string>(reply.id(), reply.first_name(), reply.last_name(), reply.email(), reply.title());
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      throw "RPC failed";
    }
  }

private:
  std::unique_ptr<Author::Stub> stub_;
};

int main(int argc, char **argv)
{

  AuthorClient greeter(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  int count{1};

  if (argc > 1)
  {
    count = atoi(argv[1]);
  }

  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::future<void>> futures;

  for (int j = 0; j < 10; ++j)
    futures.push_back(std::async(
        std::launch::async, [&]() {
          for (int i = 0; i < count; ++i)
          {
            std::tuple<int, std::string, std::string, std::string, std::string> reply = greeter.GetAuthor(i);
            // std::cout << "id: " << std::get<0>(reply) << std::endl;
            // std::cout << "first_name: " << std::get<1>(reply) << std::endl;
            // std::cout << "last_name: " << std::get<2>(reply) << std::endl;
            // std::cout << "email: " << std::get<3>(reply) << std::endl;
            // std::cout << "title: " << std::get<4>(reply) << std::endl;
          }
        }
          ));

      for (auto &future : futures)
      {
        future.wait();
      }

      auto end = std::chrono::high_resolution_clock::now();
      // Вычисляем разницу во времени в наносекундах
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

      // Выводим результат
      std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
      std::cout << "RPS: " << (count*10*1000) / (duration) << std::endl;

      return 0;
}
