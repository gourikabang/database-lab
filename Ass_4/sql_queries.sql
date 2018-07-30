create database 09feb2018;
use 09feb2018;

/*Course_id and division are made primary key as first year courses have different divisions*/
create table Course(
	course_id varchar(10),
	division enum('I','II','III','IV','NA') default 'NA',
	primary key(course_id,division)
	);

/*Department id is made primary key to identify each department uniquely.
	Name is not null because a department always has a name*/
create table Department(
	department_id varchar(10),
	name varchar(40) not null,
	primary key(department_id)
	);

/*As classes are repeated weekly so, class in which slot on which day is required. 
Therefore, letter and day are made primary key
We also need to check start_time < end_time, check() is not implemented in mysql*/
create table Slot(
	letter enum('A','B','C','D','E','F','G','H','I','J','K','L','A1','B1','C1','D1','E1'),
	day enum ('Monday','Tuesday','Wednesday','Thursday','Friday'),
	start_time time not null,
	end_time time not null,
	primary key(letter,day),
	check(start_time < end_time)
	);

/*Room number as a primary key is sufficient to identify which room and room number cannot be null so,
it is incorporated in primary key*/
create table Room(
	room_number varchar(25), 
	location enum('Core-I','Core-II','Core-III','Core-IV','LH','Local') not null, 
	primary key(room_number)
	);

create table ScheduledIn (
	course_id varchar(10) not null,
	division enum('I','II','III','IV','NA') default 'NA',
	department_id varchar(10) not null, 
	letter enum ('A','B','C','D','E','F','G','H','I','J','K','L','A1','B1','C1','D1','E1'), 
	day enum ('Monday','Tuesday','Wednesday','Thursday','Friday'), 
	room_number varchar(25),
	primary key(division, letter, day, room_number),
	foreign key(course_id,division) references Course(course_id,division) on delete cascade on update cascade, 
	foreign key(department_id) references Department(department_id) on delete cascade on update cascade,
	foreign key(letter,day) references Slot(letter,day) on delete restrict on update cascade,
	foreign key(room_number) references Room(room_number) on delete restrict on update cascade
	);

/* 	1) 	course_id is an attr to get the information about the course from Course Table i.e. which courses' class is scheduled.
 		It is not made a primary key else we would have (CS101,A,1G1,Monday) & (BT101,A,1G1,Monday)
 		as a valid entry in schedledin table which is not possible as two courses cannot run
	  	in same room at the same time on same date. So, it has to be made a foreign key as well.
	
	2)	division needs to be a primary key thats why it is included as an attribute.
		division is made primary because of some Tutorials of 1st year courses. 
		Two divisions have class in same room, same time, same day.
		One of these entry would have been rejected if division is not included.
		Ex- PH102 is divided in 4 divisions and div I and III has class in L2 at same time, so 
		(PH102,I,PH,E,Tuesday,L2), (PH102,III,PH,E,Tuesday,L2) one of them is rejected without Division as primary key.
	
	3)	department_id is made an attribute(foreign key) to refer to which department this scheduled course belongs to.
		It is not made a primary key as each department offers several courses.

	4)	letter is made a primary key to know at what time of the day class is held. Each 1 hour slot is assigned a letter.
		So to distinguish between courses scheduled in same room at different time, we require letter as primary key.

	5)	day is a primary key because same course can be scheduled in same room at same time but different days.

	6) 	room_number is made a primary key because same course can run at same time in different rooms. Ex- first year courses.

	All above have to reference the primary keys in their respective tables, so these are made primary key. 

	Why others not made foreign key??
	1) Department name can be uniquely identified using dept id, so not reqd
	2) Start_time and end_time are derived attributes from letter, so again not required.
	3) Room location can be uniquely identified using Room number, so not reqd

	There is discrepency at 20 places in the database. that's why out of 1083 records, 1063 will be inserted. Errors are the following:

	1) EN671 - has 3 lectures but it has no department as it is an instutute level course. So, 3 errors (1 for each lecture).
	2) LS621 - has 3 lectures but no room is assigned to it, venue unknown. So again 3 errors (1 for each lecture).
	3) CH438 / CH643 are two different course_id on same time at same venue (C, Monday, 4005), this is not possible as only one course can be conducted.
	   So, out of two only 1 will be inserted and we will have 3 errors (1 for each lecture).
	4) CL204, CL612, CL642 have 1 lecture on Monday in 8-9 slot. So, only 1 of them inserted. So, two more errors.
	5) CL612, CL642 are different course_id on same time at same venue (Slot-A, Monday, Venue - 4203), this is not possible as only one course can be conducted.
	   So, out of two only 1 will be inserted and we will have 2 more errors as 1 error is already covered in above point.
	6) ME654 / ME554 have all the 3 lectures at same place on same time (F,Wednesday,1203),(H,Thursday,1203) , (F,Friday,1203). So three more errors.
	7) ME695 has no room specified so 3 more erros as it won't be inserted.
	8) CH002 has no time/slot spicified so 1 more error.

	In total #errors = (3+3+3+2+2+3+3+1) = 20 errors. 
	These records won't be inserted - EN671, LS621, CH643, CL612 monday lecture, CL642 monday, wednesday and friday lecture, ME554, ME695, CH002
*/