#include "prepsort.h"
#define getItem(x, y, z) (x+(y*z))
 
typedef struct node{
  int index;
  char val[];
}node;
    

inline int getSlot(unsigned long val, const double max, const int len){
  return (int)((val/max)*len);
}

inline void setItem(void* arr, void* temp, const int itemSize){
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


void getVals(void* arr,int* table, const long base, const int len, const int itemSize){
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

void prepSort(void* arr, const int len, const int itemSize, const void* max,const void* min,
	      unsigned long (*val_fun)(const void*, const int),
	      int (*compar_fun)(const void*, const void*),
	      void (*sort_fun)(void*, const int, const int,
			       int (*compar_fun)(const void*, const void*))){
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
  sort_fun(arr, len, itemSize, compar_fun);
  free(table);
  free(nodes);
}

