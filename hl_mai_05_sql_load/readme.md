## Пример по курсу Архитектура программныз систем

Пример показывает загрузку данных в реляционную СУБД из сгенерированного JSON файла. Полученная СУБД может применяться для тестирования



## Использование
python3 -m venv ./env
source ./env/bin/activate
pip3 install -r requirements.txt
docker-compose up -d
python3 load_json.py
deactivate
mysql -h 127.0.0.1 -P 3360 -u stud -p
<!-- docker-compose stop
docker-compose rm -->