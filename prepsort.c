#include "utils/arg.h"
#include "prepsort.h"


void sorter_bucket_sort(stype* arr, const int alen, int buckets[doBucketSort], const int offset){

}
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
void proxyMapSort_start(stype* arr, const int alen){
  todos* que=NULL;
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
    slot=getSlot(val, max, stypeMax);
    addNode(table+slot, tnodes+i, val, i);
  }

  //get tables and handle cases
  int astart=alen-1, size, index;
  for(i--;i>=0;){
    size=table[i].size;
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
      else if(table[i].max-table[i].min<doBucketSort){

	index=table[i].index;
	for(int j=astart;j<astart+size;j++){
	  arr[j]=tnodes[index].val;
	  index=tnodes[index].index;
	}
	//bucket sort that works on floats (will need to isolate the bits of importance
	//and cast them to int
	sorter_bucket_sort(arr+astart, size, myBuckets, table[i].min); 
      }
      else{
	todos* newTodo=(todos*)calloc(1, sizeof(todos));
	newTodo->index=i;
	newTodo->astart=astart;
	newTodo->next=que;
	que=newTodo;
      }
    }

  }
  
}

int main(){
  

}
