delimiter //
create procedure count_credits()
begin
    declare finished int default 0;
    declare r char(20);
    declare n char(50);
    declare c int;

    declare cur cursor for
    select A.name, B.roll, B.sum
    from (select distinct name, roll from cswl) as A
    join
    (select S.roll as roll , sum(credits) as sum
     from cswl as S
     join course_credits as C
     on C.cid = S.cid
     group by S.roll) as B
    on A.roll = B.roll;

    declare continue handler for not found set finished = 1;


    create table answer(
        roll char(20),
        name char(50),
        credits int
    );

    open cur;
    loop1 : loop
        fetch cur into n, r, c;
        if finished = 1
        then
            leave loop1;
        end if;
        if c > 40
        then
            insert into answer values (r, n, c);
        end if;
    end loop loop1;
    close cur;

    select * from answer;
    drop table answer;
end//
delimiter ;

call count_credits();
drop procedure count_credits;