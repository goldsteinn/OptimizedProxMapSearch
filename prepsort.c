#include "utils/arg.h"
#include "prepsort.h"
#include "timsort/timsort.h"


//the basic idea is to bucket sort the array/hash it just to the ratio of the value of element
//to tMax times the size of the table. This doesn't sort the data but it does order it somewhat
//which some sorting algorithms like (this was origionally thought of with timsort in mind but
//when I was comparing its performance to other sorting algorithms I saw mergesort also performed
//better). The way the table slot is computed (found in addNode) is simply (val/tMax)*tableSize
//and each slot in the table has a singly linked list. After all the values from the array have
//been added to the table, all the values will be copied back to the array now in an order
//more condusive to some sort functions. What I am currently exploring is independently sorting
//each slot in the table as it's known that any item in slot n is less than any item in slot n+1



//arguments
int randSeed=0;
unsigned int runSize=0;
unsigned int runOffset=0;

#define Version "0.1"
static ArgOption args[] = {
  // Kind, 	  Method,		name,	    reqd,  variable,		help
  { KindHelp,     Help, 	"-h" },
  { KindOption,   Integer,      "--runsize",           0,  &runSize,           "Sets size of generated runs" },
  { KindOption,   Integer,      "--runoffset",         0,  &runOffset,         "Sets size between the elements in a given run" },
  { KindOption,   Integer,      "--randseed",          0,  &randSeed,          "Sets a seed for srand()" },
  { KindEnd }
};
static ArgDefs argp = { args, "A bunch of sorting", Version, NULL };



int testSortAsGet(
#ifdef raTablePtr
	     int*
#else
	     node**
#endif
	     table, node* nodes, stype* arr){
#ifdef raPtr
  long base=(long)nodes;
#ifdef preBase
  long indBase=0;
#endif
#endif
  for(unsigned int i =0;i<len;i++){
    nodes[i].val=arr[i];
    addNode(nodes+i, table
#ifdef raPtr
	    ,base
#ifdef preBase
	    ,indBase
#endif
#endif
	    );
#ifdef raPtr
#ifdef preBase
    indBase+=sizeof(node);
#endif
#endif
  }
  getValsSortAsAdd(table, arr
#ifdef raPtr
		   ,base
#endif
		   );
}


int testSortAsAdd(
#ifdef raTablePtr
	       int*
#else
	       node**
#endif
	       table, node* nodes, stype* arr){
#ifdef raPtr
  long base=(long)nodes;
#endif
  for(unsigned int i =0;i<len;i++){
    nodes[i].val=arr[i];
    addNodeSort(nodes+i, table
#ifdef raPtr
		,base
#endif
		);
  }
  getVals(table, arr
#ifdef raPtr
	  ,base
#endif
	  );
}




int testGetWithSort(
#ifdef raTablePtr
	      int*
#else
	      node**
#endif
	      table, node* nodes, stype* arr,void (*sort_function)(stype*, const size_t)){
#ifdef raPtr
  long base=(long)nodes;
#ifdef preBase
  long indBase=0;
#endif
#endif
  for(unsigned int i =0;i<len;i++){
    nodes[i].val=arr[i];
    addNode(nodes+i, table
#ifdef raPtr
	    ,base
#ifdef preBase
	    ,indBase
#endif
#endif
	    );
#ifdef raPtr
#ifdef preBase
    indBase+=sizeof(node);
#endif
#endif
  }
  getValsSort(table, arr,
#ifdef raPtr
	      base,
#endif
	      sort_function);
}




int testSort2AfterGet(
#ifdef raTablePtr
	      int*
#else
	      node**
#endif
	      table, node* nodes, stype* arr, void (*sort_function)(stype*, const size_t)){
#ifdef raPtr
  long base=(long)nodes;
#ifdef preBase
  long indBase=0;
#endif
#endif
  for(unsigned int i =0;i<len;i++){

    nodes[i].val=arr[i];
    addNode(nodes+i, table
#ifdef raPtr
	    ,base
#ifdef preBase
	    ,indBase
#endif
#endif
	    );
#ifdef raPtr
#ifdef preBase
    indBase+=sizeof(node);
#endif
#endif
  }
  getVals(table, arr
#ifdef raPtr
	  ,base
#endif
	  );
  sort_function(arr, len);
}


int testSort4AfterGet(
#ifdef raTablePtr
	      int*
#else
	      node**
#endif
	      table, node* nodes, stype* arr, void (*sort_function)(void*, const size_t, const size_t, int (*cmp_fun)(const void*, const void*))){
#ifdef raPtr
  long base=(long)nodes;
#ifdef preBase
  long indBase=0;
#endif
#endif
  for(unsigned int i =0;i<len;i++){

    nodes[i].val=arr[i];
    addNode(nodes+i, table
#ifdef raPtr
	    ,base
#ifdef preBase
	    ,indBase
#endif
#endif
	    );
#ifdef raPtr
#ifdef preBase
    indBase+=sizeof(node);
#endif
#endif
  }
  getVals(table, arr
#ifdef raPtr
	  ,base
#endif
	  );
  sort_function(arr, len, sizeof(stype), intcomp);
}



//creates a new random array
stype* createArrRand(stype* randHolder){
  stype* arr=malloc(len*sizeof(stype));
  for(unsigned int i =0;i<len;i++){
    arr[i]=randHolder[i];
  }
  return arr;
}

//creates an array with runs
stype* createArrRuns(stype* randHolder, unsigned int runSize, unsigned int runOffset){
  stype* arr=malloc(len*sizeof(stype));
  for(unsigned int i =0;i<len;i++){
    arr[i]=randHolder[i];
    int bound=(int)(randHolder[i])%runSize;
    for(int j=i+1;j<(bound+i) && j<len;j++){
      if(((stype)RAND_MAX)<arr[i]+(j-i)*(runOffset)){
	bound=j;
	break;
      }
      arr[j]=arr[i]+(j-i)*(runOffset);
      
    }
    i+=bound-1+(bound==0);
  }
  return arr;
}

 

int main(int argc, char** argv){
  ArgParser* ap = createArgumentParser(&argp);
  if(parseArguments(ap, argc, argv)){
    printf("Error parsing arguments!\n");
    return -1;
  }
  srand(randSeed);
  struct timeval t[2];

#ifdef superVer
  stype* verArr=malloc(len*sizeof(stype));
#endif
  stype* randHolder=malloc(len*sizeof(stype));
  for(unsigned int i =0;i<len;i++){
    randHolder[i]=rand();
  }

#ifdef TS1
  stype* arrTS1= createArrRand(randHolder);
#ifdef superVer
  for(int i =0;i<len;i++){
    verArr[i]=arrTS1[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  timsort(arrTS1, len, sizeof(stype), intcomp);  
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrTS1);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(verArr[i]!=arrTS1[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrTS1);
  doPrint(TS1,getDiff(t[0], t[1]));
#endif

#ifdef PPTS1
#ifdef raTablePtr
  int* tablePPTS1=malloc(tsize*sizeof(int));
  memset(tablePPTS1,-1,tsize*sizeof(int));
#else
  node** tablePPTS1=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPTS1=calloc(len, sizeof(node));
  stype* arrPPTS1= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPTS1[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort4AfterGet(tablePPTS1, nodesPPTS1, arrPPTS1, timsort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPTS1);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(arrPPTS1[i]!=verArr[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPTS1);
  free(nodesPPTS1);
  free(arrPPTS1);
  doPrint(PPTS1,getDiff(t[0],t[1]));
#endif
#ifdef GRTS1
  stype* arrGRTS1= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i =0;i<len;i++){
    verArr[i]=arrGRTS1[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  timsort(arrGRTS1, len, sizeof(stype), intcomp);  
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRTS1);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(verArr[i]!=arrGRTS1[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRTS1);
  doPrint(GRTS1,getDiff(t[0],t[1]));
#endif
#ifdef PPGRTS1
#ifdef raTablePtr
  int* tablePPGRTS1=malloc(tsize* sizeof(int));
  memset(tablePPGRTS1,-1,tsize*sizeof(int));
#else
  node** tablePPGRTS1=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPGRTS1=calloc(len, sizeof(node));
  stype* arrPPGRTS1= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRTS1[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort4AfterGet(tablePPGRTS1, nodesPPGRTS1, arrPPGRTS1, timsort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRTS1);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRTS1[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRTS1);
  free(nodesPPGRTS1);
  free(arrPPGRTS1);
  doPrint(PPGRTS1,getDiff(t[0],t[1]));
#endif
#ifdef TS2
  stype* arrTS2= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrTS2[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_tim_sort(arrTS2, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrTS2);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrTS2[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrTS2);
  doPrint(TS2,getDiff(t[0],t[1]));
#endif

  
#ifdef PPTS2
#ifdef raTablePtr
  int* tablePPTS2=malloc(tsize* sizeof(int));
  memset(tablePPTS2,-1,tsize*sizeof(int));
#else
  node** tablePPTS2=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPTS2=calloc(len, sizeof(node));
  stype* arrPPTS2= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPTS2[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPTS2, nodesPPTS2, arrPPTS2, sorter_tim_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPTS2);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPTS2[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPTS2);
  free(nodesPPTS2);
  free(arrPPTS2);
  doPrint(PPTS2,getDiff(t[0],t[1]));
#endif
#ifdef GRTS2
  stype* arrGRTS2= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRTS2[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_tim_sort(arrGRTS2, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRTS2);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRTS2[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRTS2);
  doPrint(GRTS2,getDiff(t[0],t[1]));
#endif
#ifdef PPGRTS2
#ifdef raTablePtr
  int* tablePPGRTS2=malloc(tsize* sizeof(int));
  memset(tablePPGRTS2,-1,tsize*sizeof(int));
#else
  node** tablePPGRTS2=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPGRTS2=calloc(len, sizeof(node));
  stype* arrPPGRTS2= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRTS2[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRTS2, nodesPPGRTS2, arrPPGRTS2, sorter_tim_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRTS2);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRTS2[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRTS2);
  free(nodesPPGRTS2);
  free(arrPPGRTS2);
  doPrint(PPGRTS2,getDiff(t[0],t[1]));
#endif
#ifdef MQS
  stype* arrMQS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrMQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  qsort(arrMQS, len, sizeof(stype), intcomp);  
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrMQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrMQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrMQS);
  doPrint(MQS,getDiff(t[0],t[1]));
#endif

#ifdef PPMQS
#ifdef raTablePtr
  int* tablePPMQS=malloc(tsize* sizeof(int));
  memset(tablePPMQS,-1,tsize*sizeof(int));
#else
  node** tablePPMQS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPMQS=calloc(len, sizeof(node));
  stype* arrPPMQS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPMQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort4AfterGet(tablePPMQS, nodesPPMQS, arrPPMQS, qsort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPMQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPMQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPMQS);
  free(nodesPPMQS);
  free(arrPPMQS);
  doPrint(PPMQS,getDiff(t[0],t[1]));
#endif
#ifdef GRMQS
  stype* arrGRMQS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRMQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  qsort(arrGRMQS, len, sizeof(stype), intcomp);  
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRMQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRMQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRMQS);
  doPrint(GRMQS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRMQS
#ifdef raTablePtr
  int* tablePPGRMQS=malloc(tsize* sizeof(int));
  memset(tablePPGRMQS,-1,tsize*sizeof(int));
#else
  node** tablePPGRMQS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPGRMQS=calloc(len, sizeof(node));
  stype* arrPPGRMQS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRMQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort4AfterGet(tablePPGRMQS, nodesPPGRMQS, arrPPGRMQS, qsort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRMQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRMQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRMQS);
  free(nodesPPGRMQS);
  free(arrPPGRMQS);
  doPrint(PPGRMQS,getDiff(t[0],t[1]));
#endif
#ifdef CQS
  stype* arrCQS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrCQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_quick_sort(arrCQS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrCQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrCQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrCQS);
  doPrint(CQS,getDiff(t[0],t[1]));
#endif

#ifdef PPCQS
#ifdef raTablePtr
  int* tablePPCQS=malloc(tsize* sizeof(int));
  memset(tablePPCQS,-1,tsize*sizeof(int));
#else
  node** tablePPCQS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPCQS=calloc(len, sizeof(node));
  stype* arrPPCQS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPCQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPCQS, nodesPPCQS, arrPPCQS, sorter_quick_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPCQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPCQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPCQS);
  free(nodesPPCQS);
  free(arrPPCQS);
  doPrint(PPCQS,getDiff(t[0],t[1]));
#endif
#ifdef GRCQS
  stype* arrGRCQS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRCQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  qsort(arrGRCQS, len, sizeof(stype), intcomp);  
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRCQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRCQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRCQS);
  doPrint(GRCQS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRCQS
#ifdef raTablePtr
  int* tablePPGRCQS=malloc(tsize* sizeof(int));
  memset(tablePPGRCQS,-1,tsize*sizeof(int));
#else
  node** tablePPGRCQS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPGRCQS=calloc(len, sizeof(node));
  stype* arrPPGRCQS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRCQS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRCQS, nodesPPGRCQS, arrPPGRCQS, sorter_quick_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRCQS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRCQS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRCQS);
  free(nodesPPGRCQS);
  free(arrPPGRCQS);
  doPrint(PPGRCQS,getDiff(t[0],t[1]));
#endif
#ifdef NMS
  stype* arrNMS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrNMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_merge_sort(arrNMS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrNMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrNMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrNMS);
  doPrint(NMS,getDiff(t[0],t[1]));
#endif

#ifdef PPNMS
#ifdef raTablePtr
  int* tablePPNMS=malloc(tsize* sizeof(int));
  memset(tablePPNMS,-1,tsize*sizeof(int));
#else
  node** tablePPNMS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPNMS=calloc(len, sizeof(node));
  stype* arrPPNMS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPNMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPNMS, nodesPPNMS, arrPPNMS, sorter_merge_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPNMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPNMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPNMS);
  free(nodesPPNMS);
  free(arrPPNMS);
  doPrint(PPNMS,getDiff(t[0],t[1]));
#endif
#ifdef GRNMS
  stype* arrGRNMS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRNMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_merge_sort(arrGRNMS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRNMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRNMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRNMS);
  doPrint(GRNMS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRNMS
#ifdef raTablePtr
  int* tablePPGRNMS=malloc(tsize* sizeof(int));
  memset(tablePPGRNMS,-1,tsize*sizeof(int));
#else
  node** tablePPGRNMS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPGRNMS=calloc(len, sizeof(node));
  stype* arrPPGRNMS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRNMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRNMS, nodesPPGRNMS, arrPPGRNMS, sorter_merge_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRNMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRNMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRNMS);
  free(nodesPPGRNMS);
  free(arrPPGRNMS);
  doPrint(PPGRNMS,getDiff(t[0],t[1]));
#endif
#ifdef IMS
  stype* arrIMS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrIMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_merge_sort(arrIMS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrIMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrIMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrIMS);
  doPrint(IMS,getDiff(t[0],t[1]));
#endif

#ifdef PPIMS
#ifdef raTablePtr
  int* tablePPIMS=malloc(tsize* sizeof(int));
  memset(tablePPIMS,-1,tsize*sizeof(int));
#else
  node** tablePPIMS=calloc(tsize, sizeof(node*));
#endif
  node* nodesPPIMS=calloc(len, sizeof(node));
  stype* arrPPIMS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPIMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPIMS, nodesPPIMS, arrPPIMS, sorter_merge_sort_in_place);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPIMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPIMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPIMS);
  free(nodesPPIMS);
  free(arrPPIMS);
  doPrint(PPIMS,getDiff(t[0],t[1]));
#endif
#ifdef GRIMS
  stype* arrGRIMS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRIMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_merge_sort(arrGRIMS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRIMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRIMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRIMS);
  doPrint(GRIMS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRIMS
#ifdef raTablePtr
  int* tablePPGRIMS=malloc(tsize* sizeof(int));
  memset(tablePPGRIMS,-1,tsize*sizeof(int));
#else  
  node** tablePPGRIMS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRIMS=calloc(len, sizeof(node));
  stype* arrPPGRIMS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRIMS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRIMS, nodesPPGRIMS, arrPPGRIMS, sorter_merge_sort_in_place);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRIMS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRIMS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRIMS);
  free(nodesPPGRIMS);
  free(arrPPGRIMS);
  doPrint(PPGRIMS,getDiff(t[0],t[1]));
#endif
#ifdef SQ
  stype* arrSQ= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrSQ[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_sqrt_sort(arrSQ, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrSQ);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrSQ[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrSQ);
  doPrint(SQ,getDiff(t[0],t[1]));
#endif

#ifdef PPSQ
#ifdef raTablePtr
  int* tablePPSQ=malloc(tsize* sizeof(int));
  memset(tablePPSQ,-1,tsize*sizeof(int));
#else  
  node** tablePPSQ=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPSQ=calloc(len, sizeof(node));
  stype* arrPPSQ= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPSQ[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPSQ, nodesPPSQ, arrPPSQ, sorter_sqrt_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPSQ);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPSQ[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPSQ);
  free(nodesPPSQ);
  free(arrPPSQ);
  doPrint(PPSQ,getDiff(t[0],t[1]));
#endif
#ifdef GRSQ
  stype* arrGRSQ= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrGRSQ[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_sqrt_sort(arrGRSQ, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRSQ);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrGRSQ[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRSQ);
  doPrint(GRSQ,getDiff(t[0],t[1]));
#endif
#ifdef PPGRSQ
#ifdef raTablePtr
  int* tablePPGRSQ=malloc(tsize* sizeof(int));
  memset(tablePPGRSQ,-1,tsize*sizeof(int));
#else  
  node** tablePPGRSQ=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRSQ=calloc(len, sizeof(node));
  stype* arrPPGRSQ= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRSQ[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRSQ, nodesPPGRSQ, arrPPGRSQ, sorter_sqrt_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRSQ);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRSQ[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRSQ);
  free(nodesPPGRSQ);
  free(arrPPGRSQ);
  doPrint(PPGRSQ,getDiff(t[0],t[1]));
#endif
#ifdef SHS
  stype* arrSHS= createArrRand(randHolder);
#ifdef superVer
  for(int i =0;i<len;i++){
    verArr[i]=arrSHS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_shell_sort(arrSHS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrSHS);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(verArr[i]!=arrSHS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrSHS);
  doPrint(SHS,getDiff(t[0], t[1]));
#endif

#ifdef PPSHS
#ifdef raTablePtr
  int* tablePPSHS=malloc(tsize* sizeof(int));
  memset(tablePPSHS,-1,tsize*sizeof(int));
#else  
  node** tablePPSHS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPSHS=calloc(len, sizeof(node));
  stype* arrPPSHS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPSHS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPSHS, nodesPPSHS, arrPPSHS, sorter_shell_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPSHS);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(arrPPSHS[i]!=verArr[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPSHS);
  free(nodesPPSHS);
  free(arrPPSHS);
  doPrint(PPSHS,getDiff(t[0],t[1]));
#endif
#ifdef GRSHS
  stype* arrGRSHS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i =0;i<len;i++){
    verArr[i]=arrGRSHS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  sorter_shell_sort(arrGRSHS, len);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrGRSHS);
#endif
#ifdef superVer
  for(int i=0;i<len;i++){
    if(verArr[i]!=arrGRSHS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(arrGRSHS);
  doPrint(GRSHS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRSHS
#ifdef raTablePtr
  int* tablePPGRSHS=malloc(tsize* sizeof(int));
  memset(tablePPGRSHS,-1,tsize*sizeof(int));
#else  
  node** tablePPGRSHS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRSHS=calloc(len, sizeof(node));
  stype* arrPPGRSHS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRSHS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSort2AfterGet(tablePPGRSHS, nodesPPGRSHS, arrPPGRSHS, sorter_shell_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRSHS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRSHS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRSHS);
  free(nodesPPGRSHS);
  free(arrPPGRSHS);
  doPrint(PPGRSHS,getDiff(t[0],t[1]));
#endif
#ifdef PPINS
#ifdef raTablePtr
  int* tablePPINS=malloc(tsize*sizeof(int));
  memset(tablePPINS,-1,tsize*sizeof(int));
#else  
  node** tablePPINS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPINS=calloc(len, sizeof(node));
  stype* arrPPINS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPINS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPINS, nodesPPINS, arrPPINS, insertion_sort_ind);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPINS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPINS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPINS);
  free(nodesPPINS);
  free(arrPPINS);
  doPrint(PPINS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRINS
#ifdef raTablePtr
  int* tablePPGRINS=malloc(tsize*sizeof(int));
  memset(tablePPGRINS,-1,tsize*sizeof(int));
#else  
  node** tablePPGRINS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRINS=calloc(len, sizeof(node));
  stype* arrPPGRINS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRINS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRINS, nodesPPGRINS, arrPPGRINS, insertion_sort_ind);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRINS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRINS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRINS);
  free(nodesPPGRINS);
  free(arrPPGRINS);
  doPrint(PPGRINS,getDiff(t[0],t[1]));
#endif
#ifdef PPSA
#ifdef raTablePtr
  int* tablePPSA=malloc(tsize*sizeof(int));
  memset(tablePPSA,-1,tsize*sizeof(int));
#else  
  node** tablePPSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPSA=calloc(len, sizeof(node));
  stype* arrPPSA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSortAsGet(tablePPSA, nodesPPSA, arrPPSA);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPSA);
  free(nodesPPSA);
  free(arrPPSA);
  doPrint(PPSA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRSA
#ifdef raTablePtr
  int* tablePPGRSA=malloc(tsize*sizeof(int));
  memset(tablePPGRSA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRSA=calloc(len, sizeof(node));
  stype* arrPPGRSA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSortAsGet(tablePPGRSA, nodesPPGRSA, arrPPGRSA);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRSA);
  free(nodesPPGRSA);
  free(arrPPGRSA);
  doPrint(PPGRSA,getDiff(t[0],t[1]));
#endif

#ifdef PPANIS
#ifdef raTablePtr
  int* tablePPANIS=malloc(tsize*sizeof(int));
  memset(tablePPANIS,-1,tsize*sizeof(int));
#else  
  node** tablePPANIS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPANIS=calloc(len, sizeof(node));
  stype* arrPPANIS= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPANIS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSortAsAdd(tablePPANIS, nodesPPANIS, arrPPANIS);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPANIS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPANIS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPANIS);
  free(nodesPPANIS);
  free(arrPPANIS);
  doPrint(PPANIS,getDiff(t[0],t[1]));
#endif
#ifdef PPGRANIS
#ifdef raTablePtr
  int* tablePPGRANIS=malloc(tsize*sizeof(int));
  memset(tablePPGRANIS,-1,tsize*sizeof(int));
#else  
  node** tablePPGRANIS=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRANIS=calloc(len, sizeof(node));
  stype* arrPPGRANIS= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRANIS[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testSortAsAdd(tablePPGRANIS, nodesPPGRANIS, arrPPGRANIS);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRANIS);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRANIS[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRANIS);
  free(nodesPPGRANIS);
  free(arrPPGRANIS);
  doPrint(PPGRANIS,getDiff(t[0],t[1]));
#endif
#ifdef PPQSA
#ifdef raTablePtr
  int* tablePPQSA=malloc(tsize*sizeof(int));
  memset(tablePPQSA,-1,tsize*sizeof(int));
#else  
  node** tablePPQSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPQSA=calloc(len, sizeof(node));
  stype* arrPPQSA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPQSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPQSA, nodesPPQSA, arrPPQSA, sorter_quick_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPQSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPQSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPQSA);
  free(nodesPPQSA);
  free(arrPPQSA);
  doPrint(PPQSA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRQSA
#ifdef raTablePtr
  int* tablePPGRQSA=malloc(tsize*sizeof(int));
  memset(tablePPGRQSA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRQSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRQSA=calloc(len, sizeof(node));
  stype* arrPPGRQSA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRQSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRQSA, nodesPPGRQSA, arrPPGRQSA, sorter_quick_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRQSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRQSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRQSA);
  free(nodesPPGRQSA);
  free(arrPPGRQSA);
  doPrint(PPGRQSA,getDiff(t[0],t[1]));
#endif

#ifdef PPMSA
#ifdef raTablePtr
  int* tablePPMSA=malloc(tsize*sizeof(int));
  memset(tablePPMSA,-1,tsize*sizeof(int));
#else  
  node** tablePPMSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPMSA=calloc(len, sizeof(node));
  stype* arrPPMSA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPMSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPMSA, nodesPPMSA, arrPPMSA, sorter_merge_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPMSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPMSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPMSA);
  free(nodesPPMSA);
  free(arrPPMSA);
  doPrint(PPMSA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRMSA
#ifdef raTablePtr
  int* tablePPGRMSA=malloc(tsize*sizeof(int));
  memset(tablePPGRMSA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRMSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRMSA=calloc(len, sizeof(node));
  stype* arrPPGRMSA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRMSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRMSA, nodesPPGRMSA, arrPPGRMSA, sorter_merge_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRMSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRMSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRMSA);
  free(nodesPPGRMSA);
  free(arrPPGRMSA);
  doPrint(PPGRMSA,getDiff(t[0],t[1]));
#endif
#ifdef PPTSA
#ifdef raTablePtr
  int* tablePPTSA=malloc(tsize*sizeof(int));
  memset(tablePPTSA,-1,tsize*sizeof(int));
#else  
  node** tablePPTSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPTSA=calloc(len, sizeof(node));
  stype* arrPPTSA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPTSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPTSA, nodesPPTSA, arrPPTSA, sorter_tim_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPTSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPTSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPTSA);
  free(nodesPPTSA);
  free(arrPPTSA);
  doPrint(PPTSA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRTSA
#ifdef raTablePtr
  int* tablePPGRTSA=malloc(tsize*sizeof(int));
  memset(tablePPGRTSA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRTSA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRTSA=calloc(len, sizeof(node));
  stype* arrPPGRTSA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRTSA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRTSA, nodesPPGRTSA, arrPPGRTSA, sorter_tim_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRTSA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRTSA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRTSA);
  free(nodesPPGRTSA);
  free(arrPPGRTSA);
  doPrint(PPGRTSA,getDiff(t[0],t[1]));
#endif
#ifdef PPSQA
#ifdef raTablePtr
  int* tablePPSQA=malloc(tsize*sizeof(int));
  memset(tablePPSQA,-1,tsize*sizeof(int));
#else  
  node** tablePPSQA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPSQA=calloc(len, sizeof(node));
  stype* arrPPSQA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPSQA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPSQA, nodesPPSQA, arrPPSQA, sorter_sqrt_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPSQA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPSQA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPSQA);
  free(nodesPPSQA);
  free(arrPPSQA);
  doPrint(PPSQA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRSQA
#ifdef raTablePtr
  int* tablePPGRSQA=malloc(tsize*sizeof(int));
  memset(tablePPGRSQA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRSQA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRSQA=calloc(len, sizeof(node));
  stype* arrPPGRSQA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRSQA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRSQA, nodesPPGRSQA, arrPPGRSQA, sorter_sqrt_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRSQA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRSQA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRSQA);
  free(nodesPPGRSQA);
  free(arrPPGRSQA);
  doPrint(PPGRSQA,getDiff(t[0],t[1]));
#endif
#ifdef PPSHA
#ifdef raTablePtr
  int* tablePPSHA=malloc(tsize*sizeof(int));
  memset(tablePPSHA,-1,tsize*sizeof(int));
#else  
  node** tablePPSHA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPSHA=calloc(len, sizeof(node));
  stype* arrPPSHA= createArrRand(randHolder);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPSHA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPSHA, nodesPPSHA, arrPPSHA, sorter_shell_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPSHA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPSHA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPSHA);
  free(nodesPPSHA);
  free(arrPPSHA);
  doPrint(PPSHA,getDiff(t[0],t[1]));
#endif
#ifdef PPGRSHA
#ifdef raTablePtr
  int* tablePPGRSHA=malloc(tsize*sizeof(int));
  memset(tablePPGRSHA,-1,tsize*sizeof(int));
#else  
  node** tablePPGRSHA=calloc(tsize, sizeof(node*));
#endif  
  node* nodesPPGRSHA=calloc(len, sizeof(node));
  stype* arrPPGRSHA= createArrRuns(randHolder, runSize, runOffset);
#ifdef superVer
  for(int i=0;i<len;i++){
    verArr[i]=arrPPGRSHA[i];
  }
  qsort(verArr, len, sizeof(stype), intcomp);  
#endif
  gettimeofday(&t[0],NULL);
  testGetWithSort(tablePPGRSHA, nodesPPGRSHA, arrPPGRSHA, sorter_shell_sort);
  gettimeofday(&t[1],NULL);
#ifdef ver
  verCorrect(arrPPGRSHA);
#endif
#ifdef superVer
  for(int i =0;i<len;i++){
    if(verArr[i]!=arrPPGRSHA[i]){
      printf("Error\n");
      exit(0);
    }
  }
#endif
  free(tablePPGRSHA);
  free(nodesPPGRSHA);
  free(arrPPGRSHA);
  doPrint(PPGRSHA,getDiff(t[0],t[1]));
#endif



  
}
