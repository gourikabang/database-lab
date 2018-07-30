create database 25jan2018;
use 25jan2018;
/*As start time < end_ time so we have to check that*/
create table ett(course_id varchar(7) not null,exam_date date not null,start_time time not null,end_time time not null,check(start_time < end_time));
/*Number of credits between 0 & 8 and if not specified default 6*/
create table cc(course_id varchar(7),number_of_credits int default 6,primary key(course_id),check(number_of_credits between 0 and 8));
/*email should be unique*/
create table cwsl(cid varchar(7) not null,serial_number int,roll_number varchar(12),name varchar(40) not null,email varchar(30) not null);
create temporary table ett_temp(course_id varchar(7) not null,exam_date date not null,start_time time not null,end_time time not null,check(start_time < end_time));
create temporary table cc_temp(course_id varchar(7),number_of_credits int default 6,primary key(course_id),check(number_of_credits between 0 and 8));
create temporary table cwsl_temp(cid varchar(7) not null,serial_number int,roll_number varchar(12),name varchar(40) not null,email varchar(30) not null);
create table ett_clone LIKE ett;
create table cc_clone LIKE cc;
create table cwsl_clone LIKE cwsl;

/* primary keys- course_id in cc,cc_temp,cc_clone table */