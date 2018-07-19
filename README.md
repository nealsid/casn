Preliminary implementation of CASN from https://www.microsoft.com/en-us/research/wp-content/uploads/2002/10/2002-disc.pdf to help with a lock-free deque implementation. It's limited to storing & updating pointer values since it overwrites the lower bit of values that are stored.

```sh
$ clang++ -g casn.cc  main.cc -o casn
$ ./casn
```
