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

#endif /* sort_h */

#include "parse.h"




void swap(void** arr, int a, int b);
int partition_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) );
int compare_enrollment(course_data* a, course_data* b);
void quicksort_data(course_data** data, int p, int r, int (*compare)(course_data*, course_data*) );
