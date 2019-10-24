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


#define nfuns 3
#define toRun(f,i, x,y,z,f1, f2, f3){		\
    if(i+1==x){					\
      f.sort_fun_generic=f1;			\
    }						\
    if(i+1==y){					\
      f.sort_fun_generic=f2;	\
    }						\
    if(i+1==z){					\
      f.sort_fun_generic=f3;	\
    }						\
  }
//arg processing
#define Version "0.1"
size_t alen=20;
size_t slen=32;
int rseed=0;
int doChar=0, doShort=0, doInt=0, doLong=0, doStr=0, doAll=0;
int doQS=0, doMS=0, doTS=0, doAS=0;

static ArgOption args[] = {
  // Kind, 	  Method,		name,	    reqd,  variable,		help
  { KindHelp,     Help, 	"-h" },
  { KindOption,   Integer,      "--len",            0,     &alen,              "Sets power for array size (i.e size= 1<<len)" },
  { KindOption,   Integer,      "--slen",           0,     &slen,              "Sets size of strings to be sorted" },
  { KindOption,   Integer,      "--seed",           0,     &rseed,             "Set random seed" },
  { KindOption,   Set,          "--char",           0,     &doChar,            "do char sort test" },
  { KindOption,   Set,          "--short",          0,     &doShort,           "do short sort test" },
  { KindOption,   Set,          "--int",            0,     &doInt,             "do int sort test" },
  { KindOption,   Set,          "--long",           0,     &doLong,            "do long sort test" },
  { KindOption,   Set,          "--str",            0,     &doStr,             "do str sort test" },
  { KindOption,   Set,          "--all",            0,     &doAll,             "do all sort tests" },
  { KindOption,   Set,          "--qs",             0,     &doQS,              "do prep then quicksort" },
  { KindOption,   Set,          "--ms",             0,     &doMS,              "do prep then mergesort" },
  { KindOption,   Set,          "--ts",             0,     &doTS,              "do prep then timsort" },
  { KindOption,   Set,          "--as",             0,     &doAS,              "do all sorts" },
  
  { KindEnd }
};

static ArgDefs argp = { args, "prepsort example", Version, NULL };


//sort.h stuff

#define SORT_NAME sorter_ul
#define SORT_TYPE long
#include "sorts/sort.h"

#undef SORT_NAME
#undef SORT_TYPE

#define SORT_NAME sorter_int
#define SORT_TYPE int
#include "sorts/sort.h"

#undef SORT_NAME
#undef SORT_TYPE

#define SORT_NAME sorter_sh
#define SORT_TYPE short
#include "sorts/sort.h"

#undef SORT_NAME
#undef SORT_TYPE

#define SORT_NAME sorter_ch
#define SORT_TYPE char
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


int charCompare(const void* a, const void* b){
  return *(char*)a > *(char*)b;
}

int shortCompare(const void* a, const void* b){
  return *(short*)a > *(short*)b;
}

int intCompare(const void* a, const void* b){
  return *(int*)a > *(int*)b;
}

int longCompare(const void* a, const void* b){
  return *(long*)a > *(long*)b;
}

int strCompare(const void*a, const void* b){
  return strcmp((char*)a, (char*)b);
}


void quicksort_char(void* arr, const size_t len, const size_t itemSize,
		    int (*compar)(const void*, const void*)){
  sorter_ch_quick_sort(arr, len);
}

void quicksort_short(void* arr, const size_t len, const size_t itemSize,
		     int (*compar)(const void*, const void*)){
  sorter_sh_quick_sort(arr, len);
}

void quicksort_int(void* arr, const size_t len, const size_t itemSize,
		   int (*compar)(const void*, const void*)){
  sorter_int_quick_sort(arr, len);
}

void quicksort_long(void* arr, const size_t len, const size_t itemSize,
		    int (*compar)(const void*, const void*)){
  sorter_ul_quick_sort(arr, len);
}

void quicksort_generic(void* arr, const size_t len, const size_t itemSize,
		       int (*compar)(const void*, const void*)){
  qsort(arr, len, itemSize, compar);  
}



void timsort_char(void* arr, const size_t len, const size_t itemSize,
		  int (*compar)(const void*, const void*)){
  sorter_ch_tim_sort(arr, len);
}

void timsort_short(void* arr, const size_t len, const size_t itemSize,
		   int (*compar)(const void*, const void*)){
  sorter_sh_tim_sort(arr, len);
}

void timsort_int(void* arr, const size_t len, const size_t itemSize,
		 int (*compar)(const void*, const void*)){
  sorter_int_tim_sort(arr, len);
}

void timsort_long(void* arr, const size_t len, const size_t itemSize,
		  int (*compar)(const void*, const void*)){
  sorter_ul_tim_sort(arr, len);
}

void timsort_generic(void* arr, const size_t len, const size_t itemSize,
		     int (*compar)(const void*, const void*)){
  timsort(arr, len, itemSize, compar);  
}


void mergesort_char(void* arr, const size_t len, const size_t itemSize,
		    int (*compar)(const void*, const void*)){
  sorter_ch_merge_sort_in_place(arr, len);
}

void mergesort_short(void* arr, const size_t len, const size_t itemSize,
		     int (*compar)(const void*, const void*)){
  sorter_sh_merge_sort_in_place(arr, len);
}

void mergesort_int(void* arr, const size_t len, const size_t itemSize,
		   int (*compar)(const void*, const void*)){
  sorter_int_merge_sort_in_place(arr, len);
}

void mergesort_long(void* arr, const size_t len, const size_t itemSize,
		    int (*compar)(const void*, const void*)){
  sorter_ul_merge_sort_in_place(arr, len);
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


unsigned long charVal(const void* i, const size_t itemSize){
  return *(char*)i;
}

unsigned long shortVal(const void* i, const size_t itemSize){
  return *(short*)i;
}

unsigned long intVal(const void* i, const size_t itemSize){
  return *(int*)i;
}

unsigned long longVal(const void* i, const size_t itemSize){
  return *(long*)i;
}

void charTest(){
  char* arr=malloc(alen*sizeof(char)), *toSort=malloc(alen*sizeof(char));
  for(int i =0;i<alen;i++){
    arr[i]=rand()&0x7f;
  }

  unsigned long cMin=0;
  unsigned long cMax=0x7f;

  sort_funs f;
  struct timeval t1, t2;
  for(int i=0;i<nfuns;i++){
    f.sort_fun_generic=NULL;
    toRun(f, i, doTS, doMS, doQS, timsort_char, mergesort_char, quicksort_char);
    if(!f.sort_fun_generic){
      continue;
    }
    memcpy(toSort, arr, sizeof(char)*alen);
    gettimeofday(&t1, NULL);
    prepSort(toSort, alen, sizeof(char), 1,
	     (const void*)(&cMax),(const void*)(&cMin),
	     charVal, charCompare, f);
    gettimeofday(&t2, NULL);
    doPrint(i,3, getDiff(t1, t2));
  }
  
  memcpy(toSort, arr, sizeof(char)*alen);
  gettimeofday(&t1, NULL);
  sorter_ch_quick_sort(toSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(3,3, getDiff(t1, t2));

  free(toSort);
  free(arr);
}
void shortTest(){
  short* arr=malloc(alen*sizeof(short)), *toSort=malloc(alen*sizeof(short));
  for(int i =0;i<alen;i++){
    arr[i]=rand()&0x7fff;
  }

  unsigned long sMin=0;
  unsigned long sMax=0x7fff;

  sort_funs f;
  struct timeval t1, t2;
  for(int i=0;i<nfuns;i++){
    f.sort_fun_generic=NULL;
    toRun(f,i, doTS, doMS, doQS, timsort_short, mergesort_short, quicksort_short);
    if(!f.sort_fun_generic){
      continue;
    }
    memcpy(toSort, arr, sizeof(short)*alen);
    gettimeofday(&t1, NULL);
    prepSort(toSort, alen, sizeof(short), 1,
	     (const void*)(&sMax),(const void*)(&sMin),
	     shortVal, shortCompare, f);
    gettimeofday(&t2, NULL);
    doPrint(i,2, getDiff(t1, t2));
  }
  
  memcpy(toSort, arr, sizeof(short)*alen);
  gettimeofday(&t1, NULL);
  sorter_sh_quick_sort(toSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(3,2, getDiff(t1, t2));

  
  free(toSort);
  free(arr);

}
void intTest(){
  int* arr=malloc(alen*sizeof(int)), *toSort=malloc(alen*sizeof(int));
  for(int i =0;i<alen;i++){
    arr[i]=rand();
  }

  unsigned long iMax=RAND_MAX;
  unsigned long iMin=0;

  sort_funs f;
  struct timeval t1, t2;
  for(int i=0;i<nfuns;i++){
    f.sort_fun_generic=NULL;
    toRun(f, i,doTS, doMS, doQS, timsort_int, mergesort_int, quicksort_int);
    if(!f.sort_fun_generic){
      continue;
    }
    memcpy(toSort, arr, sizeof(int)*alen);
    gettimeofday(&t1, NULL);
    prepSort(toSort, alen, sizeof(int), 1,
	     (const void*)(&iMax),(const void*)(&iMin),
	     intVal, intCompare, f);
    gettimeofday(&t2, NULL);
    doPrint(i,1, getDiff(t1, t2));
  }
  
  memcpy(toSort, arr, sizeof(int)*alen);
  gettimeofday(&t1, NULL);
  sorter_int_quick_sort(toSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(3,1, getDiff(t1, t2));
  free(toSort);
  free(arr);

}
void longTest(){
  long* arr=malloc(alen*sizeof(long)), *toSort=malloc(alen*sizeof(long));
  for(int i =0;i<alen;i++){
    arr[i]=rand();
    arr[i]*=rand();
  }
  unsigned long lMax=RAND_MAX;
  lMax*=RAND_MAX;
  unsigned long lMin=0;

  sort_funs f;
  struct timeval t1, t2;
  for(int i=0;i<nfuns;i++){
    f.sort_fun_generic=NULL;
    toRun(f, i, doTS, doMS, doQS, timsort_long, mergesort_long, quicksort_long);
    if(!f.sort_fun_generic){
      continue;
    }
    memcpy(toSort, arr, sizeof(long)*alen);
    gettimeofday(&t1, NULL);
    prepSort(toSort, alen, sizeof(long), 1,
	     (const void*)(&lMax),(const void*)(&lMin),
	     longVal, longCompare, f);
    gettimeofday(&t2, NULL);
    doPrint(i,0, getDiff(t1, t2));
  }
  
  memcpy(toSort, arr, sizeof(long)*alen);
  gettimeofday(&t1, NULL);
  sorter_ul_quick_sort(toSort, alen);
  gettimeofday(&t2, NULL);
  doPrint(3,0, getDiff(t1, t2));
  free(toSort);
  free(arr);

}
void strTest(){
  char* arr=calloc(alen, slen), *toSort=calloc(alen,slen);;
  for(int i =0;i<alen;i++){
    for(int j=0;j<slen;j++){
      arr[(slen*i)+j]=(rand()%255);
    }
  }
  unsigned long strMin=0;
  unsigned long strMax=0;
  if(slen>=8){
    strMax=~strMax;
  }
  else{
    strMax=(1UL<<((slen<<3)))-1;
  }


  sort_funs f;
  struct timeval t1, t2;
  doMS=0;
  for(int i=0;i<nfuns;i++){
    f.sort_fun_generic=NULL;
    toRun(f, i, doTS, doMS, doQS, timsort_generic, NULL, quicksort_generic);
    if(!f.sort_fun_generic){
      continue;
    }
    memcpy(toSort, arr, slen*alen);
    gettimeofday(&t1, NULL);
    prepSort(toSort, alen, slen,0,
	     (const void*)(&strMax),(const void*)(&strMin),
	     strVal, strCompare, f);
    gettimeofday(&t2, NULL);
    doPrint(i,4, getDiff(t1, t2));
  }



  memcpy(toSort, arr, slen*alen);
  gettimeofday(&t1, NULL);
  qsort(toSort, alen, slen, strCompare);  
  gettimeofday(&t2, NULL);
  doPrint(3,4, getDiff(t1, t2));
  free(toSort);
  free(arr);
}

int main(int argc, char** argv){
  ArgParser* ap = createArgumentParser(&argp);
  if(parseArguments(ap, argc, argv)){
    printf("Error parsing arguments!\n");
    return -1;
  }
  doMS=doMS*2;
  doQS=doQS*3;
  if(doAS){
    doMS=2;
    doQS=3;
    doTS=1;
  }
  alen=1<<alen;
  srand(rseed);


  if(doChar||doAll){
    charTest();
  }
  if(doShort||doAll){
    shortTest();
  }
  if(doInt||doAll){
    intTest();
  }
  if(doLong||doAll){
    longTest();
  }
  if(doStr||doAll){
    strTest();
  }
}
