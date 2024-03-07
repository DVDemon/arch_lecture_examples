#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/StreamCopier.h>
#include <iostream>
#include <string>

int main()
{
    Poco::Net::ServerSocket srv(8080);
    Poco::Timespan span(250000);
    while (true)
    {
        if (srv.poll(span, Poco::Net::Socket::SELECT_READ))
        {

            Poco::Net::StreamSocket str = srv.acceptConnection();
            std::cout << "Connected:" << str.address().toString() << std::endl;

            // browsers likes to keap connection alive, so we nee to check end of HTTP header manually
            unsigned char  byte = 0,prev_byte=0;
            do{
                prev_byte = byte;
                str.receiveBytes(&byte, 1);          
            } while(( (byte != 13) || (prev_byte != 10)));

            str.shutdownReceive();

            std::cout << "sending .." << std::endl;
            std::string response;
            response += "HTTP/1.0 200 OK\r\n";
            response += "Content-type: text/html\r\n\r\n";
            response += "<html><head><title>My 1st Web Server</title></head><body><h1>Hello world!!!!</h1></body></html>\r\n";

            str.sendBytes(response.c_str(), response.size());
            str.shutdownSend();
        }
    }
}