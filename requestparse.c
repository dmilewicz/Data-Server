
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
    while (rest[i] != '\n') i++;
    
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
            pr->filter_parameter = strtok(tokens[0],"&"); 
            // handle sort case 
            if(strcmp(pr->filter_parameter, "sortfield") == 0){
                pr->field_type = pr->filter_parameter; 
                pr->filter_parameter = NULL; 
            }
            else
                pr->field_type = strtok(NULL, "&"); 
            pr->field = tokens[1];
        }
        else{
            pr->filter_parameter = tokens[0];
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

        fc[i]->course_info = malloc(sizeof(char)*(strlen(courses[*indices]->course_info)+1)); 
        if(fc[i]->course_info == NULL) return NULL; 
       
        strcpy(fc[i]->course_info, courses[*indices]->course_info); 
        strcpy(fc[i]->course_id, courses[*indices]->course_id); 
        strcpy(fc[i]->prof, courses[*indices]->prof); 
        fc[i]->enrollment = courses[*indices]->enrollment; 
        fc[i]->quality = courses[*indices]->quality; 
        fc[i]->difficulty = courses[*indices]->difficulty;
        fc[i]->instructor_quality = courses[*indices]->instructor_quality;

        indices++; 
    }

    // create new data container for filtered courses 
    data_container* data_filtered = malloc(sizeof(data_container)); 
    data_filtered->length = course_indices->size; 
    data_filtered->data = fc; 

    return data_filtered; 
}

data_container* filter_enrollment(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    int request = atoi(pr->filter_parameter);
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
    check = strstr(pr->filter_parameter, "+");
    if(check!=NULL){
        char* s; 
        for(s = pr->filter_parameter;(s = strchr(s, 43)); s++){
            *s = ' ';
        }
    }

    // store course indices of matched strings 
    for(int i = 0; i < data->length; i++){
        check = strstr(courses[i]->prof, pr->filter_parameter); 
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
        char* check = strstr(courses[i]->course_id, pr->filter_parameter); 
        if(check != NULL){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses);
}

data_container* filter(data_container* data, post_request* pr){
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

data_container* sort(data_container* data, post_request* pr){
    course_data** courses = data->data; // courses
    // sort by course number 
    if(strcmp(pr->field, "coursenumber") == 0){
        // sort by course id
        quicksort_data(courses, 0, data->length - 1, compare_course_id);
        return data;
    }
    // sort by instructor 
    if(strcmp(pr->field, "instructorname") == 0){
        // sort by instructors
        quicksort_data(courses, 0, data->length - 1, compare_professors);
        return data; 
    }
    // sort by enrollment
    if(strcmp(pr->field, "enrollment") == 0){
        // sort by enrollment
        quicksort_data(courses, 0, data->length - 1, compare_enrollment);
        return data; 
    }
    // sort by course quality 
    if(strcmp(pr->field, "coursequalityhigh") == 0){
        // sort by course quality 
         quicksort_data(courses, 0, data->length - 1, compare_quality);
        return data;
    }
    // sort by course difficulty 
    if(strcmp(pr->field, "coursedifficultyhigh") == 0){
        // sort by course difficulty 
        quicksort_data(courses, 0, data->length - 1, compare_difficulty);
        return data; 
    }
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
    printf("Search string: %s\n", pr->filter_parameter);  
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

// void free(){

// }

  

  

