#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class PizzaOrder{
        private:
            long _id;
            long _user_id;
            std::string _type;
            long _count;
            long _price;

            struct Address {
                std::string city;
                std::string street;
                std::string country;
                const std::string to_string() const;
            };
            Address _address;

        public:

            static PizzaOrder  fromJSON(const std::string & str);

            long               get_id() const;
            long               get_user_id() const;
            long               get_count() const;
            long               get_price() const;
            const std::string &get_type() const;
            const std::string  get_address_as_string() const;

            long&        id();
            long&        user_id();
            long&        count();
            long&        price();
            std::string& type();
            Address&     address() ;

            static std::optional<PizzaOrder> read_by_id(long id);
            static std::vector<PizzaOrder> read_by_user_id(long user_id);
            void   add();
            void   update();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}
