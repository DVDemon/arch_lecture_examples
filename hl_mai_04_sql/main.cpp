#include <string>
#include <iostream>
#include <exception>

#include <Poco/Data/PostgreSQL/Connector.h>
#include <Poco/Data/PostgreSQL/PostgreSQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>

auto main(int argc, char *argv[]) -> int
{
    if (argc < 3)
    {
        std::cout << "Usage: sql_test address port" << std::endl;
        return 0;
    }
    std::string host(argv[1]);
    std::string port(argv[2]);

    std::cout << "connecting to:" << host <<":"<< port<< std::endl;
    Poco::Data::PostgreSQL::Connector::registerConnector();
    std::cout << "connector registered" << std::endl;

    std::string connection_str;
    connection_str = "user=stud password=stud host="+host+" port="+port+" dbname=archdb";

    std::cout << connection_str << std::endl;

    try
    {
        Poco::Data::Session session(
            Poco::Data::SessionFactory::instance().create(
                Poco::Data::PostgreSQL::Connector::KEY, connection_str));


        std::cout << "creating tables" << std::endl;
        Poco::Data::Statement create_stmt(session);
        create_stmt << "CREATE TABLE IF NOT EXISTS Author (id INT NOT NULL,"
                    << "first_name VARCHAR(256) NOT NULL,"
                    << "last_name VARCHAR(256)NOT NULL,"
                    << "email VARCHAR(256) NULL,"
                    << "title VARCHAR(1024) NULL,"
                    << "PRIMARY KEY (id));";
        create_stmt.execute();
        std::cout << "table created" << std::endl;

        Poco::Data::Statement sequence_stmt(session);
        sequence_stmt << "CREATE SEQUENCE IF NOT EXISTS Ids START 1";
        sequence_stmt.execute();

        std::cout << "sequence created" << std::endl;

        Poco::Data::Statement insert(session);

        std::string first_name{"Ivan"};
        std::string last_name{"Ivanov"};
        std::string email{"ivanov@ivan.ru"};
        std::string title{"mister"};

        insert << "INSERT INTO Author (id,first_name,last_name,email,title) VALUES( NEXTVAL('Ids'),$1, $2, $3, $4)",
            Poco::Data::Keywords::use(first_name),
            Poco::Data::Keywords::use(last_name),
            Poco::Data::Keywords::use(email),
            Poco::Data::Keywords::use(title);

        insert.execute();

        std::cout << "record inserted" << std::endl;

        Poco::Data::Statement select(session);
        long id;
        select << "SELECT id, first_name, last_name, email, title FROM Author",
            Poco::Data::Keywords::into(id),
            Poco::Data::Keywords::into(first_name),
            Poco::Data::Keywords::into(last_name),
            Poco::Data::Keywords::into(email),
            Poco::Data::Keywords::into(title),
            Poco::Data::Keywords::range(0, 1); //  iterate over result set one row at a time


        while (!select.done())
        {
            if(select.execute()){
                std::cout << id << ":[" << first_name << "," << last_name << "," << email << "," << title << "]" << std::endl;
            }
        };


        Poco::Data::Statement ids(session);
        ids << "SELECT NEXTVAL('Ids')", Poco::Data::Keywords::into(id); 
        if(!ids.done())
            if(ids.execute())
                std::cout << "next_sequence:" << id << std::endl;
            
        
    }
    catch (Poco::Data::PostgreSQL::PostgreSQLException &e)
    {
        std::cout << "connection:" << e.displayText() << std::endl;
    }
    catch (Poco::Data::ConnectionFailedException &e)
    {
        std::cout << "connection:" << e.displayText() << std::endl;
    }
    // catch (Poco::Data::PostgreSQL::StatementException &e)
    // {

    //     std::cout << "statement:" << e.displayText() << std::endl;
    // }
    catch (std::exception *ex)
    {
        std::cout << "exception:" << ex->what() << std::endl;
    }
    return 1;
}