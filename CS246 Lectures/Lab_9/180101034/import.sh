cd /home/ktk53x/Downloads/database-09nov2019/
ls
mysql -e "use 180101034_19may2020; load data local infile 'exam-time-table.csv' into table ett FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\n';" -u root
mysql -e "use 180101034_19may2020; load data local infile 'course-credits.csv' into table course_credits FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\n';" -u root
cd /home/ktk53x/Downloads/database-09nov2019/course-wise-students-list/
ls
for f in */*.csv
do
    mysql -e "use 180101034_19may2020; load data local infile '"$f"' into table cswl FIELDS TERMINATED BY ','  ENCLOSED BY '\"' LINES TERMINATED BY '\n' set cid = substring_index(substring_index('"$f"', '/', -1), '.', 1) ;" -u root
done
