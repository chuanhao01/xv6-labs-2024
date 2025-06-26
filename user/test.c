#include "kernel/types.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
  printf("dbkpgtbl starting\n");
  dbkpgtbl();
  printf("dbkpgtbl : OK\n");
  exit(0);
}
