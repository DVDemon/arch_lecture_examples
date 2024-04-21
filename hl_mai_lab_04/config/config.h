#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class  Config{
    private:
        Config();
        std::string _host;
        std::string _port;
        std::string _login;
        std::string _password;
        std::string _database;

        std::string _mongo;
        std::string _mongo_port;
        std::string _mongo_database;

        std::string _cache_servers;

    public:
        static Config& get();

        std::string& port();
        std::string& host();
        std::string& login();
        std::string& password();
        std::string& database();
        std::string& cache_servers();

        std::string& mongo();
        std::string& mongo_port();
        std::string& mongo_database();

        const std::string& get_port() const ;
        const std::string& get_host() const ;
        const std::string& get_login() const ;
        const std::string& get_password() const ;
        const std::string& get_database() const ;

        const std::string& get_mongo() const;
        const std::string& get_mongo_port() const;
        const std::string& get_mongo_database() const;

        const std::string& get_cache_servers() const;
};

#endif