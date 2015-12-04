create table requests (

        id 				integer primary key autoincrement not null,
        id_cliente		integer not null,
        id_transac	varchar(20) not null,
        numtarjeta		varchar(20) not null,
        monto			integer not null,
        status 			integer default 0 not null, 
        receive_date 	timestamp default current_timestamp not null

);

create table cards(
	id 			integer primary key autoincrement not null,
	numtarjeta varchar(20) not null,
        id_cliente             integer not null,
	cash integer not null
);
