/*
 * Header file for parsing data. 
 */



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

int parse	(char* str, course_data* data) ;

