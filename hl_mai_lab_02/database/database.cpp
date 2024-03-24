#include "database.h"
#include "../config/config.h"
#include <stdexcept>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Database::Database() : database_mongo(Config::get().get_mongo_database())
    {
        _connection_string += "host=";
        _connection_string += Config::get().get_host();
        _connection_string += " user=";
        _connection_string += Config::get().get_login();
        _connection_string += " dbname=";
        _connection_string += Config::get().get_database();
        _connection_string += " port=";
        _connection_string += Config::get().get_port();
        _connection_string += " password=";
        _connection_string += Config::get().get_password();

        std::cout << "# Connecting to postgresql: " << _connection_string << std::endl;
        Poco::Data::PostgreSQL::Connector::registerConnector();
        _pool = std::make_unique<Poco::Data::SessionPool>(Poco::Data::PostgreSQL::Connector::KEY, _connection_string);

        std::cout << "# Connecting to mongodb: " << Config::get().get_mongo() << ":" << Config::get().get_mongo_port()  << std::endl;
        connection_mongo.connect(Config::get().get_mongo(), atoi(Config::get().get_mongo_port().c_str()));
    }

    Poco::MongoDB::Database &Database::get_mongo_database()
    {
        return database_mongo;
    }

    Database &Database::get()
    {
        static Database _instance;
        return _instance;
    }

    Poco::Data::Session Database::create_session()
    {
        return Poco::Data::Session(_pool->get());
    }

    long Database::count_from_mongo(const std::string &collection, std::map<std::string, long> &params)
    {
        try
        {
            Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = database_mongo.createCountRequest(collection);

            auto &query = request->selector().addNewDocument("query");
            for (auto &[key, val] : params)
                query.addNewDocument(key).add("$eq",val);
            
            Poco::MongoDB::ResponseMessage response;
            connection_mongo.sendRequest(*request, response);

            if ( response.hasDocuments() )
                return response.documents()[0]->getInteger("n");
        }
        catch (std::exception &ex)
        {
            std::cout << "mongodb exception: " << ex.what() << std::endl;
            std::string lastError = database_mongo.getLastError(connection_mongo);
            if (!lastError.empty())
                std::cout << "mongodb Last Error: " << lastError << std::endl;
        }

        return -1;
    }
    void Database::update_mongo(const std::string &collection, std::map<std::string, long> &params, Poco::JSON::Object::Ptr json)
    {
        try
        {
            Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = database_mongo.createUpdateRequest(collection);
            for (auto &[key, val] : params)
                request->selector().add(key, val);
            std::function<void(Poco::MongoDB::Document &, Poco::JSON::Object::Ptr &)> fill_document;

            fill_document = [&](Poco::MongoDB::Document &doc, Poco::JSON::Object::Ptr &obj) -> void
            {
                for (auto &val : *obj)
                {
                    if (val.second.isInteger())
                    {
                        int v;
                        val.second.convert(v);
                        doc.add(val.first, v);
                    }
                    else if (val.second.isString())
                    {
                        doc.add(val.first, val.second.extract<std::string>());
                    }
                    else
                    {
                        try
                        {
                            Poco::JSON::Object::Ptr child = val.second.extract<Poco::JSON::Object::Ptr>();
                            Poco::MongoDB::Document &child_doc = doc.addNewDocument(val.first);
                            fill_document(child_doc, child);
                        }
                        catch (...)
                        {
                            doc.add(val.first, val.second.toString());
                        }
                    }
                }
            };

            Poco::MongoDB::Document &doc = request->update();
            fill_document(doc, json);
            connection_mongo.sendRequest(*request);
        }
        catch (std::exception &ex)
        {
            std::cout << "mongodb exception: " << ex.what() << std::endl;
            std::string lastError = database_mongo.getLastError(connection_mongo);
            if (!lastError.empty())
                std::cout << "mongodb Last Error: " << lastError << std::endl;
        }
    }

    void Database::send_to_mongo(const std::string &collection, Poco::JSON::Object::Ptr json)
    {
        try
        {
            Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertRequest = database_mongo.createInsertRequest(collection);
            std::function<void(Poco::MongoDB::Document &, Poco::JSON::Object::Ptr &)> fill_document;

            fill_document = [&](Poco::MongoDB::Document &doc, Poco::JSON::Object::Ptr &obj) -> void
            {
                for (auto &val : *obj)
                {
                    if (val.second.isInteger())
                    {
                        int v;
                        val.second.convert(v);
                        doc.add(val.first, v);
                    }
                    else if (val.second.isString())
                    {
                        doc.add(val.first, val.second.extract<std::string>());
                    }
                    else
                    {
                        try
                        {
                            Poco::JSON::Object::Ptr child = val.second.extract<Poco::JSON::Object::Ptr>();
                            Poco::MongoDB::Document &child_doc = doc.addNewDocument(val.first);
                            fill_document(child_doc, child);
                        }
                        catch (...)
                        {
                            doc.add(val.first, val.second.toString());
                        }
                    }
                }
            };

            Poco::MongoDB::Document &doc = insertRequest->addNewDocument();
            fill_document(doc, json);
            connection_mongo.sendRequest(*insertRequest);

        }
        catch (std::exception &ex)
        {
            std::cout << "mongodb exception: " << ex.what() << std::endl;
            std::string lastError = database_mongo.getLastError(connection_mongo);
            if (!lastError.empty())
                std::cout << "mongodb Last Error: " << lastError << std::endl;
        }
    }

    std::vector<std::string> Database::get_from_mongo([[maybe_unused]] const std::string &collection, [[maybe_unused]] std::map<std::string, long> &params)
    {
       std::vector<std::string> result;

        try
        {

            Poco::MongoDB::QueryRequest request("arch.orders");
            Poco::MongoDB::ResponseMessage response;
            for (auto &[key, val] : params)
                request.selector().add(key, val);
            connection_mongo.sendRequest(request, response);

            for (auto doc : response.documents())
                result.push_back(doc->toString());
        }
        catch (std::exception &ex)
        {
            std::cout << "mongodb exception: " << ex.what() << std::endl;
            std::string lastError = database_mongo.getLastError(connection_mongo);
            if (!lastError.empty())
                std::cout << "mongodb Last Error: " << lastError << std::endl;
        }
        return result;
    }
}