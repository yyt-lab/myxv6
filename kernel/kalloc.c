// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;
struct {
    struct spinlock lock;
    int pagecount_[32769];
} pgcnt;


void
kinit()
{
  initlock(&kmem.lock,  "kmem");
  initlock(&pgcnt.lock, "pgcnt");
  for (int i =0 ;i<32769;i++){
    pgcnt.pagecount_[i]=1;
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  acquire(&pgcnt.lock);
  pgcnt.pagecount_[PA2PGREF(pa)]--; // * free时 应用计数-1
    // printf("pgcnt.pagecount_[%d] %d\n",PA2PGREF(pa),pgcnt.pagecount_[PA2PGREF(pa)] );
  release(&pgcnt.lock);
  if (pgcnt.pagecount_[PA2PGREF(pa)] == 0){  // *只有当引用计数为0时，才会释放物理页
    memset(pa, 1, PGSIZE);
    // printf("here12323\n\n");
    r = (struct run*)pa;
    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
//   // Fill with junk to catch dangling refs.
//   memset(pa, 1, PGSIZE);

//   r = (struct run*)pa;

//   acquire(&kmem.lock);
//   r->next = kmem.freelist;
//   kmem.freelist = r;
//   release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
// * r 为按页对其的pa
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
//   printf("kalloc%x\n",r);
  if(r){
    kmem.freelist = r->next;
    int k = PA2PGREF(r);
    acquire(&pgcnt.lock);
    pgcnt.pagecount_[k] = 1; // * kalloc时设置应用计数为1
    release(&pgcnt.lock);
  }
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}


// ! increase the pagecount of the specific page
void 
increase_pgcount(uint64 pa)
{
    int k = PA2PGREF(pa);
    acquire(&pgcnt.lock);
    pgcnt.pagecount_[k]++;
    release(&pgcnt.lock);
}