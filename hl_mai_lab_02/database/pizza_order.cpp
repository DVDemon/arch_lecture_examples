#include "pizza_order.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    PizzaOrder PizzaOrder::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;
        PizzaOrder order;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        order.id() = object->getValue<long>("user_id");
        order.user_id() = object->getValue<long>("user_id");
        order.type() = object->getValue<std::string>("type");
        order.count() = object->getValue<long>("count");
        order.price() = object->getValue<long>("price");
        Poco::JSON::Object::Ptr object_array = object->getObject("address");
        order._address.city = object_array->getValue<std::string>("city");
        order._address.country = object_array->getValue<std::string>("country");
        order._address.street = object_array->getValue<std::string>("street");

        return order;
    }

    long PizzaOrder::get_id() const
    {
        return _id;
    }

    long PizzaOrder::get_user_id() const
    {
        return _user_id;
    }
    long PizzaOrder::get_count() const
    {
        return _count;
    }
    long PizzaOrder::get_price() const
    {
        return _price;
    }
    const std::string &PizzaOrder::get_type() const
    {
        return _type;
    }
    const std::string PizzaOrder::get_address_as_string() const
    {
        return _address.to_string();
    }
    const std::string PizzaOrder::Address::to_string() const
    {
        std::stringstream ss;
        ss << country << ", " << city << ", " << street;
        return ss.str();
    }
    long &PizzaOrder::id()
    {
        return _id;
    }
    long &PizzaOrder::user_id()
    {
        return _user_id;
    }
    long &PizzaOrder::count()
    {
        return _count;
    }
    long &PizzaOrder::price()
    {
        return _price;
    }
    std::string &PizzaOrder::type()
    {
        return _type;
    }
    PizzaOrder::Address &PizzaOrder::address()
    {
        return _address;
    }

    std::optional<PizzaOrder> PizzaOrder::read_by_id(long id)
    {
        std::optional<PizzaOrder> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("orders",params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<PizzaOrder> PizzaOrder::read_by_user_id(long user_id)
    {
        std::vector<PizzaOrder> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("orders",params);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    void PizzaOrder::add()
    {
        database::Database::get().send_to_mongo("orders",toJSON());
    }

    void PizzaOrder::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("orders",params,toJSON());
    }
    Poco::JSON::Object::Ptr PizzaOrder::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("type", _type);
        root->set("count", _count);
        root->set("price", _price);

        Poco::JSON::Object::Ptr address = new Poco::JSON::Object();

        address->set("country",_address.country);
        address->set("city",_address.city);
        address->set("street",_address.street);

        root->set("address", address);


        return root;
    }
}