#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int
main(int argc, char *argv[])
{
  // trace(0xFFFF);
  if(argc != 2){
    printf("Usage: secret the-secret\n");
    exit(1);
  }
  char *end = sbrk(PGSIZE*32); // Adding 32 pages(4096) of memory
  // printf("%d\n", PGSIZE*32);
  // printf("p %p\n", end);
  end = end + 9 * PGSIZE; // Move pointer 9 pages forward
  strcpy(end, "my very very very secret pw is:   ");
  strcpy(end+32, argv[1]); // Copy secret over
  exit(0);
}
