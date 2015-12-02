create table cuentas(
        customer_id             integer primary key not null,
        saldo                   integer default 1000000 not null 
);

create table requests (
        id 				integer primary key autoincrement not null,
        customer_id		integer not null,
        transaction_id	varchar(20) not null,
        card_number		varchar(20) not null,
        amount			integer not null,
        status 			integer default 1 not null, 
        receive_date 	timestamp default current_timestamp not null,
        FOREIGN KEY(customer_id) REFERENCES cuentas(customer_id)
);


