# Пример по курсу Архитектуре программных систем
### GCC 11
* sudo apt install build-essential manpages-dev software-properties-common
* sudo add-apt-repository ppa:ubuntu-toolchain-r/test
* sudo apt update && sudo apt install gcc-11 g++-11

### Зависимости
* git clone https://github.com/Tencent/rapidjson.git
* git clone https://github.com/taocpp/PEGTL.git
* git clone --recurse-submodules https://github.com/microsoft/cppgraphqlgen.git

### Генерация схемы
/usr/local/bin/cppgraphqlgen/schemagen  -s database/schema.graphql -p GQL -n database

### implement stubs
* ServiceMock.h
* ServiceMock.cpp

Код подучения сервиса
```cpp
    std::shared_ptr<graphql::service::Request> GetService()
    {
        std::shared_ptr<Query> query = std::make_shared<Query>(std::make_shared<QueryImpl>());
        std::shared_ptr<Mutations> mutation = std::make_shared<Mutations>(std::make_shared<MutationsImpl>());
        auto service = std::make_shared<Operations>(std::move(query),std::move(mutation));
        return service;
    }
```

Код обработки http запроса

```cpp
        std::ostream &ostr = rsp.send();
        auto service = graphql::database::object::GetService();

        try
        {
            graphql::peg::ast query;

            std::istream_iterator<char> start{request.stream() >> std::noskipws}, end{};
            std::string input{start, end};

            query = graphql::peg::parseString(std::move(input));

            if (!query.root)
            {
                std::cerr << "Unknown error!" << std::endl;
                std::cerr << std::endl;
            }

            ostr << graphql::response::toJSON(service->resolve({query, ""}).get())
                 << std::endl;
        }
        catch (const std::runtime_error &ex)
        {
            std::cerr << "exception:" << ex.what() << std::endl;
        }
```

### Полезные ссылки про GraphQL

* https://graphql.org/graphql-js/language/
* https://spec.graphql.org/October2021/
* https://www.howtographql.com/basics/0-introduction/

### Схема

```graphql
schema {
  query: Query
  mutation: Mutations
}

type Mutations{
  add_user(first_name: String!,
            last_name: String!,
            email: String!,
            title: String!,
            login: String!,
            password: String!) : String!
}

type Query {
  get_user(id: Int): User
  all_users: [User]
  search(term1: String!,term2: String!): [User!]!
}

type User  {
  id: Int
  first_name:String!
  last_name:String!
  email:String!
  title:String!
  login:String!
  password:String!
}
```


### Примеры запросов

Пробуем из Postman

#### Поиск по маске

```graphql
query {
    search(term1:"I%", term2:"I%"){
        id,
        first_name,
        last_name,
        email,
        title,
        login,
        password
    }
}
```

#### Поиск всех

```graphql
query {
    all_users{
        id,
        first_name,
        last_name,
        email,
        title,
        login,
        password
    }
}
```

#### Поиск по id

```graphql
query {
    get_user(id:1){
        id,
        first_name,
        last_name,
        email,
        title,
        login,
        password
    }
}
```

#### Добавление 

```graphql
mutation{
    add_user(first_name: "Ivan",
                   last_name: "Ivanov",
                   email: "ii@yandex.ru",
                   title: "mr",
                   login: "ivanov",
                   password: "12345678")
}
```