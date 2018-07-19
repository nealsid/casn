#include <iostream>
#include "casn.h"

using namespace casn;

int main(int argc, char *argv[]) {

  int a = 1, b = 2;
  int* aptr = &a, *bptr = &b;
  int c = 5;
  int* cptr = &c;

  RDCSSDescriptor r;
  r.a1.store(aptr);
  r.o1 = aptr;
  r.a2.store(bptr);
  r.o2 = bptr;
  r.n2 = cptr;

  cout << * (int *)(r.a2.load()) << endl;
  rdcss(&r);
  cout << * (int *)(r.a2.load()) << endl;
}
