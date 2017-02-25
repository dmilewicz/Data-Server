//
//  sort.c
//  595_project1
//
//  Created by David Milewicz on 2/19/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#include "sort.h"




/*
 ********************************************
 *
 *  Quicksort functions
 *
 ********************************************
 */


void quicksort_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    if (p >= r) return;
    
    int q = partition_data(data, p, r, compare);
    quicksort_data(data, p, q-1, compare);
    quicksort_data(data, q+1, r, compare);
    
}

/*
 * partition function for the Quicksort operations
 */
int partition_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    course_data* pivot = data[p];
    
    int i = p+1, j = r;
    
    while( i < j) {
        
        // printing functions used for debugging
//        printf("%d\n", p);
//        printf("i: %d, j: %d\n", i,j);
//        printf("ii: %d, jj: %d\n", data[i]->enrollment, data[j]->enrollment);
        
        
        while(i <= j && compare(data[i], pivot) <= 0) {
            i++;
//            printf("i: %d\n", i);
        }
        while(i <= j && compare(data[j], pivot) > 0) j--;
        
        if (i <= j) swap(data, i, j);
    }
    swap(data, p, j);
    
    return j;
    
}


void swap(course_data** arr, int a, int b) {
    course_data* temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}


/*
 ********************************************
 *
 *  Comparison functions
 *
 ********************************************
 */

int compare_course_id(course_data* a, course_data* b) {
    return strcmp(a->course_id, b->course_id);
}

int compare_professors(course_data* a, course_data* b) {
    return strcmp(a->prof, b->prof);
}

int compare_enrollment(course_data* a, course_data* b) {
    return a->enrollment - b->enrollment;
}

int compare_quality(course_data* a, course_data* b) {
    return a->instructor_quality - b->instructor_quality;
}

int compare_dificulty(course_data* a, course_data* b) {
    return a->difficulty - b->difficulty;
}

int compare_instructor_quality(course_data* a, course_data* b) {
    return a->instructor_quality - b->instructor_quality;
}


/*
 ********************************************
 *
 *  Average functions
 *
 ********************************************
 */

int average_enrollment(data_container* d) {
    int sum = 0;
    
    for (int i = 0; i < d->length; i++) sum += d->data[i]->enrollment;
    
    return sum / d->length;
}

double average_difficulty(data_container* d) {
    double sum = 0;
    
    for ( int i = 0; i < d->length; i++) sum += d->data[i]->difficulty;
    
    return sum / d->length;
}

double average_quality(data_container* d) {
    double sum = 0;
    
    for ( int i = 0; i < d->length; i++) sum += d->data[i]->quality;
    
    return sum / d->length;
}

double average_instructor_quality(data_container* d) {
    double sum = 0;
    
    for ( int i = 0; i < d->length; i++) sum += d->data[i]->instructor_quality;
    
    return sum / d->length;
}









