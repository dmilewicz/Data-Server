//
//  requestparse.h
//  595_project1
//
//  Created by David Milewicz on 2/14/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//


#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "parse.h"

typedef struct internetrequest {
    char* request_type;
    char* resource;
    char* HTTP_version;
    char* host;
    char* rest;
    char* postdata;
} parsed_request;

typedef struct PostResponse { 
	char* search;
	char* field_type; 
	char* field; 
} post_request; 

parsed_request* parse_request(char* request_str);
void print_request(parsed_request pr);
char* get_variables(char* rest);
int isPost(parsed_request* p);
char* get_post(char* pr); 
post_request* parse_post(post_request* pr, char* string);
void print_post_request(post_request* pr); 
data_container* post_process(data_container* data, post_request* pr); 
data_container* filter(data_container* data, post_request* pr); 
data_container* filter_course_number(post_request* pr, data_container* container); 
data_container* filter_instructor(post_request* pr, data_container* data);
data_container* filter_enrollment(post_request* pr, data_container* data); 


