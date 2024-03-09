import pandas as pd
from sqlalchemy import create_engine
engine = create_engine("postgresql+psycopg2://stud:stud@postgres/archdb", echo = False)

df = pd.read_json("ExportJson.json")
df.to_sql("users", con=engine, if_exists = 'replace', index=False)

# Добавим уникальный идентификатор
# ALTER TABLE "users" ADD COLUMN id SERIAL PRIMARY KEY
# UPDATE "users" SET login=email
# explain select * from users where first_name='Alan';
# create index fnu on users(first_name);