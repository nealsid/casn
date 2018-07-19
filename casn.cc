#include <atomic>
#include <iostream>

#include "casn.h"

namespace casn {
  
void* rdcss(RDCSSDescriptor* d) {
  void* oldValue;
  void* descriptorPointer = (void *)((int)d | 1)
  do {
    // Try to compare the expected old value with what's at a2 and
    // swap it with the descriptor pointer.
    bool succeeded = d->a2.compare_exchange_weak(d->o2, descriptorPointer);
    // If we didn't succeed, figure out if the current value is a
    // descriptor.
    if (!succeeded) {
      // We don't have the same CAS primitive as the paper requires -
      // ours doesn't return the old value.  We can load the old value
      // if the CAS fails, but that introduces a race condition.  The
      // cases are as follows:
      //
      // 1) The value is the same as when the CAS failed - we're fine.
      // 2) The value has been updated to a non-descriptor value - we're fine
      // 3) The value has been updated to another descriptor, in which
      // case we'll try to help it finish it, as the algorithm
      // prescribes, and then retry our own operation.
      oldValue = d->a2.load();
      if (isDescriptor(oldValue)) {
	complete((RDCSSDescriptor*)oldValue);
      }
    } else {
      oldValue = d->o2;
    }
  } while (isDescriptor(oldValue));

  // If the CAS succeeded, the loop will break and we'll complete the swap here.
  if (oldValue == d->o2) {
    complete(d);
  }

  return oldValue;
}

void complete(RDCSSDescriptor* d) {
  void* controlValue = d->a1.load();
  void* descriptor_void_ptr = (void *) d & (~2 << sizeof(void*));

  if (controlValue == d->o1) {
    d->a2.compare_exchange_weak(descriptor_void_ptr, d->n2);
  } else {
    d->a2.compare_exchange_weak(descriptor_void_ptr, d->o2);
  }
}

bool isDescriptor(void* p) {
  cout << "checking if " << p << " is a descriptor: " << ((uint64_t)p & 1) << endl;
  return ((uint64_t)p & 1);
}

}
