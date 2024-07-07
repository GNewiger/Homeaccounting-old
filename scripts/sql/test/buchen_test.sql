create or replace procedure test.run_unit_tests()
language plpgsql
as $$
DECLARE
    bafoeg_id app.konto.id%TYPE;
    versicherung_id app.konto.id%TYPE;
   	gehalt_id app.konto.id%TYPE;
begin
    -- general setup
    truncate test.test_result cascade;
    truncate test.test_error cascade;
    truncate app.konto restart identity cascade;

    call app.create_konto('bafoeg', bafoeg_id);
    call app.create_konto('versicherung', versicherung_id);
    call app.create_konto('gehalt', gehalt_id);

    -- test execution
    call app.test_buchen_source_haben(bafoeg_id, versicherung_id, gehalt_id);
    --call app.test_buchen_source_soll();

    -- cleanup
    --truncate app.konto cascade;
end$$;

create or replace procedure app.test_buchen_source_haben(bafoeg_id smallint, versicherung_id smallint, gehalt_id smallint)
language plpgsql
as $$
DECLARE
    name varchar(50);
begin
    name := 'Split onto several soll-konten';
    truncate app.buchung cascade;
    truncate app.buchung_target cascade;
    call app.buchen(gehalt_id, true, '2024-06-25 21:25:51.627 +0200', 'source desc', row(bafoeg_id, 100, 'target desc')::app.target_konto_split, row(versicherung_id, 50, 'target desc')::app.target_konto_split);
    
    with only_one_buchung as (
        select count(*) != 1 as is_error, 'Expected count of buchung to be 1, actual: ' || count(*) as error_message from app.buchung
    ),
    source_has_right_amount as (
        select amount != 150 as is_error, 'Expected amount to be 150, actual: ' || amount as error_message from app.buchung
    ),
    source_is_haben as (
        select not source_konto_is_on_haben_side as is_error, 'Expected source buchung to be haben, actual: soll' as error_message from app.buchung
    ),
    result as (
        insert into test.test_result(test_name, passed)
        select name, case
            when exists
            (
                select is_error from only_one_buchung where is_error = true
                union all
                select is_error from source_has_right_amount where is_error = true
                union all
                select is_error from source_is_haben where is_error = true
            )
            then false
            else true
            end
    )
    insert into test.test_error(test_name, error_message)
        select name, error_message from only_one_buchung where is_error
        union all
        select name, error_message from source_has_right_amount where is_error
        union all
        select name, error_message from source_is_haben where is_error;
end$$;