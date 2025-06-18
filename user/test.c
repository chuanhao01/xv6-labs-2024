#include "kernel/types.h"
#include "user/user.h"
#include "kernel/syscall.h"

int main(int argc, char *argv[])
{
  int trace_bm = 1 << SYS_close;
  int check = 1 & trace_bm >> SYS_close;
  printf("%d\n", trace_bm);
  printf("check %d\n", check);
  trace(trace_bm);
  exit(0);
}
