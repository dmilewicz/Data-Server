#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

void print_courses(course_data* courses, int size){
	for(int i = 0; i < size; i++){
		printf("COURSE ID: %s\n",courses[i].course_id);
		printf("INSTRUCTOR: %s\n", courses[i].prof); 
		printf("ENROLLMENT: %d\n", courses[i].enrollment);
		printf("QUALITY: %f\n", courses[i].quality);
		printf("DIFFICULTY: %f\n", courses[i].difficulty);
		printf("QUALITY (I): %f\n", courses[i].instructor_quality);
		printf("\n");
	}
}

int main(){
char* file_name = "course_evals.txt"; 
FILE* courses_file = fopen(file_name, "rb");
int num_lines = 0; 
int character; 
char course_info[150];
char info[100];

// get total number of lines in file 
do 
{
    character = fgetc(courses_file);
    if(character == '\n')
        num_lines++;
} while (character != EOF);

// close and reopen file 
fclose(courses_file);
courses_file = fopen(file_name, "rb");

course_data courses[num_lines]; 
int index = 0;
char* temp; 
char* check; 
int length = 0; 
int count = 0; 
int count_data = 0;

// Read in words from text and store into hashtable 
while(fgets(course_info,150,courses_file)){
	// set the newline to null 
		length = strlen(course_info);
		course_info[length-1] = '\0';
		
		// initialize space for course info space 
		courses[index].course_info = malloc(sizeof(char)*length+1); 
		if(courses[index].course_info == NULL)
			return -1; 
		courses[index].course_info[length] = '\0';

		// copy string contents from line to data structure 
		strcpy(courses[index].course_info, course_info); 

		// temp = course_info; 
		while(count != 6){
			if(count == 0){
				temp = strtok(course_info, ",");  
				strcpy(courses[index].course_id, temp); 
			}

			if(count == 1){
				temp = strtok(NULL, ","); 
				temp++; 
				strcpy(courses[index].prof, temp); 
				// printf("%s\n", courses[index].prof);
			}

			if(count == 2){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].enrollment = strtol(temp, NULL, 10); 
				// printf("%d\n", courses[index].enrollment);	
			}

			if(count == 3){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].quality = strtod(temp, NULL); 
				// printf("%f\n", courses[index].quality);	
			}

			if(count == 4){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].difficulty = strtod(temp, NULL); 
				// printf("%f\n", courses[index].difficulty);	
			}

			if(count == 5){
				temp = strtok(NULL, "\n"); 
				temp++; 
				courses[index].instructor_quality = strtod(temp, NULL); 
				// printf("%f\n", courses[index].instructor_quality);	
			}
			count++; 
		}
		// printf("%s\n", courses[index].course_info);
		memset(course_info, '\0',150);
		index++;
		count = 0; 
}
	// printf("\n");
	print_courses(courses,num_lines);

// close file 
fclose(courses_file);
return 0; 
	
}
	