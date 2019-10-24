This is an implementation of proxmap sort combined with timsort. Generic functions for prepsort are
available in prepsort.c/prepsort.h. It is recommended to combine prepsort with timsort. Prepsort
combined with timsort will outperform all other sorts in nearly all cases. requires additional
memory equal to len*(data type size + 8).

An example of using prepsort with timsort on chars, shorts, ints, longs, and strings is example.c
Note, especially with strings, it is important to create a good evaluation function. The one
present in example.c is what I have found to be the best.

prepsort:
The basic idea is to get the data roughly into sorted order in O(n) time before calling the true
sort function. Especially with sorts like timsort which perform better on data which is already
nearly sorted, doing this can often lead to a significantly faster sort.
