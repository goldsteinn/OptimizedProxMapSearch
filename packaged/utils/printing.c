#include "printing.h"

const char p_type[numTypes][32]={
  "long",
  "int",
  "short",
  "char",
  "string",
  "generic"
};

const char p_fun[numFuns][32]={
  "Prep Timsort",
  "Quick Sort"
};


void doPrint(int funNum, int typeNum, double tDif){
  int total=strlen(p_type[typeNum]);
  total+= strlen(p_fun[funNum]);
  printf("--------------------------------------------------------------------------\n");
  printf("%s %s", p_type[typeNum], p_fun[funNum]);
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

