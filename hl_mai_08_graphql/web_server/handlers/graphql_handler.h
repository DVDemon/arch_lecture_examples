#ifndef GRAPHQLHANDLER_H
#define GRAPHQLHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>
#include <cstdio>
#include <iterator>
#include <stdexcept>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/ServiceMock.h"

class GraphQLHandler : public HTTPRequestHandler
{
public:
    GraphQLHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest([[maybe_unused]] HTTPServerRequest &request,
                       [[maybe_unused]] HTTPServerResponse &rsp)
    {
        rsp.setChunkedTransferEncoding(true);
        rsp.setContentType("text/json");

        std::ostream &ostr = rsp.send();
        auto service = graphql::database::object::GetService();

        try
        {
            graphql::peg::ast query;

            std::istream_iterator<char> start{request.stream() >> std::noskipws}, end{};
            std::string input{start, end};

            query = graphql::peg::parseString(std::move(input));

            if (!query.root)
            {
                std::cerr << "Unknown error!" << std::endl;
                std::cerr << std::endl;
            }

            ostr << graphql::response::toJSON(service->resolve({query, ""}).get())
                 << std::endl;
        }
        catch (const std::runtime_error &ex)
        {
            std::cerr << "exception:" << ex.what() << std::endl;
        }
    }

private:
    std::string _format;
};
#endif // !WEBPAGEHANDLER_H