/* 
 * Created by Obinna Asinugo on 2/14/17.
 * Copyright © 2017 Obinna Asinugo. All rights reserved.
 * 
 * parse_v2.c 
 * 
 * PURPOSE: 
 * This program was constructed to help parse course evaluation data 
 * (from the University of Pennsylvania) into html format.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readHTML.h"
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

int total_lines(char* file_name){ 
	// open file and initialize variables 
	FILE* courses_file = fopen(file_name, "rb");
	int num_lines = 0; 
	int character; 

	// get total number of lines in file 
	do 
	{
	    character = fgetc(courses_file);
	    if(character == '\n')
	        num_lines++;
	} while (character != EOF);

	// close file and return total number of lines 
	fclose(courses_file);
	return num_lines; 
}

char* parse(char* file_name, course_data* courses){
	// Initialize files 
	FILE* data_file = fopen(file_name,"wb"); 
	FILE* courses_file = fopen("course_evals.txt", "rb");
	
	// initialize string tracking info 
	char course_info[150];
	int index = 0;
	char* temp; 
	int length = 0; 
	int count = 0; 

	// write header to data.html file 
	char* header = "<html>\n<head>\n</head>\n<body>\n<table border='1'>\n<tr>\n<th>Course Number</th>\n<th>Instructor</th>\n<th>Enrollment</th>\n<th>Course Quality</th>\n<th>Course Difficulty</th>\n<th>Instructor Quality</th>\n"; 
	fputs(header, data_file);

	// initialize open and close row/data tags
	char* open_row_tag = "<tr>\n"; 
	char* closed_row_tag = "</tr>\n"; 
	char* open_data_tag = "<td>"; 
	char* closed_data_tag = "</td>\n"; 

	// Read in words from text and store into hashtable 
	while(fgets(course_info,150,courses_file)){
		// start new row of data 
		fputs(open_row_tag, data_file);

		// set the newline to null 
		length = strlen(course_info);
		course_info[length-1] = '\0';
		
		// initialize space for course info space 
		courses[index].course_info = malloc(sizeof(char)*length+1); 
		if(courses[index].course_info == NULL)
			return NULL; 
		courses[index].course_info[length] = '\0';

		// copy string contents from line to data structure 
		strcpy(courses[index].course_info, course_info); 

		// temp = course_info; 
		while(count != 6){
			if(count == 0){
				temp = strtok(course_info, ",");  
				strcpy(courses[index].course_id, temp); 
				fputs(open_data_tag, data_file);
				fputs(courses[index].course_id, data_file);
				fputs(closed_data_tag, data_file);

			}

			if(count == 1){
				temp = strtok(NULL, ","); 
				temp++; 
				strcpy(courses[index].prof, temp); 
				fputs(open_data_tag, data_file);
				fputs(courses[index].prof, data_file);
				fputs(closed_data_tag, data_file);
			}

			if(count == 2){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].enrollment = strtol(temp, NULL, 10); 	
				fputs(open_data_tag, data_file);
				fputs(temp, data_file);
				fputs(closed_data_tag, data_file);
			}

			if(count == 3){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].quality = strtod(temp, NULL); 	
				fputs(open_data_tag, data_file);
				fputs(temp, data_file);
				fputs(closed_data_tag, data_file);
			}

			if(count == 4){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index].difficulty = strtod(temp, NULL); 
				fputs(open_data_tag, data_file);
				fputs(temp, data_file);
				fputs(closed_data_tag, data_file);	
			}

			if(count == 5){
				temp = strtok(NULL, "\n"); 
				temp++; 
				courses[index].instructor_quality = strtod(temp, NULL); 
				fputs(open_data_tag, data_file);
				fputs(temp, data_file);
				fputs(closed_data_tag, data_file);
			}
			count++; 
		}
		memset(course_info, '\0',150);
		index++;
		count = 0; 

		// end row of data 
		fputs(closed_row_tag,data_file);
	}	

	// add footer to data.html file 
	char* footer = "</table>\n</body>\n</html>";
	fputs(footer, data_file);

	// close files 
	fclose(courses_file);
	fclose(data_file);
	return readHTML("data.html"); 
}
