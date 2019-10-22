#include "printing.h"

const char p_type[numTypes][32]={
  ""
};

const char p_fun[numFuns][32]={
  "Prep Timsort['int']",
  "Prep Timsort['str']",
  "qsort['int']",
  "qsort['str']",
  "Prep Merge Sort['int']",
  "Prep Heap Sort['int']"
};


void doPrint(int defNum, double tDif){
  int total=strlen(p_type[defNum%numTypes]);
  total+= strlen(p_fun[defNum/numTypes]);
  printf("--------------------------------------------------------------------------\n");
  printf("%s %s", p_type[defNum%numTypes], p_fun[defNum/numTypes]);
  for(int i =total;i<printLen;i++){
    printf(" ");
  }
  printf(": %.2lf ms\n", tDif);
  printf("--------------------------------------------------------------------------\n");

}

double getDiff(struct timeval t1, struct timeval t2){
  double ms1=t1.tv_sec*1000 + t1.tv_usec/1000.0;
  double ms2=t2.tv_sec*1000 + t2.tv_usec/1000.0;
  return ms2-ms1;
}

