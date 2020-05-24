create database 180101034_19may2020;
use 180101034_19may2020;

create table ett(
    cid char(6),
    edate date,
    stime time,
    etime time,
    line int not null auto_increment,
    primary key(line)
);

create table course_credits(
    cid char(6),
    credits int,
    primary key(cid)
);

create table cswl(
    line int,
    roll char(20),
    name char(50),
    email char(50),
    cid char(6),
    primary key(roll, cid),
    foreign key(cid) references course_credits(cid)
);

