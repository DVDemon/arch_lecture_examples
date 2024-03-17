#include <iostream>
#include <string>
#include <exception>

/*
    Антипример использования DDD
*/

struct Client{
    void setClientId(const std::string &) {};
   void setClientFirstName(const std::string &) {};
   void setClientLastName(const std::string &) {};
   void setStreetAddress(const std::string &) {};
   void setPhoneNumber(const std::string &) {};
   void setEmailAddress(const std::string &) {};
};

void saveClient(const std::string& client_id, 
                const std::string& client_first_name, 
                const std::string& client_last_name, 
                const std::string& street_address, 
                const std::string& phone_number, 
                const std::string& email_address) {
    
    Client client; // = clientDao.readClient(client_id);
     
    if (!client_first_name.empty()) client.setClientFirstName(client_first_name);
    if (!client_last_name.empty()) client.setClientLastName(client_last_name);
    if (!street_address.empty()) client.setStreetAddress(street_address);
    if (!phone_number.empty())  client.setPhoneNumber(phone_number);
    if (!email_address.empty()) client.setEmailAddress(email_address);
    
    //clientDao.saveClient(client);
}
auto main() -> int {
    return 0;
}