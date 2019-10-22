#include "utils/arg.h"
#include "prepsort.h"


int getSlot(stype val, const double rmax, const int bound){
  return (int)((val/rmax)*bound);
}


void addNode(bucket_head* table, node* tnode, const stype val, const int nindex){

  table->size++;
  table->max=MAX(table->max, val);
  table->min=MIN(table->min, val);
  tnode->index=table->index;
  table->index=nindex;

}
void proxMapSort_recursive(stype* arr, const int alen, int tsize,  bucket_head* table, node* tnodes, double bound, int indexStart){
    int slot, i, index=indexStart, temp;
    stype val;
    for(int i =0;i<tsize;i++){
      table[i]=init;
    }
    for(i =0;i<alen;i++){
      val=tnodes[index].val;
      slot=getSlot(val, bound, tsize);
      if(slot>=tsize||slot<0){
	printf("Slot: %d, bound: %lf, tsize: %d, val: %d\n", slot, bound, tsize,val);
      }
      temp=index;
      index=tnodes[temp].index;
      addNode(table+slot, tnodes+temp, val, temp);
      if(val<table[slot].min||val>table[slot].max){
	printf("WE: %d vs %d ::: %d\n", table[slot].max, table[slot].min,val);
      }
      //
    }

    proxMapSort_internal(table, tnodes, alen, arr);
}

void proxMapSort_internal(bucket_head* table, node* tnodes, const int alen, stype* arr){
  todos* que=NULL;
  //get tables and handle cases
  int astart=alen, size, index;
  int min=0, max=0, bval=0;
  int i=alen;
  stype val;
  i--;
  for(;i>=0;i--){
    size=table[i].size;
    max=((changer_4)table[i].max).iform;
    min=((changer_4)table[i].min).iform;
    if(size){
      astart-=size;
      if(size<doQuickSort){
	//might be worth it to optimize here, wait until a case
	//where size is above the threshold
	//then timsort or maybe inplace mergesort
	//instead of quicksort here
      index=table[i].index;
      for(int j=astart;j<astart+size;j++){
	arr[j]=tnodes[index].val;
	index=tnodes[index].index;
      }
      sorter_quick_sort(arr+astart, size);
      }
      else if(max-min<doBucketSort){
	index=table[i].index;
	for(int j=0;j<size;j++){
	  bval=((changer_4)(tnodes[index].val)).iform;

	  myBuckets[bval-min]++;
	  index=tnodes[index].index;
	}
	index=astart;
	for(int j=0;j<doBucketSort;j++){
	  for(int k=0;k<myBuckets[j];k++){
	    val=myBuckets[j]+min;
	    arr[index]=((changer_4)val).sform;
	    index++;
	  }
	}
      }
      else{
	if(i+MIN(size, max-min)<alen){
	  proxMapSort_recursive(arr+astart, size, MIN(size, max-min), table+i, tnodes, (double)(max-min)+1.0, table[i].index);
	  //recursive call now
	}
	else{
	todos* newTodo=(todos*)calloc(1, sizeof(todos));
	newTodo->astart=astart;
	newTodo->t=table[i];
	newTodo->next=que;
	que=newTodo;
	}
      }
    }

  }
  while(que!=NULL){
    proxMapSort_recursive(arr+que->astart, size, MIN(que->t.size, que->t.max-que->t.min), table+i, tnodes, (double)(que->t.max-que->t.min)+1.0, que->t.index);
    que=que->next;
  }
}
void proxMapSort_start(stype* arr, const int alen){
  bucket_head* table=(bucket_head*)malloc(alen*sizeof(bucket_head));
  for(int i =0;i<alen;i++){
    table[i]=init;
  }
  node* tnodes=(node*)malloc(alen*sizeof(node));
  int slot, i;
  stype val;
  //setup origional table
  for(i =0;i<alen;i++){
    val=arr[i];
    tnodes[i].val=val;
    slot=getSlot(val, max, alen);
    addNode(table+slot, tnodes+i, val, i);
  }

    proxMapSort_internal(table, tnodes, alen, arr);
}

#define len 335544
int main(){
  srand(time(NULL));
  stype* arr=(stype*)calloc(len, sizeof(stype));
  for(int i =0;i<len;i++){
          arr[i]=rand()%500;
    //       arr[i]=(i%50)*(RAND_MAX/50);
    //    arr[i]=rand();
  }
  proxMapSort_start(arr, len);
  for(int i =1;i<len;i++){
    if(arr[i]<arr[i-1]){
            printf("Error at %d\n", i);
    }
  }
}
