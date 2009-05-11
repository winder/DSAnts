#ifndef MEMORYSTATS_H
#define MEMORYSTATS_H

#include <unistd.h>
#include <malloc.h>

extern u8 __end__[];        // end of static code and data
extern u8 __eheap_end[];    // farthest point to which the heap will grow

u8* getHeapStart() {
   return __end__;
}
u8* getHeapEnd() {
   return (u8*)sbrk(0);
}
u8* getHeapLimit() {
   return __eheap_end;
}
int getMemUsed() {
   struct mallinfo mi = mallinfo();
   return mi.uordblks;
}
int getMemFree() {
   struct mallinfo mi = mallinfo();
   return mi.fordblks + (getHeapLimit() - getHeapEnd());
} 

#endif
