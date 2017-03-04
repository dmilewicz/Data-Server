//
//  readHTML.c
//  595_project1
//
//  Created by David Milewicz on 2/14/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#include "readHTML.h"



char* readHTML(char* resource) {
    FILE* fp= fopen(resource, "rb");
    if (fp == NULL) return NULL;
    
    // get the file length
    fseek(fp, 0, SEEK_END);
    long file_len = ftell(fp);
    rewind(fp);
    
    // read the file
    char* file_contents = malloc((file_len + 1) * sizeof(char));
    fread(file_contents, file_len, 1, fp);
    fclose(fp);
    
    return file_contents;

}
