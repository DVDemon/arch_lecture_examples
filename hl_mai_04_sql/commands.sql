# create table
CREATE TABLE IF NOT EXISTS Author (id INT NOT NULL,first_name VARCHAR(256),last_name VARCHAR(256),email VARCHAR(256), title VARCHAR(1024));

# show tables
\dt+

# create sequence
CREATE SEQUENCE IF NOT EXISTS Ids START 1

SELECT * from Author;

select NEXTVAL('ids');

INSERT INTO Author(id,first_name,last_name,email,title) VALUES ( NEXTVAL('ids'),'Petr','Petrov','pp@yandex.r
u','mr');

INSERT INTO Author(id,first_name,last_name,email,title) VALUES ( NEXTVAL('ids'),'Andr','Ezhoff','pp@yandex.
ru','mr');

drop table Author;
select * from Author where id<3;
select * from Author where first_name='Petr';
select * from Author where first_name='Petr' OR first_name='Andr';

explain select * from Author where first_name='Petr' OR first_name='Andr';
explain select * from Author where id<3;

CREATE UNIQUE INDEX author_id ON Author (id);

\d Author

create index fn_ln on Author(first_name);

drop index fn_ln;

create index fn_ln on Author(first_name,last_name);

SELECT * FROM Author WHERE first_name LIKE 'P%' OR last_name LIKE 'E%';