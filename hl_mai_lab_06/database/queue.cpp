#include "queue.h"
#include "../config/config.h"
#include <thread>

namespace database
{
    Queue::Queue() 
    {
        handler = std::make_unique<SimplePocoHandler>(Config::get().get_queue_server(), atoi(Config::get().get_queue_port().c_str()));
        connection = std::make_unique<AMQP::Connection>(handler.get(), AMQP::Login("guest", "guest"), "/");
        channel = std::make_unique<AMQP::Channel>(connection.get());

        std::thread th([&](){
            std::cout << "# connected to broker" << std::endl;
            while (true)
                handler->loop();
        });
        th.detach();
    }

    Queue& Queue::get()
    {
        static Queue instance;
        return instance;
    }

    void Queue::send(const std::string &val)
    {
        channel->declareQueue(Config::get().get_queue_topic());
        channel->publish("", Config::get().get_queue_topic(), val);
        std::cout << "#send [" << val << "]" << std::endl; 
    }

    void Queue::subscribe(callback_t callback)
    {
        channel->declareQueue(Config::get().get_queue_topic());
        channel->consume(Config::get().get_queue_topic(), 
                AMQP::noack).onReceived([&](const AMQP::Message &message, [[maybe_unused]] uint64_t deliveryTag, [[maybe_unused]] bool redelivered)
                {
                    std::string msg;
                    for(size_t i=0;i<message.bodySize();++i) msg+=message.body()[i];
                    std::cout << "#Received ["<<msg << "]" << std::endl;
                    callback(msg);
                });
    }
}