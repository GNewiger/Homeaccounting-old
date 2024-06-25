DO $$ 
DECLARE
    bafoeg_id app.konto.id%TYPE;
   	gehalt_id app.konto.id%TYPE;
begin
	truncate app.konto cascade;
    call app.create_konto('bafoeg', bafoeg_id);
    call app.create_konto('gehalt', gehalt_id);
   
    call app.buchen(gehalt_id, true, '2024-06-25 21:25:51.627 +0200', 'source desc', row(bafoeg_id, 200, 'target desc')::app.target_konto_split);
END $$