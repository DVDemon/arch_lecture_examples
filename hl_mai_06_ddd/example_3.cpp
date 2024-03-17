#include <iostream>
#include <string>
#include <exception>


/*
    Пример применения принципов DDD
*/

struct Address{};
struct Telephone{};
struct EmailAddress{};

struct Client {
    
     void changeName(const std::string& firstName, const std::string&  lastName) {}
     void relocateTo(const Address& address) {}
     void changePhoneNumber(const Telephone& telephone){}
     void changeEmailAddress(const EmailAddress& emailAddress){}
};
struct ClientService {
    void changeClientPhoneNumber(const std::string& clientId, const Telephone& telephone) {
        Client client; // = clientRepository.getById(clientId);
        client.changePhoneNumber(telephone);
    }
};

auto main() -> int {
    return 0;
}