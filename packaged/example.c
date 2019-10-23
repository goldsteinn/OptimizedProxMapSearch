#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <stdint.h>
#include "utils/printing.h"
#include "prepsort/prepsort.h"
#include "sorts/timsort.h"
#include "utils/arg.h"




//sort.h stuff
#define SORT_NAME sorter_int
#define SORT_TYPE int
#include "sorts/sort.h"
#undef SORT_NAME
#undef SORT_TYPE

#define SORT_NAME sorter_ul
#define SORT_TYPE long
#include "sorts/sort.h"

#define SORT_CMP(x, y) (x, y)
#define MAX(x,y) (((x) > (y) ? (x)   : (y)))
#define MIN(x,y) (((x) < (y) ? (x)   : (y)))




//prepsort stuff
inline unsigned long swapUL(unsigned long x){
  x=(((x & 0xffffffff00000000ull)>>32ull)|((x & 0x0000000ffffffffull)<<32ull));
  x=(((x & 0xffff0000ffff0000ull)>>16ull)|((x & 0x0000ffff0000ffffull)<<16ull));
  x=(((x & 0xff00ff00ff00ff00ull)>>8ull)|((x & 0x00ff00ff00ff00ffull)<<8ull));
  return x;
}

inline unsigned long swapUI(unsigned long x){
  x=(((x & 0xffff0000)>>16)|((x & 0x0000ffff)<<16));
  x=(((x & 0xff00ff00)>>8)|((x & 0x00ff00ff)<<8));
  return x;
}

inline unsigned long swapUS(unsigned long x){
  x=(((x & 0xff00)>>8)|((x & 0x00ff)<<8));
  return x;
}


int charCompare(const void*a, const void* b){
  return strcmp((char*)a, (char*)b);
}
int intCompare(const void* a, const void* b){
  return *(int*)a > *(int*)b;
}

int longCompare(const void* a, const void* b){
  return *(long*)a > *(long*)b;
}

void timsort_int2(void* arr, const size_t len, const size_t itemSize,
	      int (*compar)(const void*, const void*)){
  sorter_int_tim_sort(arr, len);
}
void timsort4(void* arr, const size_t len, const size_t itemSize,
	      int (*compar)(const void*, const void*)){
  timsort(arr, len, itemSize, compar);  
}


void timsort_long2(void* arr, const size_t len, const size_t itemSize,
	      int (*compar)(const void*, const void*)){
  sorter_ul_tim_sort(arr, len);
}

unsigned long strVal(const void* str, const size_t itemSize){
  if(itemSize>=8){
    unsigned long val=swapUL(*(unsigned long*)str);
    return val;
  }
  else if(itemSize<8&&itemSize>=4){
    unsigned long val=swapUI((*(unsigned long*)str)&0xffffffff);
    return val;
  }
  else if(itemSize<4&&itemSize>=2){
    unsigned long val=swapUS((*(unsigned long*)str)&0xffff);
    return val;
  }
  else{
    return (*(unsigned long*)str)&0xff;
  }
}

unsigned long intVal(const void* i, const size_t itemSize){
  return *(int*)i;
}

unsigned long longVal(const void* i, const size_t itemSize){
  return *(long*)i;
}


#define Version "0.1"
size_t alen=20;
size_t slen=32;
int rseed=0;
static ArgOption args[] = {
  // Kind, 	  Method,		name,	    reqd,  variable,		help
  { KindHelp,     Help, 	"-h" },
  { KindOption,   Integer,      "--len",            0,     &alen,              "Sets size of arrays to be sorted" },
  { KindOption,   Integer,      "--slen",           0,     &slen,              "Sets size of strings to be sorted" },
  { KindOption,   Integer,      "--seed",           0,     &rseed,             "Set random seed" },
  { KindEnd }
};
static ArgDefs argp = { args, "prepsort example", Version, NULL };
int main(int argc, char** argv){
  ArgParser* ap = createArgumentParser(&argp);
  if(parseArguments(ap, argc, argv)){
    printf("Error parsing arguments!\n");
    return -1;
  }
  alen=1<<alen;
  srand(rseed);
  int* intArr=malloc(alen*sizeof(int)), *intToSort=malloc(alen*sizeof(int));
  for(int i =0;i<alen;i++){
    intArr[i]=rand();
  }

  long* longArr=malloc(alen*sizeof(long)), *longToSort=malloc(alen*sizeof(long));
  for(int i =0;i<alen;i++){
    longArr[i]=rand();
    longArr[i]*=rand();
  }
  
  char* strArr=calloc(alen, slen), *strToSort=calloc(alen,slen);;
  for(int i =0;i<alen;i++){
    int bound=slen;
    for(int j=0;j<bound;j++){
      strArr[(slen*i)+j]=(rand()%255);
    }
  }

  unsigned long sMin=0;
  unsigned long sMax=1<<(MIN(63,(slen<<3)));
  sMax--;
  unsigned long iMax=RAND_MAX;
  unsigned long iMin=0;
  unsigned long lMax=RAND_MAX;
  lMax*=RAND_MAX;
  unsigned long lMin=0;
  sort_funs f;
  struct timeval t1, t2;

  //testing special int case
  f.sort_fun_0=timsort_int2;
  memcpy(intToSort, intArr, sizeof(int)*alen);
  gettimeofday(&t1, NULL);
  prepSort(intToSort, alen, sizeof(int), 1,
	   (const void*)(&iMax),(const void*)(&iMin),
	   intVal, intCompare, f);
  gettimeofday(&t2, NULL);
  doPrint(0, getDiff(t1, t2));

  f.sort_fun_0=timsort_long2;
  memcpy(longToSort, longArr, sizeof(long)*alen);
  gettimeofday(&t1, NULL);
  prepSort(longToSort, alen, sizeof(long), 1,
	   (const void*)(&lMax),(const void*)(&lMin),
	   longVal, longCompare, f);
  gettimeofday(&t2, NULL);
  doPrint(7, getDiff(t1, t2));

  f.sort_fun_0=timsort4;
  memcpy(strToSort, strArr, slen*alen);
  gettimeofday(&t1, NULL);
  prepSort(strToSort, alen, slen,0,
	   (const void*)(&sMax),(const void*)(&sMin),
	   strVal, charCompare, f);
  gettimeofday(&t2, NULL);
  doPrint(1, getDiff(t1, t2));



  memcpy(intToSort, intArr, sizeof(int)*alen);
  gettimeofday(&t1, NULL);
  sorter_int_quick_sort(intToSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(2, getDiff(t1, t2));

  memcpy(longToSort, longArr, sizeof(long)*alen);
  gettimeofday(&t1, NULL);
  sorter_ul_quick_sort(longToSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(3, getDiff(t1, t2));


  memcpy(strToSort, strArr, slen*alen);
  gettimeofday(&t1, NULL);
  qsort(strToSort, alen, slen, charCompare);  
  gettimeofday(&t2, NULL);
  doPrint(3, getDiff(t1, t2));

  
  free(strToSort);
  free(intToSort);
  free(longToSort);
  free(longArr);
  free(intArr);
  free(strArr);
}
