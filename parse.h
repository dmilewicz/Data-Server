/* 
 * Created by Obinna Asinugo on 2/14/17.
 * Copyright © 2017 Obinna Asinugo. All rights reserved.
 * 
 * parse_v2.h
 * 
 * Purpose: 
 * Header file for parse_v2.c. 
 * 
 */
#ifndef parse_h
#define parse_h

// declare functions/ structure(s)
typedef struct CourseData{
	char* course_info; 
	char course_id[50];
	char prof[50]; 
	int enrollment; 
	double quality; 
	double difficulty; 
	double instructor_quality; 
} course_data;



typedef struct data {
    course_data** data;
    size_t length;
} data_container;

void free_data_shallow(data_container* d);
void print_courses(course_data** courses, int size);
int parse (char* str, course_data* data);
void data_to_HTML(data_container* data , char* data_target);
data_container* parse_data(char* filename);
void free_data_container(data_container* d);
void print_data(data_container* data);

#endif
