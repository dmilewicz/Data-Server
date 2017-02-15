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





typedef struct internetrequest {
    char* request_type;
    char* resource;
    char* HTTP_version;
    char* host;
    char* rest;
} parsed_request;

parsed_request* parse_request(char* request_str);
void print_request(parsed_request pr);

