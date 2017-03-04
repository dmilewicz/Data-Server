//
//  sort.c
//  595_project1
//
//  Created by David Milewicz on 2/19/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

/*
 ********************************************
 *
 *  Quicksort functions
 *
 ********************************************
 */
#include "sort.h"
#include <string.h>

/*
 * Sort data using quicksort algorithm.
 */
void quicksort_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    if (p >= r) return;
    // partition data 
    int q = partition_data(data, p, r, compare);
    
    // sort the data based on partition  
    quicksort_data(data, p, q-1, compare);
    quicksort_data(data, q+1, r, compare);
    
}

/*
 * partition function for the Quicksort operations
 */

int partition_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) ) {
    course_data* pivot = data[p];
    
    int i = p+1, j = r;
    
    while( i <= j) {

        while(i <= j && compare(data[i], pivot) <= 0) {
            i++;
        }

        while(i <= j && compare(data[j], pivot) > 0) j--;
        
        if (i <= j) swap(data, i, j);
    }
    swap(data, p, j);
    return j; 
}

/*
 * Swap
 */
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

/*
 * Sort by course id. 
 */ 
int compare_course_id(course_data* a, course_data* b) {
    return strcmp(a->course_id, b->course_id);
}

/*
 * Sort by professors. 
 */ 
int compare_professors(course_data* a, course_data* b) {
    return strcmp(a->prof, b->prof);
}

/*
 * Sort by enrollment. 
 */ 
int compare_enrollment(course_data* a, course_data* b) {
    return a->enrollment - b->enrollment;
}

/*
 * Sort by course quality from low to high. 
 */ 
int compare_quality_low(course_data* a, course_data* b) {
    return ((100 * a->quality)) - ((100 * b->quality));
}

/*
 * Sort by course difficulty from high to low.  
 */ 
int compare_difficulty_high(course_data* a, course_data* b) {
    return  (b->difficulty * 100) - (a->difficulty * 100);
}

/*
 * Sort by instructor quality from  
 */ 
int compare_instructor_quality(course_data* a, course_data* b) {
    return (a->instructor_quality * 100) - (100 * b->instructor_quality);
}

/*
 * Sort by course quality from high to low. 
 */ 
int compare_quality_high(course_data* a, course_data* b) {
    return ((100 * b->quality)) - ((100 * a->quality));
}

/*
 * Sort by course difficulty low to high. 
 */
int compare_difficulty_low(course_data* a, course_data* b) {
    return (100 * b->difficulty) - (100 * a->difficulty);
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





