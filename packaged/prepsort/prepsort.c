#include "prepsort.h"


#define getItem(x, y, z) (x+(y*z))
 
typedef struct node{
  int index;
  char val[];
}node;
    
typedef struct node_int{
  int index;
  int val;
}node_int;

typedef struct node_long{
  int index;
  long val;
}node_long;

void prepSort_int(int* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_long(long* arr, const size_t len, const void* max,const void* min, sort_funs f);


inline int getSlot(unsigned long val, const double max, const size_t len){
  return (int)((val/max)*len);
}


inline void setItem(void* arr, void* temp, const size_t itemSize){
  switch (itemSize){
  case 1:
    *((char*)arr)=*(char*)temp;
    break;
  case 2:
    *((short*)arr)=*(short*)temp;
    break;
  case 4:
    *((int*)arr)=*(int*)temp;
    break;
  case 8:
    *((long*)arr)=*(long*)temp;
    break;
  default:
    memcpy(arr, temp, itemSize);
    break;
  }
}


void getVals(void* arr,int* table, const long base, const size_t len, const size_t itemSize){
  int arrInd=0;
  const int nodeSize=itemSize+sizeof(int);
  node* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node*)(table[i]*nodeSize+base);
      while(temp->index){
	setItem(arr+itemSize*arrInd, temp->val, itemSize);
	temp=(node*)(temp->index*nodeSize+base);
	arrInd++;
      }
      setItem(arr+itemSize*arrInd, temp->val, itemSize);
      arrInd++;
    }
  }
}

void prepSort(void* arr, const size_t len, const size_t itemSize, int ordered,
	      const void* max,const void* min,
	      unsigned long (*val_fun)(const void*, const size_t),
	      int (*compar_fun)(const void*, const void*),sort_funs f){
  if(ordered){
  if(itemSize==4){
    return prepSort_int((int*)arr, len, max,min, f);
  }
  if(itemSize==8){
    return prepSort_long((long*)arr, len, max,min, f);
  }
  }
  int* table=calloc(len,sizeof(int));
  const int nodeSize=itemSize+sizeof(int);
  void* nodes=malloc(len*nodeSize);
  const long base=((long)nodes)-nodeSize;
  unsigned int slot;
  node* temp;
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;
  for(int i =0;i<len;i++){
    slot=getSlot(val_fun(getItem(arr, i, itemSize), itemSize)-ulMin, rMax, len);
    temp=(node*)(nodes+i*nodeSize);
    temp->index=table[slot];
    table[slot]=i+1;
    setItem(temp->val, getItem(arr, i, itemSize), itemSize);
  }
  getVals(arr, table, base, len, itemSize);
  f.sort_fun_0(arr, len, itemSize, compar_fun);
  free(table);
  free(nodes);
}



void getVals_int(int* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_int* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_int*)(table[i]*sizeof(node_int)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_int*)(temp->index*sizeof(node_int)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_int(int* arr, const size_t len, const void* max,const void* min, sort_funs f){
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_int));
  const long base=((long)nodes)-sizeof(node_int);
  unsigned int slot;
  node_int* temp;
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_int*)(nodes+i*sizeof(node_int));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_int(arr, table, base, len);
  f.sort_fun_1(arr, len);
  free(table);
  free(nodes);
}



void getVals_long(long* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_long* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_long*)(table[i]*sizeof(node_long)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_long*)(temp->index*sizeof(node_long)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_long(long* arr, const size_t len, const void* max,const void* min, sort_funs f){
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_long));
  const long base=((long)nodes)-sizeof(node_long);
  unsigned int slot;
  node_long* temp;
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_long*)(nodes+i*sizeof(node_long));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_long(arr, table, base, len);
  f.sort_fun_2(arr, len);
  free(table);
  free(nodes);
}

