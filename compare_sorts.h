#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

typedef int stype; //define data type to sort (keep it 4 bytes for now)
#define SORT_NAME sorter
#define SORT_TYPE stype


#include "sort/sort.h"


//sort.h stuff
#define SORT_CMP(x, y) (x - y)
#define MAX(x,y) (((x) > (y) ? (x)   : (y)))
#define MIN(x,y) (((x) < (y) ? (x)   : (y)))

//plug in whatever values you want here (i find len == tsize works best)
#define loglen 24
#define len (1<<loglen)  //length of array to be sorted
#define logsize 24    //log of table size
#define tsize (1<<logsize)  //actual table size


//TODO kendall tau distance
//TODO better ways to sort each slot
//Adding a queue for each table slot maybe? https://stackoverflow.com/questions/3759112/whats-faster-inserting-into-a-priority-queue-or-sorting-retrospectively

//instead of each bucket being linked list store offset from base for next (faster)
#define raPtr 1
#ifdef raPtr

//instead of storing head of table slot as pointer, store it also as offset from base (fastest)
#define raTablePtr 1 
//#define preBase
#endif

#define useFloat 1 //to use float division to get slot (otherwise uses lower logsize bits)

//comment out the two verifications once your content that its not broken (it double testing time)
//#define superVer 1 //verifies sorted elements are exact right elements
//#define ver 1 //verifies elements were actually sorted

//these #defines are the possible methods you can run (comment out and they wont run)
//every set of 4 is a different sort method, i.e 0-3 uses normal mergesort
//#define NMS 0  //just running the sort algorithm on random data
//  #define GRNMS 1 //running sort algorithm on data with alot of 'runs'
//  #define PPNMS 2 //running sort sort after using my preprocessing method
//  #define PPGRNMS 3 //running sort on data with alot of 'runs' that was also preprocesses

  //inplace mergesort
//  #define IMS 4
//  #define GRIMS 5
  #define PPIMS 6
//  #define PPGRIMS 7

//quicksort from sort.h
#define CQS 8
/*#define GRCQS 9
  #define PPCQS 10
  #define PPGRCQS 11

  //quicksort from math.h
  #define TEST 12 
  #define GRTEST 13*/
  #define PPTEST 14
/*  #define PPGRTEST 15

  //timsort from timsort.h (this idea for pre processing data
  //was origionally with timsort in mind)
  #define TS1 16 
  #define GRTS1 17  
  #define PPTS1 18  
  #define PPGRTS1 19*/

  //timsort from sort.h
//  #define TS2 20 
//  #define GRTS2 21  
  #define PPTS2 22  
//  #define PPGRTS2 23

  //sqrt sort from sort.h
/*  #define SQ 24
  #define GRSQ 25
  #define PPSQ 26
  #define PPGRSQ 27*/

//insert sort on each slot of preproccessed data
//note: straight insert sort not included
#define PPINS 30
//#define PPGRINS 31

//another method of doing insert sort on
//preproccessed data
//note: again only used on preprocessed data
#define PPSA 34
//#define PPGRSA 35

//insert to bucket list sorted
//#define PPANIS 38
//#define PPGRANIS 39


//quicksort on sub arrays as data is got
//node: only on preprocessed
//#define PPQSA 42 
//#define PPGRQSA 43


//mergesort on sub arrays as data is got
//node: only on preprocessed
#define PPMSA 46
//#define PPGRMSA 47


//timsort on sub arrays as data is got
//node: only on preprocessed
//#define PPTSA 50
//#define PPGRTSA 51

//sqrt on sub arrays as data is got
//node: only on preprocessed
//#define PPSQA 54
//#define PPGRSQA 55


  //shell on sub arrays as data is got
  //node: only on preprocessed
//  #define PPSHA 58
//#define PPGRSHA 59

  //shell on sub arrays as data is got
  //node: only on preprocessed
  /*#define SHS 60
  #define GRSHS 61
  #define PPSHS 62
  #define PPGRSHS 63*/



//inverts order of preprocessed table (not wise to use/will break many of the sortAsGet tests)
//#define invert

//just stuff for printing results
#define printLen 45
#define numTypes 4
#define numFuns 16
const char types[numTypes][32]={
"Rand",
  "Gen Runs" ,
  "Rand Pre Processed",
  "Gen Runs Pre Processes"
  };

const char funs[numFuns][32]={
"Merge Sort[Normal]",
  "Merge Sort[In Place]" ,
  "Quick Sort['Sort.h']",
  "Test Bucket Times",
  "Tim Sort['Timsort.h']",
  "Tim Sort['Sort.h']",
  "Sqrt Search",
  "Insert Sort After Get",
  "Insert Sort as Get",
  "LL Insert Sort as Add",
  "Quicksort After Get",
  "Mergesort After Get",
  "Timsort After Get",
  "Sqrt Sort After Get",
  "Shell Sort After Get",
  "Shell Sort"
  };

//node for precprocess
typedef struct node{
#ifdef raPtr
int index;
#else
struct node* next;
#endif
stype val;
} node;


//const for getting slot in table for preprocesing elements
const double max= (stype)(RAND_MAX)+1.1;


//prints for a given run
void doPrint(int defNum, double tDif){
int total=strlen(types[defNum%numTypes]);
total+= strlen(funs[defNum/numTypes]);
printf("--------------------------------------------------------------------------\n");
printf("%s %s", types[defNum%numTypes], funs[defNum/numTypes]);
for(int i =total;i<printLen;i++){
printf(" ");
}
printf(": %.2lf ms\n", tDif);
printf("--------------------------------------------------------------------------\n");

}

//verifies array is sorted
void verCorrect(stype* arr){
for(int i =1;i<len;i++){
if(arr[i]<arr[i-1]){
printf("Bad Sort!\n");
return;
}
}
}

//adds a node to table for preprocess
void addNode(node* newNode,
#ifdef raTablePtr
	       int*
#else
	       node**
#endif
	       table
#ifdef raPtr
	       , long base
#ifdef preBase
	       , int ind
#endif
#endif
	       ){
#ifdef useFloat
  unsigned int slot=(unsigned int)((newNode->val/max)*(tsize));
#else
  unsigned int slot=newNode->val>>(32-logsize);
#endif
  
#ifdef invert
#ifdef raPtr
#ifdef raTablePtr
  newNode->index=table[len-slot-1];
#ifdef preBase
  table[line-slot-1]=ind;
#else
  table[line-slot-1]=(int)(((long)newNode)-base);
#endif

#else
  if(table[len-slot-1]){
    newNode->index=(int)(((long)table[len-slot-1])-base);
  }
  else{
    newNode->index=-1;
  }
  table[len-slot-1]=newNode;
#endif
#else
  newNode->next=table[len-slot-1];
  table[len-slot-1]=newNode;
#endif

#endif
#ifndef invert
#ifdef raPtr
#ifdef raTablePtr
  newNode->index=table[slot];
#ifdef preBase
  table[slot]=ind;
#else
  table[slot]=(int)(((long)newNode)-base);
#endif
#else
  if(table[slot]){
    newNode->index=(int)(((long)table[slot])-base);
  }
  else{
    newNode->index=-1;
  }
  table[slot]=newNode;
#endif
#else
  newNode->next=table[slot];
  table[slot]=newNode;
#endif


#endif
}

//adds a node to table for preprocess
void addNodeSort(node* newNode,
#ifdef raTablePtr
		 int*
#else
		 node**
#endif
		 table
#ifdef raPtr
		 , long base
#endif
		 ){
  stype val=newNode->val;
#ifdef useFloat
  unsigned int slot=(unsigned int)((val/max)*(tsize));
#else
  unsigned int slot=val>>(32-logsize);
#endif
#ifdef raTablePtr
  if(table[slot]==-1){
    table[slot]=(int)((((long)newNode))-base);;
    newNode->index=-1;
    return;
  }
  node* temp=(node*)(table[slot]+base);
  node* prev=temp;
  while(prev->index!=-1){
    if(val<temp->val){
      break;
    }
    prev=temp;
    temp=(node*)(temp->index+base);
  }
  newNode->index=prev->index;
  prev->index=(int)((((long)newNode))-base);
  return;
#else
  if(!table[slot]){
    table[slot]=newNode;
#ifdef raPtr
    newNode->index=-1;
#endif
    return;
  }

  if(table[slot]->val>val){
#ifdef raPtr
    newNode->index=(int)(((long)table[slot])-base);
#else
    newNode->next=table[slot];
    
#endif
    table[slot]=newNode;
    return;
  }
#ifdef raPtr
  node* temp;
  if(table[slot]->index==-1){
    table[slot]->index=(int)((((long)newNode))-base);
    newNode->index=-1;
    return;
  }
  else{
    temp=(node*)(table[slot]->index+base);
  }
#else
  node* temp=table[slot]->next;
#endif
  node* prev=table[slot];
  while(temp){
    if(val<temp->val){
      break;
    }
    prev=temp;
#ifdef raPtr
    if(temp->index==-1){
      temp=NULL;
      break;
    }
    else{
      temp=(node*)(temp->index+base);
    }
#else
    temp=temp->next;
#endif
  }
#ifdef raPtr
  prev->index=(int)((((long)newNode))-base);
  if(temp){
    newNode->index=(int)((((long)temp))-base);
  }
  else{
    newNode->index=-1;
  }
#else
  prev->next=newNode;
  newNode->next=temp;
#endif
#endif
}


//gets all the values from table
void getVals(
#ifdef raTablePtr
	     int*
#else
	     node**
#endif
	     table, stype* arr
#ifdef raPtr
	     , long base
#endif
	     ){
  unsigned int ind=0;
  node* temp;
  for(unsigned int i =0;i<(tsize);i++){
#ifdef raTablePtr
    if(table[i]!=-1){
      temp=(node*)(table[i]+base);
    }
    else{
      continue;
      //      temp=NULL;
    }
#else
    temp=table[i];
#endif
    while(temp){
      arr[ind]=temp->val;
      ind++;
#if defined raPtr || defined raTablePtr
      if(temp->index==-1){
	break;
      }
      else{
	temp=(node*)(temp->index+base);
      }
#else
      temp=temp->next;
#endif

    }
  }
}

//insert sort between two pointers

//insert sort between two pointers
static void insertion_sort_ind(stype *arr, const size_t n) {
  int i, key, j;  
  for (i = 1; i < n; i++) {  
    key = arr[i];  
    j = i - 1;  
    while (j >= 0 && arr[j] > key) {  
      arr[j + 1] = arr[j];  
      j = j - 1;  
    }  
    arr[j + 1] = key;  
  }  
}

//puts an element into subarray in sorted order as their being
//gotten from table
void sortAsAdd(stype* arr, int ind, stype val){
  int i=0;
  for(;i<ind;i++){
    if(arr[i]>val){
      unsigned t=arr[i];
      arr[i]=val;
      val=t;
    }
  }
  arr[i]=val;
}

//gets values sorting as adding
void getValsSortAsAdd(
#ifdef raTablePtr
		      int*
#else
		      node**
#endif
		      table, stype* arr
#ifdef raPtr
		      , long base
#endif
		      ){
  unsigned int ind=0;
  unsigned int indStart=0;
  node* temp;
  for(unsigned int i =0;i<(tsize);i++){
#ifdef raTablePtr
    if(table[i]!=-1){
      temp=(node*)(table[i]+base);
    }
    else{
      temp=NULL;
    }
#else
    temp=table[i];
#endif
    indStart+=ind;
    ind=0;
    while(temp){
      sortAsAdd(arr+indStart, ind, temp->val);
      ind++;
#if defined raPtr || defined raTablePtr
      if(temp->index!=-1){
	temp=(node*)(temp->index+base);
      }
      else{
	break;

      }
#else
      temp=temp->next;
#endif
    }
  }
}

//After getting values for a slot, insert sort it
void getValsSort(
#ifdef raTablePtr
		 int*
#else
		 node**
#endif
		 table, stype* arr,
#ifdef raPtr
		 long base,
#endif
		 void (*sort_function)(stype*, const size_t)){
  unsigned int ind=0;
  unsigned int indStart=0;
  node* temp;
  for(unsigned int i =0;i<(tsize);i++){
#ifdef raTablePtr

    //thinking about redoing loops like this when using indexes instead of ptrs
    /*    indStart=ind;
	  temp=(node*)(table[i]+base);
	  while((long)temp>=base){
	  arr[ind]=temp->val;
	  temp=(node*)(temp->index+base);
	  ind++;
     
	  }*/
    if(table[i]!=-1){
      temp=(node*)(table[i]+base);
    }
    else{
      temp=NULL;
    }
#else
    temp=table[i];
#endif
    indStart=ind;
    while(temp){
      arr[ind]=temp->val;
      ind++;
#if defined raPtr || defined raTablePtr
      if(temp->index!=-1){
	temp=(node*)(temp->index+base);
      }
      else{
	break;
      }
#else
      temp=temp->next;
#endif
    }
    //  #endif
    sort_function(arr+indStart, ind-indStart);
  }
}



//diff two timevals
double getDiff(struct timeval t1, struct timeval t2){
  double ms1=t1.tv_sec*1000 + t1.tv_usec/1000.0;
  double ms2=t2.tv_sec*1000 + t2.tv_usec/1000.0;
  return ms2-ms1;
}


//compare function for sort functions  
static int intcomp(const void* a, const void* b){
  return *(stype*)a - *(stype*)b;
}
