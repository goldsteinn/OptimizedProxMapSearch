#ifndef PRINTING_H
#define PRINTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//util stuff
#define printLen 24
#define numTypes 6
#define numFuns 4


	      
void doPrint(int funNum, int typeNum, double tDif);
double getDiff(struct timeval t1, struct timeval t2);

#endif
