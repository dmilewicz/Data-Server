//
//  requestparse.c
//  595_project1
//
//  Created by David Milewicz on 2/14/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#include "requestparse.h"



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




void print_request(parsed_request pr) {
    printf("Request Type: %s\n", pr.request_type);
    printf("Requested resource: %s\n", pr.resource);
    printf("HTTP Version: %s\n", pr.HTTP_version);
    printf("Host: %s\n", pr.host);
    printf("Rest:\n%s\n\n", pr.rest);
}
  
