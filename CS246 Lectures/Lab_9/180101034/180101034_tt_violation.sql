create table clash as
(select A.cid as c1, B.cid as c2
from ett as A, ett as B
where A.edate = B.edate and A.cid > B.cid and ((A.stime <= B.stime and B.stime <= A.etime) or (B.stime <= A.stime and A.stime <= B.etime)));

delimiter //
create procedure tt_violation()
begin
    declare finished int default 0;
    declare r1 char(20);
    declare cc1 char(6);
    declare cc2 char(6);

    declare cur cursor for
    select A.roll, A.cid, B.cid
    from (select distinct roll, cid from cswl) as A, 
    (select distinct roll, cid from cswl) as B 
    where A.roll = B.roll and (A.cid, B.cid) in (select * from clash);


    declare continue handler for not found set finished = 1;

    create table answer2(
        roll char(20),
        cs1 char(6),
        cs2 char(6)
    );

    open cur;
    loop1 : loop
        fetch cur into r1, cc1, cc2;
        if finished = 1
        then
            leave loop1;
        end if;
        insert into answer2 values (r1, cc1, cc2);
    end loop loop1;
    close cur;

    select * from answer2;

end //
delimiter ;

call tt_violation();

drop table clash;
drop table answer2;
drop procedure tt_violation;