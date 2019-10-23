#ifndef PREPSORT_H
#define PREPSORT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//if you want to add a special case add function pointer to union
typedef union sort_funs{
  void (*sort_fun_0)(void*, const size_t, const size_t,int (*compar_fun)(const void*, const void*));
  void (*sort_fun_1)(int*, const size_t);
  void (*sort_fun_2)(long*, const size_t);
}sort_funs;


//arr to be sorted
//length of array (amount of items)
//size of each item (thus total array size is len*itemSize)
//bool of whether it can be sorted as int/ul/float/etc...
//max possible value of values in array
//min possible value of values in array
//function that turns type being sorted into a value (really only for strings...)
//compare function as many sorts require that
//sort_funs is union of sort functions that can be used, just cleans up cases
void prepSort(void* arr, const size_t len, const size_t itemSize, int ordered,
	      const void* max,const void* min,
	      unsigned long (*val_fun)(const void*, const size_t),
	      int (*compar_fun)(const void*, const void*),sort_funs f);



	
#endif
