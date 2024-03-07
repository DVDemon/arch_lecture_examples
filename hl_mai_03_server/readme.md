## Примеры по курса Архитектура программных систем

Простой сервер с использованием Poco https://github.com/pocoproject/poco


###   Пример запроса

http://localhost:8080/request?session_id=10


## Созданеи докер-образа

docker build . -t my_server
docker images
sudo docker run -t -i my_server /bin/bash
sudo docker run -p 8080:8080 my_server