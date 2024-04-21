#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

#include <Poco/Data/PostgreSQL/Connector.h>
#include <Poco/Data/PostgreSQL/PostgreSQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

#include <Poco/MongoDB/MongoDB.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>
#include <Poco/MongoDB/Cursor.h>

namespace database{
    class Database{
        private:
            std::string _connection_string;
            std::unique_ptr<Poco::Data::SessionPool> _pool;

            Poco::MongoDB::Connection connection_mongo;
            Poco::MongoDB::Database   database_mongo;

            Database();
        public:
            static Database& get();
            Poco::Data::Session create_session();
            Poco::MongoDB::Database& get_mongo_database();
            void send_to_mongo(const std::string& collection,Poco::JSON::Object::Ptr json);
            void update_mongo(const std::string& collection,std::map<std::string,long>& params,Poco::JSON::Object::Ptr json);
            std::vector<std::string> get_from_mongo(const std::string& collection,std::map<std::string,long>& params);
            long count_from_mongo(const std::string& collection,std::map<std::string,long>& params);
    };
}
#endif