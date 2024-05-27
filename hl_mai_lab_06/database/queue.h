#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <functional>
#include <memory>
#include "../rabbit/SimplePocoHandler.h"

namespace database
{
    using callback_t = std::function<void(const std::string&)>;
    class Queue
    {
        private:
            Queue();
            std::unique_ptr<SimplePocoHandler> handler;
            std::unique_ptr<AMQP::Connection> connection;
            std::unique_ptr<AMQP::Channel> channel;
        public:
            static Queue& get();
            void send(const std::string & val);
            void subscribe(callback_t callback);

    };
}

#endif