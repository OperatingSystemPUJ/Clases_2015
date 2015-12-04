create table requests (
        id 				integer primary key autoincrement not null,
        customer_id		integer not null,
        transaction_id	varchar(20) not null,
        card_number		varchar(20) not null,
        amount			integer not null,
        status 			integer default 0 not null, 
        receive_date 	timestamp default current_timestamp not null
);
