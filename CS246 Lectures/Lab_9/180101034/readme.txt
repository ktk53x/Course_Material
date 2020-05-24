First create table using the 180101034_table_creation
then run the bash script import.sh to load the tables
using ./import.sh (make sure to have chmod +x import.sh)
then exectute the 180101034_tt_violation.sql
and count_credits.sql

Also there is one roll number with two names in the database
And there are few courses in which are not in course_credit but are there in the ett table

I got 17 violations
and 1082 people with > 40 credit