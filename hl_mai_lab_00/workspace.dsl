workspace {
    name "Умный дом"
    description "Простая система управления умного дома с базовыми функциями контроля состояния атмосферы в помещении"

    # включаем режим с иерархической системой идентификаторов
    !identifiers hierarchical

    !docs documentation
    !adrs decisions
    # Модель архитектуры
    model {

        # Настраиваем возможность создания вложенных груп
        properties { 
            structurizr.groupSeparator "/"
        }
        

        # Описание компонент модели
        user = person "Пользователь умного дома"
        sensor     = softwareSystem "Датчик температуры"
        smart_home = softwareSystem "Умный дом" {
            description "Сервер управления умным домом"

            user_service = container "User service" {
                description "Сервис управления пользователями"
            }

            temperature_service = container "Temperature service" {
                description "Сервис мониторинга и управления температурой в доме"
            }

            group "Слой данных" {
                user_database = container "User Database" {
                    description "База данных с пользователями"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                user_cache = container "User Cache" {
                    description "Кеш пользовательских данных для ускорения аутентификации"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                smarthome_database = container "Smarthome Database" {
                    description "База данных для хранения информации с сенсоров"
                    technology "MongoDB 5"
                    tags "database"
                }
            }

            user_service -> user_cache "Получение/обновление данных о пользователях" "TCP 6379"
            user_service -> user_database "Получение/обновление данных о пользователях" "TCP 5432"

            temperature_service -> smarthome_database "Получение/обновление данных о температуре" "TCP 27018"
            temperature_service -> user_service "Аутентификация пользователя" "REST HTTP 443"

            user -> user_service "Регистрация нового пользователя" "REST HTTP:8080"
            sensor -> temperature_service "Получение данных о температуре в доме" "REST HTTP:8080"
        }

        user -> smart_home "Управление устройствами умного дома"
        sensor -> smart_home "Обновление актуальных данных о температуре в доме" "REST HTTP:8080"

        deploymentEnvironment "Production" {
            deploymentNode "User Server" {
                containerInstance smart_home.user_service
            }

            deploymentNode "Temperature Server" {
                containerInstance smart_home.temperature_service
                properties {
                    "cpu" "4"
                    "ram" "256Gb"
                    "hdd" "4Tb"
                }
            }

            deploymentNode "databases" {
     
                deploymentNode "Database Server 1" {
                    containerInstance smart_home.user_database
                }

                deploymentNode "Database Server 2" {
                    containerInstance smart_home.smarthome_database
                    instances 3
                }

                deploymentNode "Cache Server" {
                    containerInstance smart_home.user_cache
                }
            }
            
        }
    }

    views {
        themes default

        properties { 
            structurizr.tooltips true
        }


        !script groovy {
            workspace.views.createDefaultViews()
            workspace.views.views.findAll { it instanceof com.structurizr.view.ModelView }.each { it.enableAutomaticLayout() }
        }

        dynamic smart_home "UC01" "Добавление нового пользователя" {
            autoLayout
            user -> smart_home.user_service "Создать нового пользователя (POST /user)"
            smart_home.user_service -> smart_home.user_database "Сохранить данные о пользователе" 
        }

        dynamic smart_home "UC02" "Удаление пользователя" {
            autoLayout
            user -> smart_home.user_service "Удалить нового пользователя (DELETE /user)"
            smart_home.user_service -> smart_home.user_database "Удалить данные о пользователе" 
        }

        dynamic smart_home "UC03" "Сохранить данные о температуре" {
            autoLayout
            sensor -> smart_home.temperature_service "Сохранить данные о температуре (POST /user)"
            smart_home.temperature_service -> smart_home.smarthome_database "Сохранить данные о температуре" 
        }

        dynamic smart_home "UC04" "Получить данные о температуре" {
            autoLayout
            sensor -> smart_home.temperature_service "Получить данные о температуре (GET /user)"
            smart_home.temperature_service -> smart_home.user_service "Проверить аутентификацию пользователя (GET /user)"
            smart_home.temperature_service -> smart_home.smarthome_database "Получить данные о температуре" 
        }


        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}