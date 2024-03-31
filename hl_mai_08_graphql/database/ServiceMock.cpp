#include "ServiceMock.h"
#include <iostream>
#include "database.h"
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace Poco::Data::Keywords;

namespace graphql::database::object
{

    std::shared_ptr<graphql::service::Request> GetService()
    {
        std::shared_ptr<Query> query = std::make_shared<Query>(std::make_shared<QueryImpl>());
        std::shared_ptr<Mutations> mutation = std::make_shared<Mutations>(std::make_shared<MutationsImpl>());
        auto service = std::make_shared<Operations>(std::move(query),std::move(mutation));
        return service;
    }

    void QueryImpl::beginSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const
    {
    }
    void QueryImpl::endSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const
    {
    }

    std::shared_ptr<User> QueryImpl::getGet_user([[maybe_unused]] service::FieldParams&& params, 
                                                    [[maybe_unused]] std::optional<int> &&idArg) const
    {
        std::cout << "user" << std::endl;
        auto user_impl = std::make_shared<UserImpl>();
        try
        {
            Poco::Data::Session session = ::db::Database::get().create_session();
            Poco::Data::Statement select(session);
            select << "SELECT id, first_name, last_name, email, title, login, password FROM users where id=$1",
                into(user_impl->_id),
                into(user_impl->_first_name),
                into(user_impl->_last_name),
                into(user_impl->_email),
                into(user_impl->_title),
                into(user_impl->_login),
                into(user_impl->_password),
                use(idArg.value()),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                if (!select.execute())
                    throw std::logic_error("Record not found");
            }
            return std::make_shared<User>(user_impl);
        }

        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
             std::cout << "connection:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {
             std::cout << "statement:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
    }

    std::optional<std::vector<std::shared_ptr<User>>> QueryImpl::getAll_users([[maybe_unused]] service::FieldParams&& params) const
    {
        try
        {
            std::cout << "getAll" << std::endl;
            Poco::Data::Session session = ::db::Database::get().create_session();
            Poco::Data::Statement select(session);

            std::vector<std::shared_ptr<User>> result;
            auto user_impl = std::make_shared<UserImpl>();
            select << "SELECT id, first_name, last_name, email, title,login, password FROM users",
                into(user_impl->_id),
                into(user_impl->_first_name),
                into(user_impl->_last_name),
                into(user_impl->_email),
                into(user_impl->_title),
                into(user_impl->_login),
                into(user_impl->_password),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                {
                    auto a = std::make_shared<UserImpl>();
                    *a = *user_impl;
                    result.push_back(std::make_shared<User>(a));
                }
            }
            return result;
        }
        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
    }

    std::vector<std::shared_ptr<User>> QueryImpl::getSearch([[maybe_unused]] service::FieldParams&& params,
     [[maybe_unused]] std::string &&term1Arg, [[maybe_unused]] std::string &&term2Arg) const
    {
        try
        {
            std::cout << "search" << std::endl;
            Poco::Data::Session session = ::db::Database::get().create_session();
            Poco::Data::Statement select(session);

            std::vector<std::shared_ptr<User>> result;
            auto user_impl = std::make_shared<UserImpl>();
            select << "SELECT id, first_name, last_name, email, title, login, password FROM users WHERE first_name LIKE $1 AND last_name LIKE $2",
                into(user_impl->_id),
                into(user_impl->_first_name),
                into(user_impl->_last_name),
                into(user_impl->_email),
                into(user_impl->_title),
                into(user_impl->_login),
                into(user_impl->_password),
                use(term1Arg),
                use(term2Arg),
                range(0, 1); //  iterate over result set one row at a time
            std::cout << select.toString() << std::endl;
            while (!select.done())
            {
                if (select.execute())
                {
                    auto a = std::make_shared<UserImpl>();
                    *a = *user_impl;
                    result.push_back(std::make_shared<User>(a));
                }
            }
            return result;
        }
        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw std::logic_error(e.what());
        }
    }


    void MutationsImpl::beginSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const{

    }
    void MutationsImpl::endSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const{

    }
    std::string MutationsImpl::applyAdd_user([[maybe_unused]] service::FieldParams&& params, 
                                              [[maybe_unused]] std::string && first_nameArg, 
                                              [[maybe_unused]] std::string && last_nameArg, 
                                              [[maybe_unused]] std::string && emailArg, 
                                              [[maybe_unused]] std::string && titleArg, 
                                              [[maybe_unused]] std::string && loginArg, 
                                              [[maybe_unused]] std::string && passwordArg) const{

        try
        {
            std::cout << "mutation" << std::endl; 
            Poco::Data::Session session = ::db::Database::get().create_session();
            Poco::Data::Statement insert(session);
            long _id{1};
            insert << "INSERT INTO users (first_name,last_name,email,title,login,password) VALUES($1, $2, $3, $4, $5, $6)",
                use(first_nameArg),
                use(last_nameArg),
                use(emailArg),
                use(titleArg),
                use(loginArg),
                use(passwordArg);
            std::cout << insert.toString() << std::endl;
            insert.execute();

            return std::to_string(_id);
        }
        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "connection:" << e.displayText() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::PostgreSQLException &e){
            
            std::cout << "connection:" << e.displayText() << std::endl;
            throw;
        }
    }
}