/*
 * Header file for parsing data. 
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

char* parse	(char* str, course_data* data) ;
void print_courses	(course_data* courses, int size) ;
int total_lines(char* file_name) ; 

#endif  



