CREATE TABLE orders (
	order_id SERIAL,
	order_date DATE NOT NULL,
	customer_name VARCHAR(255),
	product_name VARCHAR(255),
	quantity INT
) PARTITION BY RANGE(EXTRACT(YEAR FROM order_date));

CREATE TABLE orders_2019 PARTITION OF orders FOR VALUES FROM (2019) TO (2020);
CREATE TABLE orders_2020 PARTITION OF orders FOR VALUES FROM (2020) TO (2021);
CREATE TABLE orders_2021 PARTITION OF orders FOR VALUES FROM (2021) TO (2022);

INSERT INTO orders (order_date, customer_name, product_name, quantity)
VALUES ('2019-01-01', 'John Smith', 'Product A', 10),
	('2019-02-15', 'Jane Doe', 'Product B', 5),
	('2019-04-20', 'Bob Johnson', 'Product C', 2),
	('2019-07-10', 'Alice Brown', 'Product A', 7),
	('2019-12-30', 'Mike Wilson', 'Product B', 8);

INSERT INTO orders (order_date, customer_name, product_name, quantity)
VALUES ('2020-02-14', 'John Smith', 'Product A', 15),
	('2020-03-20', 'Jane Doe', 'Product B', 3),
	('2020-06-05', 'Bob Johnson', 'Product C', 10),
	('2020-08-15', 'Alice Brown', 'Product A', 5),
	('2020-11-25', 'Mike Wilson', 'Product B', 2);

SELECT COUNT(*) FROM orders;