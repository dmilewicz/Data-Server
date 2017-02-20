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


typedef struct data {
    course_data** data;
    size_t length;
} data_container;




int parse (char* str, course_data* data) ;
void data_to_HTML(data_container* data);
data_container* parse_data(char* filename);
void free_data_container(data_container* d);

