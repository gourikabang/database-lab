/*150101062-2*/
delimiter //	/*defines // as the delimiter*/

drop procedure if exists count_credits;//	/*to drop an existing procedure/table*/

create procedure count_credits()
begin
	
	declare roll_scan_done int default 0;	/*variable declaration*/
	declare cid_scan_done int default 0;
	declare credit_scan_done int default 0;
	declare roll varchar(20);
	declare rname varchar(100);
	declare cid varchar(10);
	declare ans int default 0;
	declare cred int default 0;
	declare cour_id varchar(10);

	declare cur_roll_scan cursor for (select distinct roll_no,name from cwsl);	/*cursor is declared to fetch (rollno,name) from cwsl*/
	declare continue handler for not found set roll_scan_done = 1;	/*declare continue handler for the above cursor for error handling when scanning is done*/
	open cur_roll_scan;

	main_loop:LOOP 			/*this loop fetches rollno,name from above cursor*/
	fetch from cur_roll_scan into roll,rname;

		if roll_scan_done then	/*if scanning is done, leave the loop and close the cursor*/
		close cur_roll_scan;
		leave main_loop;
		end if;
		set ans=0;	/*ans stores the total credits of a student with roll_no roll*/
		BLOCK2:begin /*this block is used to fetch a course enrolled by a student*/
			declare cur_cid_scan cursor for select course_id from cwsl where roll_no=roll; /*cursor is declared to get the courses registered by a rollno*/
			declare continue handler for not found set cid_scan_done = 1; /*declare continue handler for the above cursor for error handling when scanning is done*/
			open cur_cid_scan;

			inner_loop:LOOP
				fetch from cur_cid_scan into cid;

				if cid_scan_done then   /*if scanning is done, leave the loop and close the cursor and set cid_scan_done variable to 0 so that for next rollno courses can be fetched again*/
				set cid_scan_done=0;
				close cur_cid_scan;
				leave inner_loop;
				end if;

				BLOCK3:begin
					declare cur_credit_scan cursor for (select credits from cc where course_id = cid); /*cursor is declared to get the credits associated with a cid*/
					declare continue handler for not found set credit_scan_done=1;/*declare continue handler for the above cursor for error handling when scanning is done*/
					open cur_credit_scan;

					cred_loop:LOOP
					fetch from cur_credit_scan into cred;

						if credit_scan_done then /*if scanning is done, leave the loop and close the cursor and set credit_scan_done variable to 0 so that for next credit can be fetched again*/
						set credit_scan_done = 1;
						close cur_credit_scan;
						leave cred_loop;
						end if;
					end loop cred_loop;
				end BLOCK3;
				set ans = ans + cred; /*it updates ans by adding the reqd credits*/
			end loop inner_loop;
		end BLOCK2;
		if ans > 40 then/*if the total credit count of a student is > 40 then insert into the output table*/
			insert into Output2 values(roll,rname,ans);
		end if;
	end loop main_loop;
end //

drop table if exists Output2; //
create table Output2(rollno varchar(20),name varchar(100),credit int); // /*the table to store output*/

call count_credits(); // /*to call the procedure*/

select * from Output2; // /*to print the output*/

delimiter ;