#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void read_ett()
{
	FILE* fp = fopen("exam-time-table.csv","r");
	FILE* gp = fopen("150101062_ett.sql","w");

	char *cid = (char*)malloc(7*sizeof(char));
	char *date = (char*)malloc(12*sizeof(char));
	char *stime = (char*)malloc(7*sizeof(char));
	char *etime = (char*)malloc(7*sizeof(char));
	
	while(fscanf(fp,"%[^,],%[^,],%[^,],%[^'\n']\n",cid,date,stime,etime)!=EOF)
	{
		fprintf(gp,"insert into ett values ('%s','%s','%s','%s');\n",cid,date,stime,etime);
		fprintf(gp,"insert into ett_temp values ('%s','%s','%s','%s');\n",cid,date,stime,etime);
		fprintf(gp,"insert into ett_clone values ('%s','%s','%s','%s');\n",cid,date,stime,etime);
	}

	fclose(fp);
	fclose(gp);

	free(cid);
	free(date);
	free(stime);
	free(etime);
}

void read_cc()
{
	FILE* fp = fopen("course-credits.csv","r");
	FILE* gp = fopen("150101062_cc.sql","w");

	char *cid = (char*)malloc(7*sizeof(char));
	int credits;

	while(fscanf(fp,"%[^,],%d\n",cid,&credits)!=EOF)
	{
		fprintf(gp,"insert into cc values ('%s',%d);\n",cid,credits);
		fprintf(gp,"insert into cc_temp values ('%s',%d);\n",cid,credits);
		fprintf(gp,"insert into cc_clone values ('%s',%d);\n",cid,credits);	
	}

	fclose(fp);
	fclose(gp);

	free(cid);
}

void read_cwsl()
{
	int i,sno;

	DIR* d1 = opendir(".");
	struct dirent* e1;

	DIR* d2;
	struct dirent* e2;

	DIR* d3;
	struct dirent* e3;

	char* course_path = (char*)malloc(50*sizeof(char));
	char* file_path = (char*)malloc(60*sizeof(char));
	char* course_name = (char*)malloc(10*sizeof(char));
	char* name = (char*)malloc(40*sizeof(char));
	char* email = (char*)malloc(30*sizeof(char));
	char* roll = (char*)malloc(12*sizeof(char));


	FILE* fp;
	FILE* gp = fopen("150101062_cwsl.sql","w");

	while((e1 = readdir(d1))!=NULL)
	{
		if(strcmp(e1->d_name,"course-wise-students-list")==0)
		{
			d2 = opendir(e1->d_name);
			while((e2 = readdir(d2))!=NULL)
			{
				if(e2->d_name[0]=='.')
					continue;

				strcpy(course_path,"course-wise-students-list/");
				strcat(course_path,e2->d_name);

				d3 = opendir(course_path);

				while((e3 = readdir(d3))!=NULL)
				{
					if(e3->d_name[0]=='.')
						continue;
					strcpy(file_path,course_path);
					strcat(file_path,"/");
					strcat(file_path,e3->d_name);
					
					strcpy(course_name,e3->d_name);
					i=0;
					while(course_name[i]!='.') i++;
					course_name[i]='\0';

					fp = fopen(file_path,"r");
					while(fscanf(fp,"%d,%[^,],%[^,],%[^'\n']\n",&sno,roll,name,email)!=EOF)
					{
						fprintf(gp,"insert into cwsl values('%s',%d,'%s','%s','%s');\n",course_name,sno,roll,name,email);
						fprintf(gp,"insert into cwsl_temp values('%s',%d,'%s','%s','%s');\n",course_name,sno,roll,name,email);
						fprintf(gp,"insert into cwsl_clone values('%s',%d,'%s','%s','%s');\n",course_name,sno,roll,name,email);
						strcpy(email,"");
					}
					fclose(fp);
				}
				closedir(d3);
			}
			closedir(d2);
		}
	}
	closedir(d1);
}

void main()
{
	read_ett();
	read_cc();
	read_cwsl();
}