#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>


typedef float stype; //define data type to sort (keep it 4 bytes for now)
#define SORT_NAME sorter
#define SORT_TYPE stype


#include "sort/sort.h"


//sort.h stuff
#define SORT_CMP(x, y) (x - y)
#define MAX(x,y) (((x) > (y) ? (x)   : (y)))
#define MIN(x,y) (((x) < (y) ? (x)   : (y)))



#define doBucketSort 256
#define doQuickSort 256
#define queSize 256


typedef struct bucket_head{
  int size;
  int index;
  stype min;
  stype max;
}bucket_head;

typedef struct node{
  int index;
  stype val;
}node;

typedef struct todos{

  bucket_head t;
  int astart;
  struct todos* next;
}todos;


typedef union changer_4{
  int iform;
  float fform;
  stype sform;
}changer_4;

const double max=(stype)(RAND_MAX)+1.1;
const stype stypeMax=(stype)(RAND_MAX);;

int myBuckets[doBucketSort]={0};
bucket_head init={.size=0,.index=0,.min=RAND_MAX,.max=0};

void proxMapSort_start(stype* arr, const int alen);
int getSlot(stype val, const double rmax, const int bound);
void addNode(bucket_head* table, node* tnode, const stype val, const int nindex);
void sorter_bucket_sort(stype* arr, const int alen, int buckets[doBucketSort], const int offset);
void proxMapSort_recursive(stype* arr, const int alen, int tsize,bucket_head* table, node* nodes, double bound, int indexStart);
void proxMapSort_internal(bucket_head* table, node* tnodes, const int alen, stype* arr);

