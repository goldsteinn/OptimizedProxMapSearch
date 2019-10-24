#include "prepsort.h"


#define getItem(x, y, z) (x+(y*z))
 
typedef struct node{
  int index;
  char val[];
}node;
    
typedef struct node_long{
  int index;
  long val;
}node_long;

typedef struct node_int{
  int index;
  int val;
}node_int;

typedef struct node_short{
  int index;
  short val;
}node_short;

typedef struct node_char{
  int index;
  char val;
}node_char;


void prepSort_char(char* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_short(short* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_int(int* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_long(long* arr, const size_t len, const void* max,const void* min, sort_funs f);


inline int getSlot(unsigned long val, const double max, const size_t len){
  return (int)((val/max)*len);
}


inline void setItem(void* arr, void* temp, const size_t itemSize){
  int i;
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
    switch(itemSize){
    case 8:
      return prepSort_long((long*)arr, len, max,min, f);
    case 4:
      return prepSort_int((int*)arr, len, max,min, f);
    case 2:
      return prepSort_short((short*)arr, len, max,min, f);
    case 1:
      return prepSort_char((char*)arr, len, max,min, f);
    }
  }


  const unsigned long ulMin=(*(unsigned long*)min);
  const unsigned long dif=(*(unsigned long*)max)-ulMin;
  const size_t effectiveLen=MIN(dif, len);
  int* table=calloc(effectiveLen,sizeof(int));
  const int nodeSize=itemSize+sizeof(int);
  void* nodes=malloc(len*nodeSize);
  const long base=((long)nodes)-nodeSize;
  unsigned int slot;
  node* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=getSlot(val_fun(getItem(arr, i, itemSize), itemSize)-ulMin, rMax, effectiveLen);
    temp=(node*)(nodes+i*nodeSize);
    temp->index=table[slot];
    table[slot]=i+1;
    setItem(temp->val, getItem(arr, i, itemSize), itemSize);
  }
  getVals(arr, table, base, effectiveLen, itemSize);
  f.sort_fun_generic(arr, len, itemSize, compar_fun);
  free(table);
  free(nodes);
}



inline void sortAsAdd(long* arr, int ind, long val){
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



void getVals_long(long* arr,int* table, const long base, const size_t len){
  int arrInd=0, lstart;
  node_long* temp=NULL;

  for(int i=0;i<len;i++){
    if(table[i]){
      lstart=arrInd;
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
  const unsigned long ulMin=(*(unsigned long*)min);
  unsigned long dif=(*(unsigned long*)max)-ulMin;
  if(dif<=len){
    dif++;
    //assumed < 2^31 items always
    int* buckets=calloc(dif, sizeof(int));
    for(int i=0;i<len;i++){
      buckets[arr[i]]++;
    }
    int arrIndex=0;
    for(int i=0;i<dif;i++){
      for(int j=0;j<buckets[i];j++){
	arr[arrIndex]=i;
	arrIndex++;
      }
    }
    free(buckets);
    return;
  }
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_long));
  const long base=((long)nodes)-sizeof(node_long);
  unsigned int slot;
  node_long* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_long*)(nodes+i*sizeof(node_long));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_long(arr, table, base, len);
  f.sort_fun_long(arr, len);
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

  const unsigned long ulMin=(*(unsigned long*)min);
  unsigned long dif=(*(unsigned long*)max)-ulMin;
  if(dif<=len){
    dif++;
    //assumed < 2^31 items always
    int* buckets=calloc(dif, sizeof(int));
    for(int i=0;i<len;i++){
      buckets[arr[i]]++;
    }
    int arrIndex=0;
    for(int i=0;i<dif;i++){
      for(int j=0;j<buckets[i];j++){
	arr[arrIndex]=i;
	arrIndex++;
      }
    }
    free(buckets);
    return;
  }
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_int));
  const long base=((long)nodes)-sizeof(node_int);
  unsigned int slot;
  node_int* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_int*)(nodes+i*sizeof(node_int));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_int(arr, table, base, len);
  f.sort_fun_int(arr, len);
  free(table);
  free(nodes);
}


void getVals_short(short* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_short* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_short*)(table[i]*sizeof(node_short)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_short*)(temp->index*sizeof(node_short)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_short(short* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  unsigned long dif=(*(unsigned long*)max)-ulMin;
  if(dif<=len){
    dif++;
    int* buckets=calloc(dif, sizeof(int));
    for(int i=0;i<len;i++){
      buckets[arr[i]]++;
    }
    int arrIndex=0;
    for(int i=0;i<dif;i++){
      for(int j=0;j<buckets[i];j++){
	arr[arrIndex]=i;
	arrIndex++;
      }
    }
    free(buckets);
    return;
  }
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_short));
  const long base=((long)nodes)-sizeof(node_short);
  unsigned int slot;
  node_short* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_short*)(nodes+i*sizeof(node_short));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_short(arr, table, base, len);
  f.sort_fun_short(arr, len);
  free(table);
  free(nodes);
}


void getVals_char(char* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_char* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_char*)(table[i]*sizeof(node_char)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_char*)(temp->index*sizeof(node_char)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_char(char* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  unsigned long dif=(*(unsigned long*)max)-ulMin;
  if(dif<=len){
    dif++;
    int* buckets=calloc(dif, sizeof(int));
    for(int i=0;i<len;i++){
      buckets[arr[i]]++;
    }
    int arrIndex=0;
    for(int i=0;i<dif;i++){
      for(int j=0;j<buckets[i];j++){
	arr[arrIndex]=i;
	arrIndex++;
      }
    }
    free(buckets);
    return;
  }
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_char));
  const long base=((long)nodes)-sizeof(node_char);
  unsigned int slot;
  node_char* temp;

  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_char*)(nodes+i*sizeof(node_char));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_char(arr, table, base, len);
  f.sort_fun_char(arr, len);
  free(table);
  free(nodes);
}

