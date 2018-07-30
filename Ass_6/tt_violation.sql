/*150101062-1*/
delimiter //	 /*defines // as the delimiter*/

drop procedure if exists tt_violation; //	/*to drop an existing procedure/table*/

create procedure tt_violation()	
begin
	declare roll_scan_done int default 0;	/*variable declaration*/
	declare collide int default 0;	
	declare cid_scan1 int default 0;
	declare cid_scan2 int default 0;
	declare i int default 0;
	declare j int default 0;
	declare ett_scan1 int default 0;
	declare ett_scan2 int default 0;
	declare e1_date date;
	declare e2_date date;
	declare e1_stime time;
	declare e2_stime time;
	declare e1_etime time;
	declare e2_etime time;
	declare roll varchar(20);
	declare cid1 varchar(10);
	declare cid2 varchar(10);
	declare rname varchar(100);

	declare cur_roll_scan cursor for select distinct roll_no,name from cwsl;	/*cursor is declared to fetch (rollno,name) from cwsl*/
	declare continue handler for not found set roll_scan_done = 1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
	open cur_roll_scan;

	main_loop:LOOP
	fetch from cur_roll_scan into roll,rname;	/*This loop fetches rollno,name*/

		if roll_scan_done then           /*if scanning is done, leave the loop and close the cursor*/
		close cur_roll_scan;
		leave main_loop;
		end if;

		set i=0;
		BLOCK2: begin   /*this block fetches a course enrolled by a student*/    
			declare cur_cid1 cursor for select course_id from cwsl where roll_no = roll; /*this cursor fetches course1 for a student*/
			declare continue handler for not found set cid_scan1=1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
			open cur_cid1;
			
			cid_loop:LOOP              /*fetch course1 for a student*/
			fetch from cur_cid1 into cid1;

				if cid_scan1 then           /*if scanning is done, leave the loop and close the cursor and set cid_scan variable to 0 so that for next rollno courses can be fetched again*/
				set cid_scan1 = 0;
				close cur_cid1;
				leave cid_loop;
				end if;

				set i=i+1;               
				set j=0;
				BLOCK3: begin /*this block fetches another course enrolled by a student*/
					declare cur_cid2 cursor for select course_id from cwsl where roll_no = roll;	/*this cursor fetches course2 for a student*/
					declare continue handler for not found set cid_scan2=1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
					open cur_cid2;

					while j<i do 				/*it skips those records which have already been checked; (course1,course2) pair is checked only once*/
						set j= j+1;
						fetch from cur_cid2 into cid2;
					end while; 
					
					inner_cid_loop:LOOP    /*fetch course2 for a student*/
					fetch from cur_cid2 into cid2;

						if cid_scan2 then /*if scanning is done, leave the loop and close the cursor and set cid_scan variable to 0 so that for next rollno courses can be fetched again*/
						set cid_scan2 = 0;
						close cur_cid2;
						leave inner_cid_loop;
						end if;
						
						BLOCK4: begin /*to fetch exam time tables*/
							declare cur_et1 cursor for select exam_date,start_at,end_at from ett where course_id = cid1;	/*it is declared to fetch exam_tt for course1*/
							declare continue handler for not found set ett_scan1=1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
							open cur_et1;

							ett_main_loop:LOOP 
							fetch from cur_et1 into e1_date,e1_stime,e1_etime;

								if ett_scan1 then	/*if scanning is done, leave the loop and close the cursor and set ett_scan variable to 0 so that for next exam_tt can be fetched again*/
								set ett_scan1=0;
								close cur_et1;
								leave ett_main_loop;
								end if;

								BLOCK5:begin /*this block is used to detect tt clash for a student*/
									declare cur_et2 cursor for select exam_date,start_at,end_at from ett where course_id = cid2;	/*it fetches exam_tt for course2*/
									declare continue handler for not found set ett_scan2=1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
									open cur_et2;

									ett_inner_loop:LOOP
									fetch from cur_et2 into e2_date,e2_stime,e2_etime;
									
										if ett_scan2 then	/*if scanning is done, leave the loop and close the cursor and set ett_scan variable to 0 so that for next ett can be fetched again*/
										set ett_scan2=0;
										close cur_et2;
										leave ett_inner_loop;
										end if;

										set collide=0;	/*collide is set implies that there is a clash between course1 and course2*/
										if e1_date=e2_date then	/*for a clash the date must be same and the time should overlap*/
											if (e1_stime <= e2_stime and e1_etime >= e2_stime) then
												set collide = 1;
											elseif (e2_stime <= e1_stime and e2_etime >= e1_stime) then
												set collide = 1;
											end if;
										end if;
										
										if collide then	/*if collision has occured between course1 and course2 then insert into the output table*/
											set ett_scan1=0;
											set ett_scan2=0;
											close cur_et1;
											close cur_et2;
											insert into Output1 values(roll,rname,cid1,cid2);
											leave ett_main_loop;	/*since a clash is already detected so no need to check the other time tables of both course1 and course2 so leave loop4 and close both resp cursors*/
										end if;

									end loop ett_inner_loop;
								end BLOCK5;
							end loop ett_main_loop;
						end BLOCK4;
					end loop inner_cid_loop;
				end BLOCK3;
			end loop cid_loop;	
		end BLOCK2;
	end LOOP main_loop;
end //

drop table if exists Output1; //	
create table Output1(rollno varchar(20),rname varchar(100),cid1 varchar(10),cid2 varchar(10)); //	/*create the output table*/

call tt_violation();//	/*to invoke the procedure*/

select * from Output1; //	/*print output*/
delimiter ;	/*change delimiter*/
