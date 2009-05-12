#ifndef MEMORYSTATS_H
#define MEMORYSTATS_H

// at some point I'll want to have something like _DEBUG but for now just
// enable it.
//#ifdef _DEBUG
#define GUARD_VALUE 0xDEEDBEEF
#define NEXT4ALIGN(x) ( (x + 3) & ~0x3)


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


static u32 total = 0;
static u32 objects = 0;
static u32 oid = 0;

void *operator new(size_t sz)
{
    u8 *mem = reinterpret_cast<u8*>(malloc(sz+12+3));
    if (mem)
    {
        reinterpret_cast<u32*>(mem)[0] = sz; // insert size before data area
        reinterpret_cast<u32*>(mem)[1] = ++oid; // insert allocation id before data area
        mem += 8;
        *reinterpret_cast<u32*>(mem+NEXT4ALIGN(sz)) = GUARD_VALUE; // insert guard value at end of memory
        total += sz; // increment total size of allocations
        ++objects;
//        PrintStatusMessage("Alloc: %p,%u,%u,%u,%u,%p\n",mem,oid,sz,objects,total,__builtin_return_address(0));
    }
    else
    {
//        PrintStatusMessage("Failed allocate %u\n",sz);
    }
    return mem;

}

void operator delete(void *p) {
    u32 sz = reinterpret_cast<u32*>(p)[-2]; // retrieve size from memory
    u32 id = reinterpret_cast<u32*>(p)[-1]; // retrieve id
    if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(p)+NEXT4ALIGN(sz)) != GUARD_VALUE) // check against guard value
    {
//        PrintStatusMessage("MALLOC OVERFLOW!\n");
    }
    total -= sz; // update counter
    --objects;
//    PrintStatusMessage("Free:  %p,%u,%u,%u,%u\n",p,id,sz,objects,total);
    free(reinterpret_cast<u8*>(p)-8);
}

void PrintMemoryStatistics()
{
//    PrintStatusMessage("Objects: %u, sz: %u\n",objects,total);
    printf("Objects: %u, sz: %u\n",objects,total);
}


#endif
