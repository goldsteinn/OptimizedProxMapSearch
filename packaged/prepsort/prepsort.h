#ifndef PREPSORT_H
#define PREPSORT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prepSort(void* arr, const size_t len, const size_t itemSize, const void* max,const void* min,
	      unsigned long (*val_fun)(const void*, const size_t),
	      int (*compar_fun)(const void*, const void*),
	      void (*sort_fun)(void*, const size_t, const size_t,
			       int (*compar_fun)(const void*, const void*)));


#endif
