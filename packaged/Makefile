CC=gcc
CFLAGS = -O3 -g -std=c99 -D_GNU_SOURCE 
LDLIBS = -lm 

all: example

example: example.o prepsort/prepsort.o sorts/timsort.o utils/printing.o utils/arg.o 
sorts/timsort.o: sorts/timsort.c sorts/timsort.h
prepsort/prepsort.o: prepsort/prepsort.c prepsort/prepsort.h
example.o: example.c prepsort/prepsort.h sorts/timsort.h sorts/sort.h utils/printing.h utils/arg.h
utils/arg.o: utils/arg.c utils/arg.h
utils/printing.o: utils/printing.c utils/printing.h




clean:
	rm -f *~ *.o *#* example prepsort/*~ prepsort/*.o prepsort/*#* sorts/*~ sorts/*.o sorts/*#* utils/*~ utils/*.o utils/*#*
