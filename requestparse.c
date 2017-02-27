
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
#include "sort.h"

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
    char* delim = "&";
    char* tokens[3];
    int index = 0; 
    char* temp = strtok(post_string, delim);

    while(temp != NULL){
        temp = strtok(NULL, delim);
        tokens[index] = temp;
        printf("%s\n", tokens[index]); 
        index++; 
    }
    
    
    char* val;
    for (int i = 0; i < index; i++) {
        val = strtok(tokens[i], "=");
        
        if (strcmp(tokens[i], "sortfield") == 0) pr->sort_field = val;
        else if (strcmp(tokens[i], "searchfield") == 0) pr->filter_field = val;
        else if (strcmp(tokens[i], "search") == 0) pr->filter_parameters = val;
        
        
    }
    

    // store search filter string and field type 
    if(tokens[0]!=NULL){
        char* check = strstr(tokens[0], "&"); 
        // printf("%s\n", check);
        if(check != NULL){
            pr->search = strtok(tokens[0],"&"); 
            // handle sort case 
            if(strcmp(pr->search, "sortfield") == 0){
                pr->field_type = pr->search; 
                pr->search = NULL; 
            }
            else
                pr->field_type = strtok(NULL, "&"); 
            pr->field = tokens[1];
        }
        else{
            pr->search = tokens[0];
        }
    }
    return pr;
}

data_container* array_to_data(void* list, course_data** courses){
    arraylist* course_indices = (arraylist*) list; 
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


data_container* filter_enrollment(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    int request = atoi(pr->search);
    for(int i = 0; i < data->length; i++){
        if(request == courses[i]->enrollment){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses); 
}

data_container* filter_instructor(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if string contains any spaces (+ in html) 
    char* check; 
    check = strstr(pr->search, "+");
    if(check!=NULL){
        char* s; 
        for(s = pr->search;(s = strchr(s, 43)); s++){
            *s = ' ';
        }
    }

    // store course indices of matched strings 
    for(int i = 0; i < data->length; i++){
        check = strstr(courses[i]->prof, pr->search); 
        if(check != NULL){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses);  
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
    return array_to_data(course_indices, courses);
}

data_container* choose_filter(data_container* data, post_request* pr){
    char* field = pr->field; // field for filtering 

    // filter by course number 
    if(strcmp(field, "coursenumber") == 0)
        return filter_course_number(pr, data);
    // filter by instructor 
    if(strcmp(field, "instructorname") == 0)
        return filter_instructor(pr, data); 
    // filter by enrollment
    if(strcmp(field, "enrollment") == 0)
        return filter_enrollment(pr, data); 
    return NULL; 
}

data_container* sort_course_number(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses
    // sort by course id
    quicksort_data(courses, 0, data->length - 1, compare_course_id);
    return data;
}

data_container* sort_instructor(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses
    // sort by course id
    quicksort_data(courses, 0, data->length - 1, compare_professors);
    return data;
}

data_container* sort_enrollment(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses
    // sort by course id
    quicksort_data(courses, 0, data->length - 1, compare_enrollment);
    return data;
}

data_container* sort_quality(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses
    // sort by course id
    quicksort_data(courses, 0, data->length - 1, compare_quality);
    return data;
}

data_container* sort_difficulty(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses
    // sort by course id
    quicksort_data(courses, 0, data->length - 1, compare_difficulty);
    return data;
}

data_container* sort(data_container* data, post_request* pr){
    // filter by course number 
    if(strcmp(pr->field, "coursenumber") == 0)
        return sort_course_number(pr, data);
    // filter by instructor 
    if(strcmp(pr->field, "instructorname") == 0)
        return sort_instructor(pr, data); 
    // filter by enrollment
    if(strcmp(pr->field, "enrollment") == 0)
        return sort_enrollment(pr, data); 
    // sort by course quality 
    if(strcmp(pr->field, "coursequalityhigh") == 0)
        return sort_quality(pr, data); 
    // sort by course difficulty 
    if(strcmp(pr->field, "coursedifficultyhigh") == 0)
        return sort_difficulty(pr, data); 
    return NULL; 
}




int process_sort(post_request* pr){
    // filter by course number
    if(strcmp(pr->field, "coursenumber") == 0)
        return 0;
    // filter by instructor
    if(strcmp(pr->field, "instructorname") == 0)
        return 1;
    // filter by enrollment
    if(strcmp(pr->field, "enrollment") == 0)
        return 2;
    // sort by course quality
    if(strcmp(pr->field, "coursequalityhigh") == 0)
        return sort_quality(pr, data);
    // sort by course difficulty
    if(strcmp(pr->field, "coursedifficultyhigh") == 0)
        return sort_difficulty(pr, data);
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
    // sort data
    if(strcmp(pr->field_type, "sortfield")==0)
        return sort(data,pr);  
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




  

  

