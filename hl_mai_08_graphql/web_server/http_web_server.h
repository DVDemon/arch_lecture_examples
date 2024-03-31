#ifndef HTTPWEBSERVER_H
#define HTTPWEBSERVER_H

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

#include "http_request_factory.h"
#include "../config/config.h"
#include "../database/database.h"
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
using namespace Poco::Data::Keywords;

class HTTPWebServer : public Poco::Util::ServerApplication
{
protected:
    void init()
    {
        try
        {

            Poco::Data::Session session = db::Database::get().create_session();
            Poco::Data::Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS users (id SERIAL,"
                        << "first_name VARCHAR(256) NOT NULL,"
                        << "last_name VARCHAR(256) NOT NULL,"
                        << "login VARCHAR(256) NOT NULL,"
                        << "password VARCHAR(256) NOT NULL,"
                        << "email VARCHAR(256) NULL,"
                        << "title VARCHAR(1024) NULL);",
                now;
        }

        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    int main([[maybe_unused]] const std::vector<std::string> &args)
    {
        init();
        std::string format(
            config().getString("HTTPWebServer.format",
                               DateTimeFormat::SORTABLE_FORMAT));

        ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", 8080));
        HTTPServer srv(new HTTPRequestFactory(format),
                       svs, new HTTPServerParams);
        srv.start();
        waitForTerminationRequest();
        srv.stop();

        return Application::EXIT_OK;
    }
};
#endif // !HTTPWEBSERVER