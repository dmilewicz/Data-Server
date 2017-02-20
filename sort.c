//
//  sort.c
//  595_project1
//
//  Created by David Milewicz on 2/19/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#include "sort.h"








void quicksort_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    if (p >= r) return;
    
    int q = partition_data(data, p, r, compare);
    quicksort_data(data, p, q-1, compare);
    quicksort_data(data, q+1, r, compare);
    
}




int partition_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    course_data* pivot = data[p];
    
    int i = p+1, j = r;
    
    while( i <= j) {
        
        while(compare(data[i], pivot) < 0) i++;
        while(compare(data[i], pivot) > 0) j--;
        
        if (i <= j) swap(data, i, j);
    }
    swap(data, p, j);
    
    return j;
    
}



int compare_enrollment(course_data* a, course_data* b) {
    return a->enrollment - b->enrollment;
}



void swap(void** arr, int a, int b) {
    void* temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}


