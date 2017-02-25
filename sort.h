//
//  sort.h
//  595_project1
//
//  Created by David Milewicz on 2/19/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#ifndef sort_h
#define sort_h

#include <stdio.h>
#include <strings.h>

#endif /* sort_h */

#include "parse.h"




void swap(course_data** arr, int a, int b);
int partition_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) );

void quicksort_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) );


int compare_course_id(course_data* a, course_data* b);
int compare_professors(course_data* a, course_data* b);
int compare_enrollment(course_data* a, course_data* b);
int compare_quality(course_data* a, course_data* b);
int compare_dificulty(course_data* a, course_data* b);
int compare_instructor_quality(course_data* a, course_data* b);
int average_enrollment(data_container* d);
double average_difficulty(data_container* d);
double average_quality(data_container* d);
double average_instructor_quality(data_container* d);
