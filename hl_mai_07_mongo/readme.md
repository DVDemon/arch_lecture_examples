
# mongo – консоль управления (на сервере, где установлен mongodb)
# показывает базы данных
show dbs 
# подключается (и создает если надо) базу данных
use shop
# создадим коллекцию и добавляет элементы
db.products.insertOne({name:"A Book", price: 29, details: {pages: 500}})
# выведем все данные коллекции и отформатирует
db.products.find().pretty()

# выведем первый элемент
db.products.findOne()

# выведем определенные данные все с name “A Book”
db.products.find({name:"A Book"}).pretty()

# выведем определенные данные все что дороже 20
db.products.find({price:{$gt:20}}).pretty()

# изменим объект updateOne / updateMany

db.products.updateOne({_id:ObjectId("65fffa53e5acdb15e9d2d884")},{$set: {price: 1000}})

# удалим объект delete/ deleteOne
db.products.deleteOne({_id:ObjectId("65fffa53e5acdb15e9d2d884")})

# вставим разных элементов
db.products.insertMany([{name:"A Book", price: 29, details: {pages: 500}},
{name:"B Book", price: 9, details: {pages: 500}},
{name:"A Book", price: 2, details: {pages: 510}},
{name:"A Book", price: 10, details: {pages: 50}},
{name:"A Book", price: 27, details: {pages: 250}},
{name:"F Book", price: 35, details: {pages: 35}}])

# выведем первый элемент
db.products.findOne()

# получает крсор по которому можно перемещаться с помощью it
db.products.find({name:"A Book"}).pretty()

# возвращает сразу все данные
db.products.find({name:"A Book"}).toArray()

# применяет к каждому элементу команду
db.products.find({name:"A Book"}).forEach((prod)=>{printjson(prod)})

# возвращает нужные данные (name)
db.products.find({},{name:1}).pretty()

# мегасложный объект (по умолчанию используется NodeJs driver)
db.companies.insertOne({ name: "Pche Lines Inc", isStartup: true, employees: 33, funding: 12231241242141, details: { ceo: "Max Steal"}, tags :[ { title: "super"} , {title : "prefect"}], foundingDate: new Date('2014-01-01'), insertedAt:new Timestamp()})

# зададим id руками
db.users.insertOne({ _id: "user-01", name: "Ivan Petrov"})
db.users.findOne()
db.orders.insertOne({ time: new Timestamp() , details : "Sone description" , user : "user-01" })
db.orders.findOne()

# Аггрегация

db.orders.aggregate([{ $lookup: { from: "users" , localField: "user" , foreignField : "_id" , as: "order_users"}}]).pretty()

# Используем схему

db.createCollection('posts', {
  validator: {
    $jsonSchema: {
      bsonType: 'object',
      required: ['title', 'text', 'creator', 'comments'],
      properties: {
        title: {
          bsonType: 'string',
          description: 'must be a string and is required'
        },
        text: {
          bsonType: 'string',
          description: 'must be a string and is required'
        },
        creator: {
          bsonType: 'objectId',
          description: 'must be an objectid and is required'
        },
        comments: {
          bsonType: 'array',
          description: 'must be an array and is required',
          items: {
            bsonType: 'object',
            required: ['text', 'author'],
            properties: {
              text: {
                bsonType: 'string',
                description: 'must be a string and is required'
              },
              author: {
                bsonType: 'objectId',
                description: 'must be an objectid and is required'
              }
            }
          }
        }
      }
    }
  }
})

# Правильно
db.posts.insertOne({title: "Some title", text: "Some text", creator: ObjectId("65d9ac4ff5793bcbf97d988f"), comments: [{text: "Coool!" , author: ObjectId("65d9ac4ff5793bcbf97d988f")}]})

# Неправильно 
db.posts.insertOne({title: "Some title", text: "Some text", comments: [{text: "Coool!" , author: ObjectId("65d9ac4ff5793bcbf97d988f")}]})

# Анализ запросов
use contacts
db.contacts.find({"dob.age" : { $gt: 60}}).pretty()
db.contacts.explain().find({"dob.age" : { $gt: 60}}) 
db.contacts.explain("executionStats").find({"dob.age" : { $gt: 60}}) 

# создаем индекс
db.contacts.createIndex({"dob.age": -1})
db.contacts.explain("executionStats").find({"dob.age" : { $gt: 60}}) 

db.contacts.dropIndex({"dob.age": -1}) # удалим индекс
db.contacts.createIndex({"dob.age": 1, gender: 1}) # "dob.age_1_gender_1"
db.contacts.explain("executionStats").find({"dob.age" : 35, "gender" : "male" }) # использует
db.contacts.explain("executionStats").find({"dob.age" : { $gt: 20}}) # использует
db.contacts.explain("executionStats").find({"gender" : "male"}) # не использует
db.contacts.getIndexes() # возвращает перечень индексов

# text index
db.contacts.createIndex({"location.street": "text"})
db.contacts.explain("executionStats").find({$text : {$search : "road"}})
# по одному слову
db.contacts.find({$text : {$search : "road"}}).count() 
# по вхождению одного из слов
db.contacts.find({$text : {$search : "road street"}}).count() 
# по вхождению фразы
db.contacts.find({$text : {$search : "\"road street\""}}).count()

# Аггрегация
db.contacts.aggregate([{$match : {gender : "male"} }, {$group : { _id:"$nat" , totalAge : {$avg : "$dob.age"}}}, { $sort : { totalAge : -1} }])