#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

#include <Poco/MongoDB/MongoDB.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>

void save_to_mongo(const std::string &collection, Poco::JSON::Array::Ptr &json)
{
    Poco::MongoDB::Connection connection;
    Poco::MongoDB::Database db("contacts");
    connection.connect("mongodb", 27017);

    try
    {
        Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertRequest = db.createInsertRequest(collection);
        std::function<void(Poco::MongoDB::Document& , Poco::JSON::Object::Ptr &)>  fill_document;

        fill_document = [&](Poco::MongoDB::Document& doc, Poco::JSON::Object::Ptr &obj) -> void{
                for(auto &val : *obj) {
                    if(val.second.isInteger()){
                        int v;
                        val.second.convert(v);
                        doc.add(val.first,v);
                    } else
                    if(val.second.isString()){
                        doc.add(val.first,val.second.extract<std::string>());
                    } else
                    {
                        try{
                            Poco::JSON::Object::Ptr child = val.second.extract<Poco::JSON::Object::Ptr>();
                            Poco::MongoDB::Document& child_doc = doc.addNewDocument(val.first);
                            fill_document(child_doc,child);
                        }catch(...){
                            doc.add(val.first,val.second.toString());
                        }
                    }
                }

            };

        for (const Poco::Dynamic::Var &person : *json)
        {
            Poco::JSON::Object::Ptr obj = person.extract<Poco::JSON::Object::Ptr>();
            Poco::MongoDB::Document &doc = insertRequest->addNewDocument();
            fill_document(doc,obj);
        }
        
        connection.sendRequest(*insertRequest);
        std::string lastError = db.getLastError(connection);
        if (!lastError.empty()) std::cout << "mongodb Last Error: " << lastError << std::endl;
    }
    catch (std::exception &ex)
    {
        std::cout << "mongodb exception: " << ex.what() << std::endl;
        std::string lastError = db.getLastError(connection);
        if (!lastError.empty())
        {
            std::cout << "mongodb Last Error: " << lastError << std::endl;
        }
    }
}


int main()
{
    std::ifstream ifs("persons.json");
    if(ifs.is_open()){

        std::stringstream buffer;
        buffer << ifs.rdbuf();

        std::string fcontent = buffer.str();

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(fcontent);
        Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
        std::cout << "inserting:" << array->size() << std::endl;
        save_to_mongo("contacts",array);

        ifs.close();
    }

    return 0;
}