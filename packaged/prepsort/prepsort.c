#include "prepsort.h"
//#define floatOp

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


typedef struct node_4_f{
  int index;
  float val;
}node_4_f;


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
void prepSort_4_f(float* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_4_i(int* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_8_l(long* arr, const size_t len, const void* max,const void* min, sort_funs f);
void prepSort_8_f(double* arr, const size_t len, const void* max,const void* min, sort_funs f);


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
#ifdef floatOp
    case 8:
      return prepSort_8_f((double*)arr, len, max,min, f);
    case 4:
      return prepSort_4_f((float*)arr, len, max,min, f);
#else
    case 8:

      return prepSort_8_l((long*)arr, len, max,min, f);

    case 4:
      return prepSort_4_i((int*)arr, len, max,min, f);
#endif
    case 2:
      return prepSort_2((short*)arr, len, max,min, f);
    case 1:
      return prepSort_1((char*)arr, len, max,min, f);
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


void prepSort_8_l(long* arr, const size_t len, const void* max,const void* min, sort_funs f){
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
  void* nodes=malloc(len*sizeof(node_8));
  const long base=((long)nodes)-sizeof(node_8);
  unsigned int slot;
  node_8* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_8*)(nodes+i*sizeof(node_8));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_8(arr, table, base, len);
  f.sort_fun_long(arr, len);
  free(table);
  free(nodes);
}

void prepSort_8_f(double* arr, const size_t len, const void* max,const void* min, sort_funs f){
  unsigned long fixMax =(*(unsigned long*)max);
  if(fixMax >> 54 == 0x7ff){
    fixMax ^= ((1UL)<<53);
  }
  const double dif=((*(double*)(&fixMax))-(*(double*)min))+1.1;
  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_8));
  const long base=((long)nodes)-sizeof(node_8);
  unsigned int slot;
  unsigned long val;
  node_8* temp;
  for(int i =0;i<len;i++){
    val = arr[i];
    slot=(val/dif)*len;
    temp=(node_8*)(nodes+i*sizeof(node_8));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=val;
  }
  getVals_8((long*)arr, table, base, len);
  f.sort_fun_long((long*)arr, len);
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

void prepSort_4_f(float* arr, const size_t len, const void* max,const void* min, sort_funs f){
    unsigned int fixMax =(*(unsigned int*)max);
    if(fixMax >> 24 == 0x7f){
      fixMax ^= ((1)<<23);
    }
    const double dif=((*(float*)(&fixMax))-(*((float*)min)))+1.1;
    int* table=calloc(len,sizeof(int));
    void* nodes=malloc(len*sizeof(node_4_f));
    const long base=((long)nodes)-sizeof(node_4_f);
    f_ui slot;
    node_4_f* temp;
    float val;
    const float flen = len;
    for(int i =0;i<len;i++){
      val = arr[i];
      slot.ui = (val/dif)*flen;
      temp=(node_4_f*)(nodes+i*sizeof(node_4_f));
      temp->index=table[slot.ui];
      table[slot.ui]=i+1;
      temp->val=val;
  }
  getVals_4((int*)arr, table, base, len);
  f.sort_fun_int((int*)arr, len);
  free(table);
  free(nodes);
}
void prepSort_4_i(int* arr, const size_t len, const void* max,const void* min, sort_funs f){
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
  /* Use if len smaller
  int down_shift= __builtin_clz(len)+1;
  slot = arr[i]>>down_shift;
  */

  int* table=calloc(len,sizeof(int));
  void* nodes=malloc(len*sizeof(node_4));
  const long base=((long)nodes)-sizeof(node_4);
  unsigned int slot;
  node_4* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_4*)(nodes+i*sizeof(node_4));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
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
  void* nodes=malloc(len*sizeof(node_2));
  const long base=((long)nodes)-sizeof(node_2);
  unsigned int slot;
  node_2* temp;
  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_2*)(nodes+i*sizeof(node_2));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
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
  void* nodes=malloc(len*sizeof(node_1));
  const long base=((long)nodes)-sizeof(node_1);
  unsigned int slot;
  node_1* temp;

  const double rMax=dif+1.1;
  for(int i =0;i<len;i++){
    slot=(arr[i]/rMax)*len;
    temp=(node_1*)(nodes+i*sizeof(node_1));
    temp->index=table[slot];
    table[slot]=i+1;
    temp->val=arr[i];
  }
  getVals_1(arr, table, base, len);
  f.sort_fun_char(arr, len);
  free(table);
  free(nodes);
}

