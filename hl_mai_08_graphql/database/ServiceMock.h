#include <graphqlservice/JSONResponse.h>
#include <graphqlservice/GraphQLService.h>

#include "../GQL/GQLSchema.h"
#include "../GQL/QueryObject.h"
#include "../GQL/MutationsObject.h"
#include "../GQL/UserObject.h"


#include <atomic>
#include <memory>
#include <stack>
#include <string>

namespace graphql::database::object
{

    std::shared_ptr<graphql::service::Request> GetService();

    struct UserImpl
    {
        int _id = 0;
        std::string _first_name;
        std::string _last_name;
        std::string _email;
        std::string _title;
        std::string _login;
        std::string _password;

        void beginSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const
        {
        }
        void endSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const
        {
        }
        int getId([[maybe_unused]] service::FieldParams &&params) const
        {
            return _id;
        }
        std::string getFirst_name([[maybe_unused]] service::FieldParams &&params) const
        {
            return _first_name;
        }

        std::string getLast_name([[maybe_unused]] service::FieldParams &&params) const
        {
            return _last_name;
        }
        std::string getEmail([[maybe_unused]] service::FieldParams &&params) const
        {
            return _email;
        }
        std::string getTitle([[maybe_unused]] service::FieldParams &&params) const
        {
            return _title;
        }
        std::string getLogin([[maybe_unused]] service::FieldParams &&params) const
        {
            return _login;
        }
        std::string getPassword([[maybe_unused]] service::FieldParams &&params) const
        {
            return _password;
        }
    };

    struct QueryImpl
    {
        void beginSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const;
        void endSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const;
        std::shared_ptr<User> getGet_user(service::FieldParams&& params, std::optional<int> &&idArg) const;
        std::optional<std::vector<std::shared_ptr<User>>> getAll_users(service::FieldParams&& params) const;
        std::vector<std::shared_ptr<User>> getSearch(service::FieldParams&& params, std::string &&term1Arg, std::string &&term2Arg) const;
    };

    struct MutationsImpl
    {
        void beginSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const;
        void endSelectionSet([[maybe_unused]] const service::SelectionSetParams &params) const;
        std::string applyAdd_user(service::FieldParams&& params,
                                   std::string &&first_nameArg, std::string &&last_nameArg, std::string &&emailArg, std::string &&titleArg, std::string &&loginArg, std::string &&passwordArg) const;
    };
    

}