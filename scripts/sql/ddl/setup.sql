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
end
$$ language plpgsql;

create or replace function app.current_saldo() returns table(konto smallint, "current_timestamp" timestamp(0), new_haben integer, new_soll integer)
as $$
with old_saldo as (
    select konto, time_of_transfer, haben_in_cents as old_haben, soll_in_cents as old_soll, max(time_of_transfer) over (partition by konto) as latest_transfer
    from app.saldo
),
only_latest_transfers as (
    select * from old_saldo where time_of_transfer = latest_transfer
),
all_transactions_since_latest_transfer as (
	(select latest.konto as id, amount,
	case when b.source_konto_is_on_haben_side then 'haben' else 'soll' end as soll_or_haben,
	b.time_of_transfer
	from only_latest_transfers latest left join app.buchung b on b.source_konto = latest.konto
	where latest.latest_transfer < b.time_of_transfer)
	union all
  	(select bt.target_konto as id, bt.amount as amount,
  	case when b.source_konto_is_on_haben_side then 'soll' else 'haben' end as soll_or_haben,
  	b.time_of_transfer as time_of_transfer
  	from app.buchung_target bt join app.buchung b on bt.buchung_id = b.id
  	join only_latest_transfers latest on bt.target_konto = latest.konto
	where latest.latest_transfer < b.time_of_transfer)
),
all_combinations_of_kontoid_and_sollhaben as (
	select konto, soll_or_haben 
	from only_latest_transfers 
	cross join 
	(select *
	from (
	  values
	    ('soll'),
	    ('haben')
	) as t(soll_or_haben))
),
all_new_transactions_for_haben_and_soll as (
	select c.konto, c.soll_or_haben, coalesce(t.amount, 0) as amount
	from all_combinations_of_kontoid_and_sollhaben c
	left join all_transactions_since_latest_transfer t on c.konto = t.id and c.soll_or_haben = t.soll_or_haben
),
sums as (
	select konto, sum(amount), soll_or_haben 
	from all_new_transactions_for_haben_and_soll
	group by konto, soll_or_haben
),
sums_per_soll_and_haben as (
	select soll.konto, soll.sum as soll, haben.sum as haben 
	from 
	(select konto, sum from sums where soll_or_haben = 'soll') soll
	join 
	(select konto, sum from sums where soll_or_haben = 'haben') haben
	on soll.konto = haben.konto
),
updated_saldos as (
    select s.konto, current_timestamp, latest.old_haben + s.haben as new_haben, latest.old_soll + s.soll as new_soll
    from only_latest_transfers latest join sums_per_soll_and_haben s
    on latest.konto = s.konto
)
insert into app.saldo select * from updated_saldos returning *;
$$ language sql;

create or replace procedure app.create_konto(IN "name" varchar(30), OUT created_konto_id smallint)
language sql
as $$
    with created_konto as (
        insert into app.konto("name")
        values ("name") returning id
    )
    insert into app.saldo(konto) -- initializes with default 0 in both soll and haben
    select id from created_konto;

    -- return result
    select id from app.konto where "name" = create_konto."name";
$$;

