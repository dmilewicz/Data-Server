
//
//  requestparse.c
//  595_project1
//
//  Created by David Milewicz on 2/14/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#include "requestparse.h"
#include "parse.h"
#include "arraylist.h"



parsed_request* parse_request(char* request_str) {
    parsed_request* incoming_request = malloc(sizeof(parsed_request));
    
//    int slen = strlen(request_str);
    int idx = 0;
    incoming_request->request_type = request_str;
    
    while (request_str[idx] != ' ') idx++;
    request_str[idx] = '\0';
    incoming_request->resource = request_str + idx + 1;
    
    while (request_str[idx] != ' ') idx++;
    request_str[idx] = '\0';
    incoming_request->HTTP_version = request_str + idx + 1;
    
    while (request_str[idx] != '\n') idx++;
    request_str[idx] = '\0';
    incoming_request->host = request_str + (idx += 7);
    
    while (request_str[idx] != '\n') idx++;
    request_str[idx] = '\0';
    incoming_request->rest = request_str + idx + 1;
    
    incoming_request->postdata = NULL;
    
    return incoming_request;
    
        
}



char* get_variables(char* rest) {
    
    int i = 0;
    while (rest[i] != '\n' || rest[i+1] != '\n') i++;
    
    rest[i] = '\0';
    rest[i+1] = '\0';
    
    return rest + i + 2;
    
}


int isPost(parsed_request* p) {
    return strcmp(p->request_type, "POST") == 0;
}

char* get_post(char* pr){
    char* search = "search";
    char* post = strstr(pr, search);
    printf("%s", post);
    return post; 
}


post_request* parse_post(post_request* pr, char* string){
    char* post_string = get_post(string); 
    // printf("%s\n",post_string);
    char* delim = "=";
    char* tokens[10]; 
    int index = 0; 
    char* temp = strtok(post_string, delim);

    while(temp != NULL){
        temp = strtok(NULL, delim);
        tokens[index] = temp; 
        printf("%s\n", tokens[index]); 
        index++; 
    }

    // store search filter string and field type 
    if(tokens[0]!=NULL){
        char* check = strstr(tokens[0], "&"); 
        // printf("%s\n", check);
        if(check != NULL){
            pr->search = strtok(tokens[0],"&"); 
            pr->field_type = strtok(NULL, "&"); 
            pr->field = tokens[1];
        }
        else{
            pr->search = tokens[0];
        }
    }
    return pr;
}

data_container* filter_course_number(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // store course indices of matched strings 
    for(int i = 0; i < data->length; i++){
        char* check = strstr(courses[i]->course_id, pr->search); 
        if(check != NULL){
            al_add(course_indices, i); 
        }
    }

    // create array of courses based on filter 
    // course_data* filtered_courses[course_indices->size];
    // course_data** fc = filtered_courses;

    course_data** fc = malloc(sizeof(course_data*)*course_indices->size); 
    if (fc == NULL) return NULL; 

    int* indices = course_indices->values; 

    for(int i = 0; i < course_indices->size; i++){
        fc[i] = malloc(sizeof(course_data)); 
        if (fc[i] == NULL) return NULL; 

        if (copy_data(courses[*indices], fc[i]) != 0) return NULL;

        indices++; 
    }

    // create new data container for filtered courses 
    data_container* data_filtered = malloc(sizeof(data_container)); 
    data_filtered->length = course_indices->size; 
    data_filtered->data = fc; 

    print_courses(fc, course_indices->size);

    return data_filtered; 
}








int copy_data(course_data* src, course_data* dest) {
    
    dest->course_info = malloc(sizeof(char)* (strlen(src->course_info)));
    if (dest->course_info == NULL) return -1;
    
    strcpy(dest->course_info, src->course_info);
    strcpy(dest->course_id, src->course_id);
    strcpy(dest->prof, src->prof);
    dest->enrollment = src->enrollment;
    dest->quality = src->quality;
    dest->difficulty = src->difficulty;
    dest->instructor_quality = src->instructor_quality;
    
    return 0;
}



data_container* filter(data_container* data, post_request* pr){
    char* field = pr->field; // field for filtering 

    if(strcmp(field, "coursenumber") == 0)
        return  filter_course_number(pr, data); 
    // consider different fields 
    return NULL; 
}




data_container* post_process(data_container* data, post_request* pr){
    // Edge case: check if field type is not NULL
    if(pr->field_type == NULL){
        return NULL;
    }

    // filter data 
    if(strcmp(pr->field_type, "searchfield")==0) 
        return filter(data, pr);
    // add way to access sort based on field type  
    return NULL; 
}

void print_post_request(post_request* pr){
    printf("Search string: %s\n", pr->search);  
    printf("Field type: %s\n", pr->field_type);
    printf("Field: %s\n", pr->field); 
}

void print_request(parsed_request pr) {
    printf("Request Type: %s\n", pr.request_type);
    printf("Requested resource: %s\n", pr.resource);
    printf("HTTP Version: %s\n", pr.HTTP_version);
    printf("Host: %s\n", pr.host);
    printf("Rest:\n%s\n\n", pr.rest);
}

 void free(){

 }




  

  

