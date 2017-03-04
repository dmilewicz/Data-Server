
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
#include <unistd.h>

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

int isPost(parsed_request* p) {
    return strcmp(p->request_type, "POST") == 0;
}

char* get_post(char* pr){
    char* search = "search";
    char* post = strstr(pr, search);
    printf("Post Gotten: %s\n\n", post);
    return post; 
}


post_request* parse_post(post_request* pr, char* string){
    char* post_string = get_post(string); 
    // printf("%s\n",post_string);
    char* delim = "&";
    char* tokens[3];
    int index = 0;
    
    
    // get the key-value pairs
    char* temp = strtok(post_string, delim);
    while(temp != NULL){
        
        tokens[index] = temp;
        index++;
        temp = strtok(NULL, delim);
    }
    
    // set all fields to null
    init_post_request(pr);
    
    char* val;
    for (int i = 0; i < index; i++) {
        val = strtok(tokens[i], "=");
        val = strtok(NULL, "=");
        
        if (strcmp(tokens[i], "sortfield") == 0) pr->sort_field = val;
        else if (strcmp(tokens[i], "searchfield") == 0) pr->filter_field = val;
        else if (strcmp(tokens[i], "search") == 0) pr->filter_parameters = val;
    }
    
    return pr;
}

void init_post_request(post_request* pr) {
    pr->filter_parameters = NULL;
    pr->filter_field = NULL;
    pr->sort_field = NULL;
}



data_container* array_to_data(void* list, course_data** courses){
    arraylist* course_indices = (arraylist*) list;
    course_data** fc = malloc(sizeof(course_data*)*course_indices->size); 
    if (fc == NULL) return NULL; 

    int* indices = course_indices->values; 

    for(int i = 0; i < course_indices->size; i++){
        fc[i] = courses[indices[i]];
    }

    // create new data container for filtered courses 
    data_container* data_filtered = malloc(sizeof(data_container)); 
    data_filtered->length = course_indices->size; 
    data_filtered->data = fc; 
    al_free(course_indices);
    return data_filtered; 
}


data_container* filter_enrollment(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    int request = atoi(pr->filter_parameters);
    for(int i = 0; i < data->length; i++){
        if(request <= courses[i]->enrollment){
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
    check = strstr(pr->filter_parameters, "+");
    if(check!=NULL){
        char* s; 
        for(s = pr->filter_parameters;(s = strchr(s, 43)); s++){
            *s = ' ';
        }
    }

    for (int j = 0; j < strlen(pr->filter_parameters); j++) {
        pr->filter_parameters[j] = toupper(pr->filter_parameters[j]);
    }
    
    printf("%s\n", pr->filter_parameters);
    
    // store course indices of matched strings 
    for(int i = 0; i < data->length; i++){
        check = strstr(courses[i]->prof, pr->filter_parameters);
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
        char* check = strstr(courses[i]->course_id, pr->filter_parameters);
        if(check != NULL){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses);
}

data_container* filter_course_quality(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    double request = strtod(pr->filter_parameters, NULL);
    for(int i = 0; i < data->length; i++){
        if(request <= courses[i]->quality){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses); 
}

data_container* filter_course_quality_low(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    double request = strtod(pr->filter_parameters, NULL);
    for(int i = 0; i < data->length; i++){
        if(request >= courses[i]->quality){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses); 
}

data_container* filter_course_difficulty(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    double request = strtod(pr->filter_parameters, NULL);
    for(int i = 0; i < data->length; i++){
        if(request <= courses[i]->difficulty){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses); 
}

data_container* filter_course_difficulty_low(post_request* pr, data_container* data){
    course_data** courses = data->data; // courses 
    arraylist* course_indices = al_initialize(2); // array list of course indices 

    // check to see if enrollment number matches 
    double request = strtod(pr->filter_parameters, NULL);
    for(int i = 0; i < data->length; i++){
        if(request >= courses[i]->difficulty){
            al_add(course_indices, i); 
        }
    }
    return array_to_data(course_indices, courses); 
}

data_container* choose_filter(data_container* data, post_request* pr){
    if (pr->filter_field == NULL || pr->filter_parameters == NULL) return data;
    char* field = pr->filter_field; // field for filtering
    
    // filter by course number
    if (strcmp(field, "coursenumber") == 0)
        return filter_course_number(pr, data);
    // filter by instructor 
    if (strcmp(field, "instructorname") == 0)
        return filter_instructor(pr, data); 
    // filter by enrollment
    if (strcmp(field, "enrollment") == 0)
        return filter_enrollment(pr, data);
    // filter by course quality above 
    if (strcmp(field, "coursequalityhigh") == 0) 
        return filter_course_quality(pr, data);
    // filter by course quality below 
    if (strcmp(field, "coursequalitylow") == 0)
        return filter_course_quality_low(pr, data);
    // filter by course difficulty above 
    if (strcmp(field, "coursedifficultyhigh") == 0) 
        return filter_course_difficulty(pr, data);
    // filter by course difficulty below
    if (strcmp(field, "coursedifficultylow") == 0) 
        return filter_course_difficulty_low(pr, data);
    return data;
}


data_container* copy_data(data_container* d) {
    data_container* new_container = malloc(sizeof(data_container));
    if (new_container == NULL) return NULL;
    
    new_container->data = malloc(sizeof(course_data*) * d->length);
    if (new_container->data == NULL) return NULL;
    
    for (int i = 0; i < d->length; i++) {
        new_container->data[i] = d->data[i];
    }
    new_container->length = d->length;
    
    return new_container;
}

//void free_shallow_data(data_container* d) {
//    // free course data
////    free(d->data);
//    // free container
//    free(d);
//}

void* choose_sort(post_request* pr){
    if (pr->sort_field == NULL) return NULL;
    
    // filter by course number
    if(strcmp(pr->sort_field, "coursenumber") == 0)
        return compare_course_id;
    // filter by instructor
    if(strcmp(pr->sort_field, "instructorname") == 0)
        return compare_professors;
    // filter by enrollment
    if(strcmp(pr->sort_field, "enrollment") == 0)
        return compare_enrollment;
    // sort by course quality high
    if(strcmp(pr->sort_field, "coursequalityhigh") == 0)
        return compare_quality_high;
    // sort by course quality low
    if(strcmp(pr->sort_field, "coursequalitylow") == 0)
        return compare_quality_low;
    // sort by course difficulty high
    if(strcmp(pr->sort_field, "coursedifficultyhigh") == 0)
        return compare_difficulty_high;
    // sort by course difficulty low
    if(strcmp(pr->sort_field, "coursedifficultylow") == 0)
        return compare_difficulty_low;
    return NULL;
}

void print_post_request(post_request* pr){
    printf("sort_field: %s\n", pr->sort_field);
    printf("filter_field: %s\n", pr->filter_field);
    printf("filter_parameters: %s\n\n", pr->filter_parameters);
}

void print_request(parsed_request pr) {
    printf("Request Type: %s\n", pr.request_type);
    printf("Requested resource: %s\n", pr.resource);
    printf("HTTP Version: %s\n", pr.HTTP_version);
    printf("Host: %s\n", pr.host);
    printf("Rest:\n%s\n\n", pr.rest);
}

