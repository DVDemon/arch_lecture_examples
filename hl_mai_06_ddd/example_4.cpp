#include <iostream>
#include <string>
#include <exception>

/*
    Фабрика в аггрегате
*/
struct Entity{};
struct Author{};

struct ForumIdentityService{
    std::string nextDiscussionId(){
        return std::string();
    };
};

struct Discussion{
    Discussion(const std::string&,const std::string&,const std::string&,const Author&,const std::string&){};
};

struct Forum : public Entity
{
    std::string tenant(){
        return "current_tennant";
    }

    std::string forumId(){
        return "forum id";
    }

    Discussion startDiscussionFor(
        std::string aSubject,
        const Author& anAuthor,
        ForumIdentityService& aForumIdentityService)
    {

        return Discussion(
                tenant(),
                forumId(),
                aForumIdentityService.nextDiscussionId(),
                anAuthor,
                aSubject);
    }
};

auto main() -> int
{
    return 0;
}