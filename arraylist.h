#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArrayList {
  int* values;     // the underlying array of values
  size_t capacity; // the maximum number of elements that can be held, i.e. the size of the underlying array
  size_t size;     // the current number of elements in the arraylist
} arraylist;

/*
 * Print out all elements in the array list 
 */
void print_list(arraylist* al){
  // Edge case: if list is null
  if(al == NULL){
    printf("Please initialize an ArrayList.\n"); 
    return ; 
  }
  // Edge case: if size of the array is 0, prompt user that array is empty 
  if(al->size == 0)
    printf("Array is empty. Please populate the array.\n");

  // print list of elements in array 
  int* temp = al->values; 
  for(int index = 0; index < al->size; index++){
    printf("list[%d]: %d\n",index, *temp);
    temp++; 
  }

  printf("Size: %zu\nCapacity: %zu\n", al->size, al->capacity);
}

/*
 * Initializes an arraylist by allocating space for it on the heap,
 * then allocating space for its underlying array. It then initializes
 * the capacity and sets the size to 0 (since it's empty).
 * Returns a pointer to the arraylist if successfully created, and returns
 * NULL if any error occurs.
 */
arraylist* al_initialize(size_t capacity) {
  /* 
   * Edge case: if capacity is 0, return NULL. ArrayList should have a         * capacity greater than or equal to 1. Print prompt to user.  
   */    
  int capacity_signed = capacity; // convert capacity to signed int
  if(capacity_signed < 1){
    printf("The capacity of your ArrayList must be greater than or equal to 1. Please enter array with an appropriate capacity.\n");
    return NULL;
  }

  // create space for ArrayList 
  arraylist* list = malloc(sizeof(arraylist));
  if(list == NULL)
    return NULL; 

  // create space for the underlying array and list capacity/ current size 
  list->values = malloc(sizeof(int)*capacity);
  if(list->values == NULL)
    return NULL; 

  // initialize values in array to avoid errors in valgrind 
  int* temp = list->values; 
  for(int i = 0; i < capacity; i++){
    *temp = 0; 
    temp++; 
  }

  // initialize capacity and size 
  list->capacity = capacity; 
  list->size = 0; 

  return list; // return initialized array list 
}

/*
 * Adds a value to the list in the next available spot.
 * If the list is not big enough to hold another value, 
 * it should be grown as described in the assignment.
 * The function should return 1 if the element is added to the list
 * and 0 if an error occurs.
 */
unsigned int al_add(arraylist* al, int value) {
  // Edge case: check to see if ArrayList is null 
  if(al == NULL){
    printf("Please initialize an ArrayList.\n"); 
    return 0; 
  }

  /* 
   * If the current size of the array list is less than its capacity 
   * then add value to the array list at the appropriate index location.       * Else, grow the array by twice the capacity.    
   */                                                                        
  if(al->size < al->capacity){
    int* temp = al->values; 
    int size = al->size; 
    temp[size] = value; 
    al->size++; // increase size of array
    return 1; // success 
  }
  else{ 
    int* newArray = malloc(2*sizeof(int)*al->capacity); // create new array 
    
    // initialize values in array to avoid errors in valgrind 
    int* temp1 = newArray; 
    for(int i = 0; i < 2*al->capacity; i++){
      *temp1 = 0; 
      temp1++; 
    } 

    if(newArray==NULL)
      return 0; 

    memcpy(newArray,al->values, sizeof(int)*al->capacity); // copy old values to new array 
    free(al->values); // free old array pointer 
    al->values = newArray; // update ArrayList with new array and capacity 
    al->capacity = 2*al->capacity; // update capacity 

    // add value to array 
    int* temp = newArray; 
    int size = al->size; 
    temp[size] = value;  
    al->size++;  
    return 1; // success 
  }
  return 0; // error has occured 
}

/*
 * Determines whether the list contains the value.
 * Returns its 0-based index if it is found, -1 if it is not.
 * The function returns -2 if the arraylist or its underlying array are NULL.
 */
int al_contains(arraylist* al, int value) {
  
  // Edge case: check to see if arraylist is null or array is empty 
  if(al == NULL || al->size == 0){
    printf("ArrayList was not intialized or is empty.\n"); 
    return -2; 
  }

  // see if array contains values
  int* temp = al->values; 
  for(int index = 0; index < al->size; index++){
    if(*temp == value){
      return index;  
    }
    temp++; 
  }

  return -1;
}

/*
 * Reduce the capacity of the array by 50% if the size is less than 25% of the 
 * initial capacity. Make sure to copy the previous values into a new array   * with a new capacity using memcpy(). 
 */
void al_reduce_capacity(arraylist* al){
  int* temp = al->values; // store pointer to current underlaying array
  al->capacity = al->capacity/2;  
  al->values = malloc(sizeof(int)*al->capacity); // create memory for new underlaying array 
  memcpy(al->values, temp, sizeof(int)*al->capacity); // copy contents from old to new array 
  free(temp); // free memory used by previous underlaying array 
}

/*
 * Removes the element at the specified index (0-based) from the arraylist.
 * Then shifts all other elements so that there is no "hole" in the 
 * underlying array.
 * If the size is less than 25% of the capacity, it should resize
 * the underlying array as described in the assignment.
 * The function should return 1 if the element is successfully removed from the list.
 * It should return 0 if the specified index is greater than or equal to the size.
 * It should return -1 if the arraylist or its underlying array are NULL, 
 * or if an error occurs in resizing the array.
 */
int al_remove(arraylist* al, unsigned int index) {
// Edge case: if arraylist is null return -1 
  if(al == NULL)
    return -1; 

  // Edge case: if index is equal to or greater than size of array return 1 
  if(al != NULL && index >= al->size)
    return 0; 

  // cycle through loop to determine element to remove once (this will occur when index = 0)
  int* temp = al->values; 

  // store counter for which index to remove 
  unsigned int counter = al->size - index - 1; 
 
  while(index != -1){
    // once index hits element, remove element 
    if(index == 0){
      *temp = 0; 
      int* nextValues = temp; // store next values 
      nextValues++; // update pointer 
      
      // shift list up by one index 
      while(counter!=0){
        *temp = *nextValues; 
        temp++; 
        nextValues++; 
        counter--; 
      }

      // set last value to null 
      nextValues--; 
      *nextValues = 0;  
      al->size--; // decrement size 
      
      // if size of array list is less than 25% of capacity, then reduce the capacity of the list 
      if(al->size < (al->capacity/4)) {
        al_reduce_capacity(al);
        printf("got here");
      }

      return 1; 
    }
    temp++; 
    index--; 
  }
  return -1;
}

/*
 * Frees all memory used by the arraylist, including the arraylist struct itself.
 */
void al_free(arraylist* al) {
  // Edge case: check to see if pointer is not null 
  if(al==NULL){
    printf("ArrayList is not initialized.\n"); 
    return; 
  }

  // free underlaying arraylist and arraylist structure 
  free(al->values); 
  free(al); 
}
    
 

