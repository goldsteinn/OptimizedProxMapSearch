#include "prepsort.h"

#define getItem(x, y, z) (x+(y*z))


typedef union f_ui{
  float f;
  unsigned int ui;
}f_ui;

typedef struct node{
  int index;
  char val[];
}node;
    
typedef struct node_8{
  int index;
  long val;
}node_8;

typedef struct node_4{
  int index;
  int val;
}node_4;


typedef struct node_2{
  int index;
  short val;
}node_2;

typedef struct node_1{
  int index;
  char val;
}node_1;


void prepSort_1(char* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_2(short* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_4(int* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_8(long* arr, const size_t len, const void* max,const void* min, sort_funs f);


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
      return prepSort_8((long*)arr, len, max,min, f);
    case 4:
      return prepSort_4((int*)arr, len, max,min, f);
    case 2:
      return prepSort_2((short*)arr, len, max,min, f);
    case 1:
      return prepSort_1((char*)arr, len, max,min, f);
    }
  }


  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;
  const size_t effectiveLen=MIN(((*(unsigned long*)max)-ulMin), len);
  int* table=calloc(effectiveLen,sizeof(int));
  const int nodeSize=itemSize+sizeof(int);
  void* nodes=malloc(len*nodeSize);
  const long base=((long)nodes)-nodeSize;
  unsigned int slot;
  node* temp;
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



void getVals_8(long* arr,int* table, const long base, const size_t len){
  int arrInd=0, lstart;
  node_8* temp=NULL;

  for(int i=0;i<len;i++){
    if(table[i]){
      lstart=arrInd;
      temp=(node_8*)(table[i]*sizeof(node_8)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_8*)(temp->index*sizeof(node_8)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}


void prepSort_8(long* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;

  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_8));
  const long base=((long)nodes)-sizeof(node_8);
  unsigned int slot0, slot1, slot2, slot3;
  node_8* temp;
  int i;
  slot0=(arr[0]/rMax)*len;
  __builtin_prefetch(&table[slot0]);
  slot1=(arr[1]/rMax)*len;
  slot2=(arr[i+2]/rMax)*len;

  
  for(i=0;i<len-3;i++){
    slot3=(arr[i+3]/rMax)*len;

    __builtin_prefetch(&table[slot2]);
    __builtin_prefetch(&table[slot3]);

  
    temp=(node_8*)(nodes+i*sizeof(node_8));
    temp->index=table[slot0];
    table[slot0]=i+1;
    temp->val=arr[i];
    slot0=slot1;
    slot1=slot2;
    slot2=slot3;
    }

  __builtin_prefetch(&table[slot1]);
  __builtin_prefetch(&table[slot2]);

  temp=(node_8*)(nodes+i*sizeof(node_8));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  slot0=slot1;
  slot1=slot2;
  __builtin_prefetch(&table[slot1]);
  i++;
  temp=(node_8*)(nodes+i*sizeof(node_8));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];


  slot0=slot1;
  i++;
  temp=(node_8*)(nodes+i*sizeof(node_8));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  
  getVals_8(arr, table, base, len);
  f.sort_fun_long(arr, len);
  free(table);
  free(nodes);
}


void getVals_4(int* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_4* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_4*)(table[i]*sizeof(node_4)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_4*)(temp->index*sizeof(node_4)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}


void prepSort_4(int* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;

  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_4));
  const long base=((long)nodes)-sizeof(node_4);
  unsigned int slot0, slot1, slot2, slot3;
  node_4* temp;
  int i;
  
  slot0=(arr[0]/rMax)*len;
  __builtin_prefetch(&table[slot0]);
  slot1=(arr[1]/rMax)*len;
  slot2=(arr[i+2]/rMax)*len;

  
  for(i=0;i<len-3;i++){
    slot3=(arr[i+3]/rMax)*len;

    __builtin_prefetch(&table[slot2]);
    __builtin_prefetch(&table[slot3]);

  
    temp=(node_4*)(nodes+i*sizeof(node_4));
    temp->index=table[slot0];
    table[slot0]=i+1;
    temp->val=arr[i];
    slot0=slot1;
    slot1=slot2;
    slot2=slot3;
    }

  __builtin_prefetch(&table[slot1]);
  __builtin_prefetch(&table[slot2]);

  temp=(node_4*)(nodes+i*sizeof(node_4));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  slot0=slot1;
  slot1=slot2;
  __builtin_prefetch(&table[slot1]);
  i++;
  temp=(node_4*)(nodes+i*sizeof(node_4));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];


  slot0=slot1;
  i++;
  temp=(node_4*)(nodes+i*sizeof(node_4));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  getVals_4(arr, table, base, len);
  f.sort_fun_int(arr, len);
  free(table);
  free(nodes);
}


void getVals_2(short* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_2* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_2*)(table[i]*sizeof(node_2)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_2*)(temp->index*sizeof(node_2)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_2(short* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;

  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_2));
  const long base=((long)nodes)-sizeof(node_2);
  unsigned int slot0, slot1, slot2, slot3;
  node_2* temp;
  int i;
  
  slot0=(arr[0]/rMax)*len;
  __builtin_prefetch(&table[slot0]);
  slot1=(arr[1]/rMax)*len;
  slot2=(arr[i+2]/rMax)*len;

  
  for(i=0;i<len-3;i++){
    slot3=(arr[i+3]/rMax)*len;

    __builtin_prefetch(&table[slot2]);
    __builtin_prefetch(&table[slot3]);

  
    temp=(node_2*)(nodes+i*sizeof(node_2));
    temp->index=table[slot0];
    table[slot0]=i+1;
    temp->val=arr[i];
    slot0=slot1;
    slot1=slot2;
    slot2=slot3;
    }

  __builtin_prefetch(&table[slot1]);
  __builtin_prefetch(&table[slot2]);

  temp=(node_2*)(nodes+i*sizeof(node_2));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  slot0=slot1;
  slot1=slot2;
  __builtin_prefetch(&table[slot1]);
  i++;
  temp=(node_2*)(nodes+i*sizeof(node_2));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];


  slot0=slot1;
  i++;
  temp=(node_2*)(nodes+i*sizeof(node_2));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  getVals_2(arr, table, base, len);
  f.sort_fun_short(arr, len);
  free(table);
  free(nodes);
}


void getVals_1(char* arr,int* table, const long base, const size_t len){
  int arrInd=0;
  node_1* temp=NULL;
  for(int i=0;i<len;i++){
    if(table[i]){
      temp=(node_1*)(table[i]*sizeof(node_1)+base);
      while(temp->index){
	arr[arrInd]=temp->val;
	temp=(node_1*)(temp->index*sizeof(node_1)+base);
	arrInd++;
      }
      arr[arrInd]=temp->val;
      arrInd++;
    }
  }
}
void prepSort_1(char* arr, const size_t len, const void* max,const void* min, sort_funs f){
  const unsigned long ulMin=(*(unsigned long*)min);
  const double rMax=((*(unsigned long*)max)-ulMin)+1.1;

  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_1));
  const long base=((long)nodes)-sizeof(node_1);
  unsigned int slot0, slot1, slot2, slot3;
  node_1* temp;
  int i;
  
  slot0=(arr[0]/rMax)*len;
  __builtin_prefetch(&table[slot0]);
  slot1=(arr[1]/rMax)*len;
  slot2=(arr[i+2]/rMax)*len;

  
  for(i=0;i<len-3;i++){
    slot3=(arr[i+3]/rMax)*len;

    __builtin_prefetch(&table[slot2]);
    __builtin_prefetch(&table[slot3]);

  
    temp=(node_1*)(nodes+i*sizeof(node_1));
    temp->index=table[slot0];
    table[slot0]=i+1;
    temp->val=arr[i];
    slot0=slot1;
    slot1=slot2;
    slot2=slot3;
    }

  __builtin_prefetch(&table[slot1]);
  __builtin_prefetch(&table[slot2]);

  temp=(node_1*)(nodes+i*sizeof(node_1));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  slot0=slot1;
  slot1=slot2;
  __builtin_prefetch(&table[slot1]);
  i++;
  temp=(node_1*)(nodes+i*sizeof(node_1));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];


  slot0=slot1;
  i++;
  temp=(node_1*)(nodes+i*sizeof(node_1));
  temp->index=table[slot0];
  table[slot0]=i+1;
  temp->val=arr[i];

  getVals_1(arr, table, base, len);
  f.sort_fun_char(arr, len);
  free(table);
  free(nodes);
}

