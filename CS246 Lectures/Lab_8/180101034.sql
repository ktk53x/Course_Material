/*
ASSUMPTIONS:
1 - If in 4b total number of students is 200 then if a new student is entered into the students table
then he/she is not added at all
2 - There are total 8 semesters, there is no 9th semester
3 - Since it is mentioned that all even roll numbered students in 4c so I have registered all the students
    with even roll number regardless of their year
*/


CREATE DATABASE lab8;
USE lab8;

CREATE TABLE courses(
    cid CHAR(7),
    cname VARCHAR(22),
    PRIMARY KEY (cid)
);


CREATE TABLE students(
    roll INT,
    sname VARCHAR(22),
    PRIMARY KEY (roll)
);


CREATE TABLE registers(
    roll INT,
    cid CHAR(7),
    PRIMARY KEY (roll, cid),
    FOREIGN KEY (roll) REFERENCES students(roll)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (cid) REFERENCES courses(cid)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);


LOAD DATA LOCAL INFILE  '/home/ktk53x/students.csv' 
INTO TABLE students  
FIELDS TERMINATED BY ','  
ENCLOSED BY '"' 
LINES TERMINATED BY '\n' 
IGNORE 1 ROWS;

LOAD DATA LOCAL INFILE  '/home/ktk53x/courses.csv' 
INTO TABLE courses  
FIELDS TERMINATED BY ','  
ENCLOSED BY '"' 
LINES TERMINATED BY '\n' 
IGNORE 1 ROWS;

LOAD DATA LOCAL INFILE  '/home/ktk53x/registers.csv'
INTO TABLE registers 
FIELDS TERMINATED BY ','  
ENCLOSED BY '"' 
LINES TERMINATED BY '\n' 
IGNORE 1 ROWS;


DELIMITER //
CREATE TRIGGER trigger1
AFTER INSERT 
ON students
    FOR EACH ROW
    BEGIN
        DECLARE NO_records INT DEFAULT 0;
        DECLARE c CHAR(7);
        DECLARE courses_cursor CURSOR FOR
        SELECT cid
        FROM courses;
        DECLARE CONTINUE HANDLER FOR NOT FOUND SET NO_records = 1;

        OPEN courses_cursor;
        loop_1 : REPEAT
            FETCH courses_cursor INTO c;
            IF NO_records = 1
            THEN
                LEAVE loop_1;
            END IF;
            INSERT INTO registers VALUES (NEW.roll, c);
            UNTIL NO_records
        END REPEAT;
        CLOSE courses_cursor;
    END
//

CREATE TRIGGER trigger2
BEFORE INSERT
ON registers
    FOR EACH ROW
    BEGIN
        IF(NEW.cid = 'CS245')
        THEN
            SET @cnt = (SELECT COUNT(*) FROM registers WHERE cid = 'CS245');
            if(@cnt > 199)
            THEN
                SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'CS245 course limit reached';
            END IF;
        END IF;
    END
//

CREATE TRIGGER trigger3
AFTER INSERT
ON courses
    FOR EACH ROW
    BEGIN
        DECLARE NO_records INT DEFAULT 0;
        DECLARE r INT DEFAULT 0;
        DECLARE students_cursor CURSOR FOR
        SELECT roll
        FROM students;
        DECLARE CONTINUE HANDLER FOR NOT FOUND SET NO_records = 1;
        
        IF(SUBSTRING(NEW.cid, 1, 3) = 'HSS')
        THEN
            IF(SUBSTRING(NEW.cid, 4, 1) IN('2', '4', '6', '8'))
            THEN
                IF(SUBSTRING(NEW.cid, 6, 1) IN('1', '3', '5', '7', '9'))
                THEN
                    OPEN students_cursor;
                    loop_1 : REPEAT
                        FETCH students_cursor INTO r;
                        IF NO_records = 1
                        THEN
                            LEAVE loop_1;
                        END IF;
                        IF(r % 2 = 0)
                        THEN
                            INSERT INTO registers VALUES (r, NEW.cid);
                        END IF;
                        UNTIL NO_records
                    END REPEAT;
                    CLOSE students_cursor;
                END IF;
            END IF;
        END IF;
    END
//

CREATE TRIGGER trigger4
AFTER INSERT
ON courses
    FOR EACH ROW
    BEGIN
        DECLARE NO_records INT DEFAULT 0;
        DECLARE r INT DEFAULT 0;
        DECLARE students_cursor CURSOR FOR
        SELECT roll
        FROM students;
        DECLARE CONTINUE HANDLER FOR NOT FOUND SET NO_records = 1;
        
        IF(SUBSTRING(NEW.cid, 1, 3) = 'HSS')
        THEN
            IF(SUBSTRING(NEW.cid, 4, 1) IN('1', '3', '5', '7'))
            THEN
                IF(SUBSTRING(NEW.cid, 6, 1) IN('0', '2', '4', '6', '8'))
                THEN
                    OPEN students_cursor;
                    loop_1 : REPEAT
                        FETCH students_cursor INTO r;
                        IF NO_records = 1
                        THEN
                            LEAVE loop_1;
                        END IF;
                        IF(r % 2 = 1)
                        THEN
                            INSERT INTO registers VALUES (r, NEW.cid);
                        END IF;
                        UNTIL NO_records
                    END REPEAT;
                    CLOSE students_cursor;
                END IF;
            END IF;
        END IF;
    END
//
DELIMITER ;