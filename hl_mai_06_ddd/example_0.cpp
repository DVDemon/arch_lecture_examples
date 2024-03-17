#include <iostream>
#include <string>
#include <exception>

/*
    Антипример использования DDD
*/

class User
{
private:
    std::string user_name;
    std::string password;
    int auth_retry_count = 0;
    bool account_is_locked = false;

public:
    std::string &UserName() { return user_name; };
    std::string &Password() { return password; };
    int &AuthAccountRetry() { return auth_retry_count; };
    bool &AccountIsLocked() { return account_is_locked; };

    const std::string &GetUserName() { return user_name; };
    const std::string &GetPassword() { return password; };
    int GetAuthAccountRetry() { return auth_retry_count; };
    bool GetAccountIsLocked() { return account_is_locked; };
};

class UserSecurityService
{
private:
    const int AuthRetryCountLimit = 5;

public:
    bool AuthenticateUser(const std::string &userName, const std::string &password)
    {
        User user; // Зашружаем откуда-то пользователя

        if (user.GetAccountIsLocked())
            throw std::logic_error("User account is locked.");
        std::string hashedPassword; // = _passwordEncryptor.EncryptPassword(password);
        if (user.GetPassword() == hashedPassword)
        {
            user.AuthAccountRetry() = 0;
            return true;
        }
        if (user.GetPassword() != hashedPassword)
            user.AuthAccountRetry()++;
        if (user.GetAuthAccountRetry() == AuthRetryCountLimit)
            user.AccountIsLocked() = true;

        // Записываем куда-то пользователя
        return false;
    }
};

auto main() -> int
{
    return 0;
}