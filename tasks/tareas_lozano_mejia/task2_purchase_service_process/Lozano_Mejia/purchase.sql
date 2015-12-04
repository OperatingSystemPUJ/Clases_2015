create table requests (

        id 				integer primary key autoincrement not null,

        customer_id		integer not null,

        transaction_id	varchar(20) not null,

        card_number		varchar(20) not null,

        amount			integer not null,

        status 			integer default 0 not null, 

        receive_date 	timestamp default current_timestamp not null

);

-- insert into requests(customer_id, transaction_id, card_number, amount) values(1,'1','1',200);
-- insert into requests(customer_id, transaction_id, card_number, amount) values(2,'2','2',600);
-- insert into requests(customer_id, transaction_id, card_number, amount) values(2,'2','2',400);
-- insert into requests(customer_id, transaction_id, card_number, amount) values(3,'3','2',300);


create table cards(
		id 			integer primary key autoincrement not null,
		card_number varchar(20) not null,
                customer_id             integer not null,
		cash integer not null
);

insert into cards(card_number,customer_id, cash) values ('1',1,15000);
insert into cards(card_number,customer_id, cash) values ('2',2,55000);
insert into cards(card_number,customer_id, cash) values ('3',3,25000);