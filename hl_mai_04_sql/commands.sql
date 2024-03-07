use archdb; 
show tables;

CREATE SEQUENCE ids START WITH 1 INCREMENT BY 1;

select NEXTVAL(ids);

CREATE TABLE IF NOT EXISTS User 
    (`id` INT NOT NULL AUTO_INCREMENT,
    `first_name` VARCHAR(256),
    `last_name` VARCHAR(256),
    `email` VARCHAR(256),
    `title` VARCHAR(1024) ,
    PRIMARY KEY (`id`),
    KEY `fn` (`first_name`),
    KEY `ln` (`last_name`));

describe User;
drop table User;
select * from User where id<10;
select id,first_name,last_name from User where id>=10 and id<20;
explain select * from User where id=10;
explain select * from User where first_name='Carl';
explain select * from User where title='Doctor';
explain select * from User where id>10 and id<20;
explain select * from User where first_name like 'Car%';
-- insert into User (first_name,last_name,email,title) values ('Иван','Иванов','ivanov@yandex.ru','господин');
-- SELECT LAST_INSERT_ID();
-- select * from User where id=LAST_INSERT_ID();
-- delete from User where id= 100001;
show index from User;
explain select * from User where first_name like 'Ca%' and last_name like 'V%';
--explain format=json select * from User where first_name='Elle%' and last_name='A%';
create index fn_ln on User(first_name);
explain select * from User where first_name like 'Ca%' and last_name like 'V%';
drop index fn_ln on User;

