#ifndef PREPSORT_H
#define PREPSORT_H
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prepSort(void* arr, const int len, const int itemSize, const void* max,const void* min,
	      unsigned long (*val_fun)(const void*, const int),
	      int (*compar_fun)(const void*, const void*),
	      void (*sort_fun)(void*, const int, const int,
			       int (*compar_fun)(const void*, const void*)));
