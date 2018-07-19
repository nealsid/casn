#ifndef _CASN_H_
#define _CASN_H_

#include <atomic>

using namespace std;

namespace casn {

struct RDCSSDescriptor {
  atomic<void*> a1;
  void* o1;
  atomic<void*> a2;
  void* o2;
  void* n2;
};

void* rdcss(RDCSSDescriptor* d);

void complete(RDCSSDescriptor* d);

bool isDescriptor(void* p);
}

#endif 
