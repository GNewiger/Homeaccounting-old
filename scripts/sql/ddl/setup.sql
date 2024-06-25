create type app.target_konto_split AS (
    konto_id smallint,
    amount integer,
    description varchar(100)
);

create or replace procedure app.buchen(
    source_konto_id smallint, 
    source_konto_is_on_haben_side boolean,
    time_of_transfer timestamp(0),
    source_description varchar(100),
    variadic target_konto_split_arr app.target_konto_split[]
    ) as $$
declare
    buchung_id integer;
    sum integer := 0;
    split_buchung app.target_konto_split;
begin
    select sum(amount) into sum from unnest(target_konto_split_arr) as targets(konto_id, amount, description);
    insert into app.buchung(amount, source_konto, source_konto_is_on_haben_side, description, time_of_transfer) 
	    values (sum, source_konto_id, source_konto_is_on_haben_side, source_description, time_of_transfer) returning id into buchung_id;

    foreach split_buchung in ARRAY target_konto_split_arr
    loop
        insert into app.buchung_target(buchung_id, amount, target_konto, description) 
	    values (buchung_id, split_buchung.amount, split_buchung.konto_id, split_buchung.description);
    end loop;

    if source_konto_is_on_haben_side then
        update app.saldo set soll_in_cents = soll_in_cents + sum where konto = any(select konto_id from unnest(target_konto_split_arr) as targets(konto_id, amount, description));
        update app.saldo set haben_in_cents = haben_in_cents + sum where konto = source_konto_id;
    else
        update app.saldo set haben_in_cents = haben_in_cents + sum where konto = any(select konto_id from unnest(target_konto_split_arr) as targets(konto_id, amount, description));
        update app.saldo set soll_in_cents = soll_in_cents + sum where konto = source_konto_id;
    end if;
end
$$ language plpgsql;

create or replace procedure app.create_konto(IN "name" varchar(30), OUT created_konto_id smallint)
language sql
begin atomic
    with created_konto as (
        insert into app.konto("name")
        values ("name") returning id
    )
    insert into app.saldo(konto) -- initializes with default 0 in both soll and haben
    select id from created_konto;

    -- return result
    select id from app.konto where "name" = create_konto."name";
end;