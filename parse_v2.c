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
#include "sort.h"

/*
 * Prints out course data. 
 */
void print_courses(course_data** courses, int size){
	for(int i = 0; i < size; i++){
		printf("COURSE ID: %s\n",courses[i]->course_id); 
		printf("INSTRUCTOR: %s\n", courses[i]->prof); 
		printf("ENROLLMENT: %d\n", courses[i]->enrollment);
		printf("QUALITY: %f\n", courses[i]->quality);
		printf("DIFFICULTY: %f\n", courses[i]->difficulty);
		printf("QUALITY (I): %f\n", courses[i]->instructor_quality);
		printf("\n");
	}
}

/*
 * Returns the total numebr of lines in the file, where each line represents a
 * course and its respective data. 
 */
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

/*
 * Prints courses. 
 */
void print_data(data_container* data) {
    print_courses(data->data, data->length);
}

/*
 * Parses the data from the file 
 */
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

		// copy string contents from line to data structure 
		strcpy(courses[index]->course_info, course_info); 

		// temp = course_info; 
		while(count != 6){
			if(count == 0){
				temp = strtok(course_info, ",");  
				strcpy(courses[index]->course_id, temp);
			}

			if(count == 1){
				temp = strtok(NULL, ","); 
				temp++; 
				strcpy(courses[index]->prof, temp); 
			}

			if(count == 2){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index]->enrollment = atoi(temp);	
			}

			if(count == 3){
				temp = strtok(NULL, ","); 
				temp++;
				courses[index]->quality = strtod(temp, NULL); 	
			}

			if(count == 4){
				temp = strtok(NULL, ","); 
				temp++; 
				courses[index]->difficulty = strtod(temp, NULL); 
			}

			if(count == 5){
				temp = strtok(NULL, "\n"); 
				temp++; 
				courses[index]->instructor_quality = strtod(temp, NULL); 	
			}
			count++; 
		}
		
        // reset & update helper variables
		memset(course_info, '\0',150);
		index++;
		count = 0;
    }
    
    // create container for the data;
    data_container* course_data = malloc(sizeof(data_container));
    course_data->data = courses;
    course_data->length = index;
    
    // close files
    fclose(courses_file);
    return course_data;
    
}

/*
 * Compute and write averages of courses info to an html file.
 */ 
void write_averages(data_container* my_data, FILE* data_file) {
    // store header 
    char* header = "<table border='1'>\n<tr>\n<th>Total Courses</th>\n<th>Avg. Enrollment</th>\n<th>Avg. Course Quality</th>\n<th>Avg. Course Difficulty</th>\n<th>Avg. Instructor Quality</th>\n";
    // write header to file 
    fputs(header, data_file);

    // initialize open and close row/data tags
    char* open_row_tag = "<tr>\n";
    char* closed_row_tag = "</tr>\n";
            
    // define data tag
    char* open_data_tag = "<td> ";
    char* closed_data_tag = "</td>\n";

    // account for having no course data 
    if(my_data->length > 0){
      int avg_enrollment = average_enrollment(my_data);
      double avg_quality = average_quality(my_data);
      double avg_difficulty = average_difficulty(my_data);
      double avg_prof_quality = average_instructor_quality(my_data); 

      char buf[15];

        // start new row of data
        fputs(open_row_tag, data_file);

        fputs(open_data_tag, data_file);
        sprintf(buf, "%zu", my_data->length);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);

        fputs(open_data_tag, data_file);
        sprintf(buf, "%d", avg_enrollment);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%.2f", avg_quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%.2f", avg_difficulty);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%.2f", avg_prof_quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file); 

        // end row of data
        fputs(closed_row_tag,data_file);
    }
     // add footer to data.html file
    char* footer = "</table>";
    fputs(footer, data_file);
}

/*
 * Write data to html file.
 */
void data_to_HTML(data_container* data , char* data_target) {
    // initialize FILE pointer 
    FILE* data_file = fopen(data_target,"w");

    // write averages to respective file
    write_averages(data, data_file);
     
    // write header to data.html file
    char* header = "<table border='1'>\n<tr>\n<th>Course Number</th>\n<th>Instructor</th>\n<th>Enrollment</th>\n<th>Course Quality</th>\n<th>Course Difficulty</th>\n<th>Instructor Quality</th>\n";
    fputs(header, data_file);

    // initialize open and close row/data tags
    char* open_row_tag = "<tr>\n";
    char* closed_row_tag = "</tr>\n";
    char* open_data_tag = "<td> ";
    char* closed_data_tag = "</td>\n";
        
    char buf[15];
    
    // write data to file 
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
        sprintf(buf, "%.2f", data->data[i]->quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%.2f", data->data[i]->difficulty);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        fputs(open_data_tag, data_file);
        sprintf(buf, "%.2f", data->data[i]->instructor_quality);
        fputs(buf, data_file);
        fputs(closed_data_tag, data_file);
        
        // end row of data
        fputs(closed_row_tag,data_file);
        
    }
    // add footer to data.html file
    char* footer = "</table>\n";
    fputs(footer, data_file);
    
    // close the file
    fclose(data_file);   
}

/*
 * Frees data from data container. 
 */
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

/*
 * Frees shallow data.
 */ 
void free_data_shallow(data_container* d) {
    free(d->data);
    free(d);
}

