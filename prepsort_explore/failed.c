/*void prepSort2(void* arr, const int len, const int itemSize, const void* max,
	       unsigned int (*rangePlace)(const void* val, const int len, const void* max),
	       int (*compar)(const void* a, const void* b)){
  int* table=calloc(len,sizeof(int));
  void* val=calloc(1,itemSize), *temp=calloc(1, itemSize);
  setItem(val, arr, itemSize);
  int offset;
  unsigned int slot;
  for(int i =0;i<len;i++){
    table[rangePlace(getItem(arr, i, itemSize), len, max)]++;
  }
  int myLen=len;
  for(int i=len-1;i>=0;i--){
    myLen-=table[i];
    table[i]=myLen;
  }
  for(int i =1;i<len;i++){
    slot=rangePlace(val, len, max);
    offset=++table[slot];
    setItem(temp, arr+(offset)*itemSize, itemSize);
    setItem(arr+(offset)*itemSize, val, itemSize);
    val=temp;
  }
  }*/
