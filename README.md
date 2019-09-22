# OptimizedProxMapSearch
Optimized verison of proxmap search, also used to preprocess the data before using other sorting algorithms.

main is in prepsort.c (also comments)

args are
1) random seed (just throw in whatever you want)
2) max size of the runs to be generated
3) distance between elements in runs (i.e run[1]=run[0]+arg3; run[2]=run[0]+arg*2; etc...)

inside usetim are a bunch of #defines for different possible configurations/sorting methods to run, just #define out that ones that you dont care about.

to build: make


dir timsort from:
https://github.com/patperry/timsort/blob/master/timsort.h

dir sort from:
https://github.com/swenson/sort


Generally outperforms all other sorting algorithms in throughput. With raPtr and raTablePtr both defined does this at the cost of +8 bytes per element to be sorted.
