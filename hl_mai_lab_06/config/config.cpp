#include "config.h"

std::string read_env(const std::string& name){
    if(std::getenv(name.c_str())!=nullptr) return std::getenv(name.c_str());
    return std::string();
}

Config::Config()
{
        _host = read_env("DB_HOST");
        _port = read_env("DB_PORT");
        _login = read_env("DB_LOGIN");
        _password = read_env("DB_PASSWORD");
        _database = read_env("DB_DATABASE");
        _mongo = read_env("MONGO_HOST");
        _mongo_port = read_env("MONGO_PORT");
        _mongo_database = read_env("MONGO_DATABASE");
        _cache_servers = read_env("CACHE");
        _queue_server = read_env("QUEUE_SERVER");
        _queue_port = read_env("QUEUE_PORT");
        _queue_topic =read_env("QUEUE_TOPIC");
}

std::string& Config::queue_server(){
    return _queue_server;
}

std::string& Config::queue_port(){
    return _queue_port;
}

std::string& Config::queue_topic(){
    return _queue_topic;
}

const std::string& Config::get_queue_server(){
    return _queue_server;
}

const std::string& Config::get_queue_port(){
    return _queue_port;
}

const std::string& Config::get_queue_topic(){
    return _queue_topic;
}

const std::string &Config::get_cache_servers() const
{
    return _cache_servers;
}

std::string &Config::cache_servers()
{
    return _cache_servers;
}

std::string& Config::mongo(){
    return _mongo;
}
std::string& Config::mongo_port(){
    return _mongo_port;
}
std::string& Config::mongo_database(){
    return _mongo_database;
}

const std::string& Config::get_mongo() const{
    return _mongo;
}
const std::string& Config::get_mongo_port() const{
    return _mongo_port;
}
const std::string& Config::get_mongo_database() const{
    return _mongo_database;
}

Config &Config::get()
{
    static Config _instance;
    return _instance;
}

const std::string &Config::get_port() const
{
    return _port;
}

const std::string &Config::get_host() const
{
    return _host;
}

const std::string &Config::get_login() const
{
    return _login;
}

const std::string &Config::get_password() const
{
    return _password;
}
const std::string &Config::get_database() const
{
    return _database;
}

std::string &Config::port()
{
    return _port;
}

std::string &Config::host()
{
    return _host;
}

std::string &Config::login()
{
    return _login;
}

std::string &Config::password()
{
    return _password;
}

std::string &Config::database()
{
    return _database;
}