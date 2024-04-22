#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>

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

#include "../database/cache.h"
/**
 * @brief Обработчик запросов на проксирование API
 *
 */
class GatewayHandler : public HTTPRequestHandler
{
protected:
    std::string get_key(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth){
        return method+":"+base_path+":"+query+":"+basic_auth;
    }

    std::string get_cached(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth){
        std::string key =  get_key(method, base_path, query, basic_auth);
        std::string result;

        if(database::Cache::get().get(key,result)){
            return result;
        } else return std::string();
    }

    void put_cache(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth,const std::string &result){
        std::string key =  get_key(method, base_path, query, basic_auth);
        database::Cache::get().put(key,result);
    }

    std::string get_request(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth, const std::string &token, const std::string &body)
    {
        try
        {
            // URL для отправки запроса
            Poco::URI uri(base_path + query);
            std::string path(uri.getPathAndQuery());
            if (path.empty())
                path = "/";

            std::cout << "# api gateway: request " << base_path + query << std::endl;
            // Создание сессии HTTP
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // Создание запроса
            Poco::Net::HTTPRequest request(method, path, Poco::Net::HTTPMessage::HTTP_1_1);
            request.setContentType("application/json");
            request.setContentLength(body.length());
            session.sendRequest(request) << body;

            if (!basic_auth.empty())
            {
                request.set("Authorization", "Basic " + basic_auth);
            }
            else if (!token.empty())
            {
                request.set("Authorization", "Bearer " + token);
            }

            // Отправка запроса
            session.sendRequest(request);

            // Получение ответа
            Poco::Net::HTTPResponse response;
            std::istream &rs = session.receiveResponse(response);

            // Вывод ответа на экран
            std::stringstream ss;
            Poco::StreamCopier::copyStream(rs, ss);

            return ss.str();
        }
        catch (Poco::Exception &ex)
        {
            std::cerr << ex.displayText() << std::endl;
            return std::string();
        }
        return std::string();
    }

public:
    void handleRequest(HTTPServerRequest &request,
                       [[maybe_unused]] HTTPServerResponse &response)
    {
        std::cout << std::endl << "# api gateway start handle request" << std::endl;
        std::string base_url_user = "http://localhost:8080";
        std::string base_url_data = "http://localhost:8080";

        if(std::getenv("USER_ADDRESS")) base_url_user = std::getenv("USER_ADDRESS");
        if(std::getenv("DATA_ADDRESS")) base_url_data = std::getenv("DATA_ADDRESS");

        std::string scheme;
        std::string info;
        request.getCredentials(scheme, info);
        std::cout << "# api gateway - scheme: " << scheme << " identity: " << info << std::endl;

        std::string login, password;
        if (scheme == "Basic")
        {
            if(request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
                std::string cache_result = get_cached(request.getMethod(), base_url_data, request.getURI(),info);
                if(!cache_result.empty()){
                    std::cout << "# api gateway - from cache : " << cache_result << std::endl;
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << cache_result;
                    ostr.flush();
                    return;
                }
            }

            std::string token = get_request(Poco::Net::HTTPRequest::HTTP_GET,base_url_user, "/auth", info, "","");
            std::cout << "# api gateway - auth :" << token << std::endl;
            if (!token.empty())
            {
                    std::string real_result = get_request(request.getMethod(), base_url_data, request.getURI(), "", token,"");
                    std::cout << "# api gateway - result: " << std::endl
                              << real_result << std::endl;

                    if(!real_result.empty()){
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << real_result;
                        ostr.flush();
                        put_cache(request.getMethod(), base_url_data, request.getURI(),info,real_result);
                    } else {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root->set("type", "/errors/unauthorized");
                        root->set("title", "Internal exception");
                        root->set("status", "401");
                        root->set("detail", "not authorized");
                        root->set("instance", "/auth");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                    }
            }
        }
    }
};

class HTTPRequestFactory : public HTTPRequestHandlerFactory
{
    HTTPRequestHandler *createRequestHandler([[maybe_unused]] const HTTPServerRequest &request)
    {
        return new GatewayHandler();
    }
};

class HTTPWebServer : public Poco::Util::ServerApplication
{
protected:
    int main([[maybe_unused]] const std::vector<std::string> &args)
    {
        ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", 8888));
        HTTPServer srv(new HTTPRequestFactory(), svs, new HTTPServerParams);

        std::cout << "Started gatweay on port: 8888" << std::endl;
        srv.start();
        waitForTerminationRequest();
        srv.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char *argv[])
{
    HTTPWebServer app;
    return app.run(argc, argv);
}