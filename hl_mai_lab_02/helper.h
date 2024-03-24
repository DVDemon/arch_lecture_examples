#pragma once
#include <string>
#include "Poco/Base64Decoder.h"
#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include <istream>
#include <ostream>

bool get_identity(const std::string identity, std::string &login, std::string &password)
{
    std::istringstream istr(identity);
    std::ostringstream ostr;
    Poco::Base64Decoder b64in(istr);
    copy(std::istreambuf_iterator<char>(b64in),
         std::istreambuf_iterator<char>(),
         std::ostreambuf_iterator<char>(ostr));
    std::string decoded = ostr.str();

    size_t pos = decoded.find(':');
    login = decoded.substr(0, pos);
    password = decoded.substr(pos + 1);
    return true;
}

std::string getJWTKey() {
    if (std::getenv("JWT_KEY") != nullptr) {
        return std::getenv("JWT_KEY");
    }
    return "0123456789ABCDEF0123456789ABCDEF";
}

std::string generate_token(long &id, std::string &login) {
    Poco::JWT::Token token;
    token.setType("JWT");
    token.setSubject("login");
    token.payload().set("login", login);
    token.payload().set("id", id);
    token.setIssuedAt(Poco::Timestamp());

    Poco::JWT::Signer signer(getJWTKey());
    return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
}

bool extract_payload(std::string &jwt_token, long &id, std::string &login) {
    if (jwt_token.length() == 0) {
        return false;
    }

    Poco::JWT::Signer signer(getJWTKey());
    try {
        Poco::JWT::Token token = signer.verify(jwt_token);
        if (token.payload().has("login") && token.payload().has("id")) {
            login = token.payload().getValue<std::string>("login");
            id = token.payload().getValue<long>("id");
            return true;
        }
        std::cout << "Not enough fields in token" << std::endl;

    } catch (...) {
        std::cout << "Token verification failed" << std::endl;
    }
    return false;
}