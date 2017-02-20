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


data_container* parse_data(char* filename){

    // Initialize files
    FILE* courses_file = fopen(filename, "rb");
    
    // initialize number of lines/ character info
    int num_lines = 0;
    int character;
    char course_info[150];
    
    // get total number of lines in file
    do
    {
        character = fgetc(courses_file);
        if(character == '\n')
            num_lines++;
    } while (character != EOF);
    
    // set file pointer back to the beginning
    rewind(courses_file);
    
    // allocate memory for the data struct
    course_data** courses = malloc(sizeof(course_data*) * num_lines);
    if (courses == NULL) return NULL;
    
    // initialize helper variables
    int index = 0;
    char* temp;
    int length = 0;
    int count = 0;
    

    // Read in words from text and store into hashtable 
    while(fgets(course_info,150,courses_file)){
        
        courses[index] = malloc(sizeof(course_data));
        if (courses[index] == NULL) return NULL;

		// set the newline to null 
		length = strlen(course_info);
		course_info[length-1] = '\0';
		
		// initialize space for course info space 

		courses[index]->course_info = malloc(sizeof(char)*length+1);
		if(courses[index]->course_info == NULL)
			return NULL;
//		courses[index].course_info[length] = '\0';

		// copy string contents from line to data structure 
		strcpy(courses[index]->course_info, course_info); 

		// temp = course_info; 
		while(count != 6){
			if(count == 0){
				temp = strtok(course_info, ",");  

				strcpy(courses[index]->course_id, temp);
				// temp = strcat(open_data_tag, courses[index].course_id); 
				// temp = strcat(temp, closed_data_tag); 
				// fputs(temp, data_file);
			}

			if(count == 1){
				temp = strtok(NULL, ","); 
				temp++; 

				strcpy(courses[index]->prof, temp); 
				// printf("%s\n", courses[index].prof);
			}

			if(count == 2){
				temp = strtok(NULL, ","); 
				temp++; 

				courses[index]->enrollment = atoi(temp);
				// printf("%d\n", courses[index].enrollment);	
			}

			if(count == 3){
				temp = strtok(NULL, ","); 
				temp++;
				courses[index]->quality = strtod(temp, NULL); 
				// printf("%f\n", courses[index].quality);	
			}

			if(count == 4){
				temp = strtok(NULL, ","); 
				temp++; 

				courses[index]->difficulty = strtod(temp, NULL); 
				// printf("%f\n", courses[index].difficulty);
			}

			if(count == 5){
				temp = strtok(NULL, "\n"); 
				temp++; 

				courses[index]->instructor_quality = strtod(temp, NULL); 
				// printf("%f\n", courses[index].instructor_quality);	
			}
			count++; 
		}
		// printf("%s\n", courses[index].course_info);
        
        // reset & update helper variables
		memset(course_info, '\0',150);
		index++;
		count = 0;
    }
    // printf("\n");
//    print_courses(courses,num_lines);
    
    // create container for the data;
    data_container* course_data = malloc(sizeof(data_container));
    
    course_data->data = courses;
    course_data->length = index;
    
    
    // close files
    fclose(courses_file);
    return course_data;
    
}




void data_to_HTML(data_container* data) {
    
    // data file to write to. Could configure it straight into a string but the size will have to be variable (this is simpler).
    FILE* data_file = fopen("data.html","w");
    
    
    
    // write header to data.html file
    char* header = "<table border='1'>\n<tr>\n<th>Course Number</th>\n<th>Instructor</th>\n<th>Enrollment</th>\n<th>Course Quality</th>\n<th>Course Difficulty</th>\n<th>Instructor Quality</th>\n";
    
    fputs(header, data_file);

    // initialize open and close row/data tags
    char* open_row_tag = "<tr>\n";
    char* closed_row_tag = "</tr>\n";
        
        
    // define data tag
    char* open_data_tag = "<td> ";
    char* closed_data_tag = "</td>\n";
        
    char buf[15];
    
    for ( int i = 0; i < data->length; i++) {
        // start new row of data
        fputs(open_row_tag, data_file);
        
        fputs(open_data_tag, data_file);
        fputs(data->data[i]->course_id, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        fputs(data->data[i]->prof, data_file);
        fputs(closed_data_tag, data_file);

        fputs(open_data_tag, data_file);
        sprintf(buf, "%d", data->data[i]->enrollment);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%f", data->data[i]->quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%f", data->data[i]->difficulty);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%f", data->data[i]->instructor_quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        // end row of data
        fputs(closed_row_tag,data_file);
        
    }
    
    // add footer to data.html file
    char* footer = "</table>";
    fputs(footer, data_file);
    
    // close the file
    fclose(data_file);
    
}



void free_data_container(data_container* d) {
    // free data
    for ( int i = 0; i < d->length; i++) {
        free(d->data[i]->course_info);
        free(d->data[i]);
    }
    
    free(d->data);
    
    // free container
    free(d);
    
    // success
    return;
}


