#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>




double getDiff(struct timespec t1, struct timespec t2){
  double ms1=t1.tv_sec*1000000 + t1.tv_nsec/1000.0;
  double ms2=t2.tv_sec*1000000 + t2.tv_nsec/1000.0;
  return ms2-ms1;
}


int log_upper(int i){
  i--;
    i |= (i >>  1);
    i |= (i >>  2);
    i |= (i >>  4);
    i |= (i >>  8);
    i |= (i >> 16);
   i++;
   return  __builtin_ctz(i);
}

int* scan_1(int* arr, int len){
  int upper_bound = log_upper(len);
  int* arr2 = calloc(len, sizeof(int));
  for(int i=0;i<=upper_bound;i++){
    for(int j=0;j<(1<<i);j++){
      arr2[j]=arr[j];
    }
    for(int j=(1<<i);j<(len);j++){
      arr2[j]=arr[j]+arr[j-(1<<(i))];
    }
    int* temp = arr;
    arr = arr2;
    arr2 = temp;
  }
  return arr2;
}


void scan_2(int* arr, int len){
  int upper_bound = log_upper(len)-1;
  for(int i=0;i<=upper_bound;i++){
    for(int j=0;j<len;j+=(1<<(i+1))){
	arr[j + (1<<(i+1))-1] = arr[j + (1<<(i))-1] + arr[j + (1<<(i+1))-1];
    }
  }
    arr[len-1] = 0;
  for(int i=upper_bound;i>=0; i--){
    for(int j=0;j<len;j+=(1<<(i+1))){
      int t = arr[j + (1<<(i))-1];
      arr[j + (1<<(i))-1] =  arr[j + (1<<(i+1))-1];
      arr[j + (1<<(i+1))-1]+= t;
    }
  }
}


#define size (1<<22)
#define max RAND_MAX
int main(){
  srand(time(NULL));
  int* arr= (int*)calloc(size, sizeof(int));
  for(int i=0;i<size;i++){
    arr[i] = rand()%max;
  }

  int* arr2= (int*)calloc(size, sizeof(int));
  memcpy(arr2, arr, sizeof(int)*size);

  struct timespec t1, t2, t3;
  clock_gettime(CLOCK_MONOTONIC, &t1);
  arr = scan_1(arr, size);
  clock_gettime(CLOCK_MONOTONIC, &t2);
  scan_2(arr2, size);
  clock_gettime(CLOCK_MONOTONIC, &t3);
  printf("Scan1: %2.lf us -> (%2.lf ms)\n", getDiff(t1, t2), getDiff(t1, t2)/1000.0);
  printf("Scan2: %2.lf us -> (%2.lf ms)\n", getDiff(t2, t3), getDiff(t2, t3)/1000.0);
;

}
