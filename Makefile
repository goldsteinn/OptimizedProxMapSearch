CC=gcc
CFLAGS = -O3 -g -std=c99 -D_GNU_SOURCE 
LDLIBS = -lm 

all: prepsort

prepsort: prepsort.o timsort/timsort.o utils/arg.o

prepsort.o: prepsort.c prepsort.h timsort/timsort.h ../sort/sort.h utils/arg.h
timsort/timsort.o: timsort/timsort.c timsort/timsort.h
utils/arg.o: utils/arg.c utils/arg.h

clean:
	rm *~ *.o timsort/*.o utils/*.o prepsort
