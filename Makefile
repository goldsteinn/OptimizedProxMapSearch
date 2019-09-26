CC=gcc
CFLAGS = -O3 -g -std=c99 -D_GNU_SOURCE 
LDLIBS = -lm 

all: compare_sorts prepsort

prepsort: prepsort.o utils/arg.o

compare_sorts: compare_sorts.o timsort/timsort.o utils/arg.o

compare_sorts.o: compare_sorts.c compare_sorts.h timsort/timsort.h sort/sort.h utils/arg.h
prepsort.o: prepsort.c prepsort.h sort/sort.h utils/arg.h

timsort/timsort.o: timsort/timsort.c timsort/timsort.h

utils/arg.o: utils/arg.c utils/arg.h

clean:
	rm *~ *.o timsort/*.o utils/*.o compare_sorts prepsort
