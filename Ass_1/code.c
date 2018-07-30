#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
	char* roll;
	int no_classes_present;
	int no_classes_absent;
	struct student* next;
};

struct node
{
	char* roll;
	char* date;
	char* attend;
	struct node* next;
};

struct node* headnode = NULL;
struct node* tailnode = NULL;

struct student* headstud = NULL;
struct student* tailstud = NULL;

void getnode()
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->roll = (char*)malloc(15*sizeof(char));
	temp->date = (char*)malloc(15*sizeof(char));
	temp->attend = (char*)malloc(15*sizeof(char));
	temp->next = NULL;
	
	if(tailnode==NULL)
	{
		tailnode = temp;
		return;
	}
	tailnode->next = temp;
	tailnode = temp;
}

void getstud()
{
	struct student* temp = (struct student*)malloc(sizeof(struct student));
	temp->roll = (char*)malloc(15*sizeof(char));
	temp->no_classes_present = 0;
	temp->no_classes_absent = 0;

	if(tailstud==NULL)
	{
		tailstud = headstud = temp;
		return;
	}
	tailstud->next = temp;
	tailstud = temp;
}

int read_database()
{
	char ch;
	FILE* fp;

	char* s1 = (char*)malloc(30*sizeof(char));
	char* s2 = (char*)malloc(30*sizeof(char));
	char* s3 = (char*)malloc(30*sizeof(char));
	
	fp = fopen("database_12jan2017.csv","r");
	if(fp!=NULL)
	{
		while(1)
		{
			fscanf(fp,"%s %s %s",s1,s2,s3);

			getnode();
			if(headnode==NULL)
				headnode = tailnode;
			strcpy(tailnode->roll,s1);
			strcpy(tailnode->date,s2);
			strcpy(tailnode->attend,s3);
			if(feof(fp))
			{
				fclose(fp);
				break;
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void print_database()
{
	struct node* temp;
	temp = headnode;
	while(temp!=tailnode)
	{
		printf("%s %s %s\n",temp->roll,temp->date,temp->attend);
		temp=temp->next;
	}
}

void print_student_record()
{
	struct student* temp;
	temp = headstud;
	while(temp)
	{
		printf("%s %d %d\n",temp->roll, temp->no_classes_present,temp->no_classes_absent);
		temp= temp->next;
	}
}

void store_student_record()
{
	int a=0,p=0,c=0;
	char str[100];
	struct node* temp = headnode->next;
	getstud();
	strcpy(tailstud->roll,headnode->roll);

	if(strcmp(headnode->attend,"Absent")==0)
		a++;
	else
		p++;

	while(temp)
	{
		if(strcmp(temp->roll,tailstud->roll))
		{
			tailstud->no_classes_present = (int)p;
			tailstud->no_classes_absent = (int)a;
			a=0;
			p=0;
			getstud();
			strcpy(tailstud->roll,temp->roll);
		}
		if(strcmp(temp->attend,"Absent")==0)
		{
			a++;
			c=1;
		}
		else
		{
			p++;
			c=0;
		}
		temp = temp->next;
	}
	if(c)
		a--;
	else
		p--;
	tailstud->no_classes_present = (int)p;
	tailstud->no_classes_absent = (int)a;
}

void calculate_attendance()
{
	FILE* fpl = fopen("L75.csv","w");
	FILE* fpg = fopen("G75.csv","w");
	if(fpl==NULL || fpg==NULL)
	{
		printf("File could not be opened\n");
		return;
	}
	float attendance;
	struct student* temp = headstud;
	while(temp)
	{
		attendance = ((float)(temp->no_classes_present))/((float)(temp->no_classes_absent+temp->no_classes_present));
		
		if(attendance<0.75)
		{
			fprintf(fpl,"%s %d, %f\n",temp->roll,temp->no_classes_present,attendance*100);	
		}
		else
		{
			fprintf(fpg,"%s %d, %f\n",temp->roll,temp->no_classes_present,attendance*100);
		}
		temp = temp->next;
	}
	fclose(fpl);
	fclose(fpg);
}

int main()
{
	int i=0,total=0,ctr=0,no_students=0;
	float a=0,p=0,attendance;
	
	if(!read_database())
	{
		printf("File does not exist\n");
		return 0;
	}
	// print_database();

	store_student_record();
	//print_student_record();
	
	calculate_attendance();
}
