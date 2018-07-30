#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct course                                          // It stores course name and its credit
{
	char* cname;
	int credits;
	struct course* next;
};

struct exam                                            // It stores exam dates
{
	char* date;
	int stime;
	int etime;
	struct exam* next;
};

struct exam_name                                       // It stores exam name and a pointer to all the dates for the exam
{
	char* cname;
	struct exam* exams;
	struct exam_name* next;
};

struct stud_course                                     // It stores the course name
{
	char *cname;
	struct stud_course* next;
};

struct student                                         // It stores the roll no, list of courses a student has registered for
{
	char *rollno;
	struct stud_course* courses;
	char *name;
	struct student* next;
};

struct course* c_head = NULL;                          // Global pointers for all the three schema mentioned in que.
struct course* c_tail = NULL;

struct student* s_head = NULL;
struct student* s_tail = NULL;

struct exam_name* e_head = NULL;
struct exam_name* e_tail = NULL;


struct course* get_course()                            // It returns a course node with its name and its credit
{
	struct course* newnode = (struct course*)malloc(sizeof(struct course));
	
	newnode->cname = (char*)malloc(8*sizeof(char));
	newnode->credits = 0;

	newnode->next = NULL;
	if(c_head == NULL)
	{
		c_head = c_tail = newnode;
		return newnode;
	}
	c_tail->next = newnode;
	c_tail = newnode;

	return newnode;
}


struct exam* get_exam_date(char* date, int stime,int etime) // It returns a node of exam date
{
	struct exam* newnode = (struct exam*)malloc(sizeof(struct exam));
	newnode->date = (char*)malloc(13*sizeof(char));
	strcpy(newnode->date,date);
	newnode->stime = stime;
	newnode->etime = etime;

	newnode->next = NULL;

	return newnode;
}

void get_exam(char* date,int stime,int etime,char *course_name) // It creates a new exam node and has a pointer to just one exam date
{
	struct exam_name* newnode = (struct exam_name*)malloc(sizeof(struct exam_name));
	
	newnode->cname = (char*)malloc(8*sizeof(char));
	strcpy(newnode->cname,course_name);

	newnode->exams = get_exam_date(date,stime,etime);

	newnode->next = NULL;
	if(e_head == NULL)
	{
		e_head = e_tail = newnode;
		return;
	}
	e_tail->next = newnode;
	e_tail = newnode;
}



struct stud_course* get_stud_course(char *course_name)      // It returns a node with course name
{
	struct stud_course* newnode = (struct stud_course*)malloc(sizeof(struct stud_course));
	
	newnode->cname = (char*)malloc(8*sizeof(char));
	newnode->next = NULL;

	strcpy(newnode->cname,course_name);
	return newnode;
}

void get_stud(char* rollno,char* name,char* course_name)   // It creates a new student node, and has a pointer to one course he has registered for
{
	struct student* newnode = (struct student*)malloc(sizeof(struct student));

	newnode->rollno = (char*)malloc(12*sizeof(char));
	newnode->name = (char*)malloc(40*sizeof(char));
	newnode->courses = (struct stud_course*)malloc(sizeof(struct stud_course));
	newnode->courses = NULL;

	strcpy(newnode->rollno,rollno);
	strcpy(newnode->name,name);

	struct stud_course* temp = get_stud_course(course_name);
	temp->next = newnode->courses;
	newnode->courses = temp;

	newnode->next = NULL;
	if(s_head == NULL)
	{
		s_head = s_tail = newnode;
		return;
	}
	s_tail->next = newnode;
	s_tail = newnode;
}


void print_course_credits()                     
{
	struct course* temp = c_head;
	while(temp)
	{
		printf("%s %d\n",temp->cname,temp->credits);
		temp = temp->next;
	}
}

void read_course_file(FILE* fp)             // It reads the course-credit file and stores credit for all courses
{
	char *cname = (char*)malloc(8*sizeof(char));
	struct course* temp;
	int credits;

	while(fscanf(fp,"%[^,],%d\n",cname,&credits)!=EOF)
	{
		temp = get_course();
		strcpy(temp->cname,cname);
		temp->credits = credits;
	}
	// print_course_credits();
}

struct exam_name* search_by_course_name(char* name)        // It returns a pointer of a course in exam list 
{
	struct exam_name* temp = e_head;
	while(temp)
	{
		if(strcmp(temp->cname,name)==0)
		{
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void print_exam_table()
{
	struct exam_name* temp = e_head;
	struct exam* etemp;
	while(temp)
	{
		printf("%s     ",temp->cname);
		etemp = temp->exams;
		while(etemp)
		{
			printf("%s ",etemp->date);
			etemp = etemp->next;
		}
		temp = temp->next;
		printf("\n\n");
	}
}

void read_exam_file(FILE* fp)             // It reads the exam-date file and stores all the dates for each course exam
{
	struct exam_name* temp;
	struct exam* newtemp;
	char s1[14],s2[14],s3[5],s4[5]; int s,e,l;
	while(fscanf(fp,"%[^,],%[^,],%d%[^,],%d%s\n",s1,s2,&s,s3,&e,s4)!=EOF)
	{
		temp = search_by_course_name(s1);
		if(temp == NULL)
		{
			get_exam(s2,s,e,s1);
		}
		else
		{
			newtemp = get_exam_date(s2,s,e);
			newtemp->next = temp->exams;
			temp->exams = newtemp;
		}
	}
	// print_exam_table();
}

struct student* search_by_roll(char* rollno)            //It returns a pointer to a student in student list by its roll no.
{
	struct student* temp = s_head;
	while(temp)
	{
		if(strcmp(temp->rollno,rollno)==0)
		{
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void read_stud_file(FILE* fp,char *course_name)          // It creates a student and its courses list.
{
	int sno;
	char *rollno = (char*)malloc(12*sizeof(char));
	char *name = (char*)malloc(35*sizeof(char));
	char *str = (char*)malloc(30*sizeof(char));
	struct stud_course* stud_temp;
	struct student* temp;

	while(fscanf(fp,"%d,%[^,],%[^,]%s\n",&sno,rollno,name,str)!=EOF)
	{
		temp = search_by_roll(rollno);
		if(temp == NULL)
		{
			get_stud(rollno,name,course_name);
		}
		else
		{
			stud_temp = get_stud_course(course_name);
			stud_temp->next = temp->courses;
			temp->courses = stud_temp;
		}
	}
}

int return_credits(char* course_name)                 //It returns credit for a particular course
{
	struct course* temp = c_head;
	while(temp)
	{
		if(strcmp(temp->cname,course_name)==0)
		{
			return temp->credits;
		}
		temp = temp->next;
	}
	return 0;
}

void print_stud_file()
{
	struct student* temp = s_head;
	struct stud_course* stud_temp;
	while(temp)
	{
		printf("%s\n",temp->rollno);
		stud_temp = temp->courses;
		while(stud_temp)
		{
			printf("%s-%d, ",stud_temp->cname,return_credits(stud_temp->cname));
			stud_temp = stud_temp->next;
		}
		printf("\n------------------------------\n");
		temp = temp->next;
	}
}

int total_credits(struct stud_course* temp)              //Total credits is sum of credits of all courses student is undertaking
{
	int credits = 0;
	while(temp)
	{
		credits = credits + (return_credits(temp->cname));
		temp = temp->next;
	}
	return credits;
}

void part2()                                                  // Outputs part2 file
{
	FILE* fp = fopen("part2.csv","w");
	struct student* temp = s_head;
	while(temp)
	{
		if(total_credits(temp->courses)>40)
		{
			fprintf(fp,"%s,%s,%d\n",temp->rollno,temp->name,total_credits(temp->courses));
		}
		temp = temp->next;
	}
}

struct exam* return_exam_ptr(char* name)                        // returns pointer to the list of dates for a particular exam
{
	struct exam_name* temp = e_head;
	while(temp)
	{
		if(strcmp(temp->cname,name)==0)
		{
			return temp->exams;
		}
		temp = temp->next;
	}
	return NULL;
}

int on_same_date(char *exam1,char *exam2)                    // checks if 2 exams on same date or not
{
	if(strcmp(exam2,exam1)==0)
		return 0;

	struct exam* e1 = return_exam_ptr(exam1);
	struct exam* e2 = return_exam_ptr(exam2);

	if(e1==NULL || e2==NULL)
		return 0;

	for(;e1!=NULL;e1 = e1->next)
	{
		for(;e2!=NULL;e2 = e2->next)
		{
			if(strcmp(e1->date,e2->date)==0)
			{
				if(e1->stime == e2->stime)
				{
					return 1;
				}
				if(e1->stime < e2->stime)
				{
					if(e1->etime > e2->stime)
					{
						return 1; 
					}
				}
				if(e2->stime < e1->stime)
				{
					if(e2->etime > e1->stime)
						return 1;
				}
			}
		}
	}
	return 0;
}

void part1()                                                             //Outputs file1
{
	FILE* fp = fopen("part1.csv","w");
	struct student* temp = s_head;
	struct stud_course* stud_temp,*stemp;

	while(temp)
	{
		stud_temp = temp->courses;
		if(stud_temp->next ==NULL)
		{
			temp = temp->next;
			continue;
		}
		for(;stud_temp!=NULL;stud_temp = stud_temp->next)
		{
			for(stemp = stud_temp->next;stemp!=NULL;stemp = stemp->next)
			{
				if(on_same_date(stud_temp->cname,stemp->cname))
				{
					fprintf(fp,"%s, %s, %s, %s\n",temp->rollno,temp->name,stud_temp->cname,stemp->cname);
				}	
			}
		}
		temp = temp->next;
	}
}

void read_file()                                                           // It reads file by file by going through each directory
{
	FILE* fp; int i;

	char *course_name = (char*)malloc(8*sizeof(char));
	char *file_name = (char*)malloc(12*sizeof(char));

	char *directory = (char*)malloc(60*sizeof(char));
	char *file_path = (char*)malloc(60*sizeof(char));

	DIR* dir_l1 = opendir(".");
	struct dirent* ent_l1;

	DIR* dir_l2;
	struct dirent* ent_l2;

	DIR* dir_l3;
	struct dirent* ent_l3;

	while((ent_l1=readdir(dir_l1))!=NULL)
	{
		if(strcmp(ent_l1->d_name,"course-wise-students-list")==0)    // It opens course-wise-student-list directory
		{
			dir_l2 = opendir(ent_l1->d_name);
			while((ent_l2=readdir(dir_l2))!=NULL)                    //It returns each course as a directory. like "bt"
			{
				if(strcmp(ent_l2->d_name,".")==0 || strcmp(ent_l2->d_name,"..")==0)
					continue;
				
				strcpy(directory,"course-wise-students-list");
				strcat(directory,"/");
				strcat(directory,ent_l2->d_name);

				dir_l3 = opendir(directory);                           // It opens the directory "bt/" and so on.
				strcpy(file_path,directory);
				while((ent_l3 = readdir(dir_l3))!=NULL)                // reads files in "bt/""
				{
					strcpy(file_path,directory);
					if(strcmp(ent_l3->d_name,".")==0 || strcmp(ent_l3->d_name,"..")==0)
						continue;

					strcpy(file_name,ent_l3->d_name);
					
					for(i=0;file_name[i]!='.';i++)                      //extracts course name from that file name
						course_name[i]=file_name[i];
					course_name[i]='\0';

					strcat(file_path,"/");
					strcat(file_path,file_name);

					fp = fopen(file_path,"r");
					read_stud_file(fp,course_name);                     //opens that file and creates the student list
					fclose(fp);
				}
				closedir(dir_l3);
			}
			closedir(dir_l2);
		}
	}
	closedir(dir_l1);
	// print_stud_file();
}

void main()
{
	FILE* fp;
	fp = fopen("course-credits.csv","r");
	read_course_file(fp);
	fclose(fp);

	fp = fopen("exam-time-table.csv","r");
	read_exam_file(fp);
	fclose(fp);

	read_file();

	part2();
	part1();
}