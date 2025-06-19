#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  trace(0xFFFF);
  int pid = fork();
  if (pid==0){
    char *aargv[] = {"secret", "hello", 0};
    exec(aargv[0], aargv);
  }
  wait(0);
  char buf[500];
  memset(buf, 0, sizeof(buf));
  char *end = sbrk(PGSIZE*32); // Adding 32 pages(4096) of memory
  end = end + 9 * PGSIZE; // Move pointer 9 pages forward
  end = end + 32;
  for(int i=0; i<32; i = i+1){
    printf("%x ", *(end+i));
  }
  // strcpy(buf, end+1);
  printf("%s\n", buf);
  // strcpy(end, "my very very very secret pw is:   ");
  // printf("end %s\n", end+32);


  exit(1);
}

// #include "kernel/types.h"
// #include "kernel/fcntl.h"
// #include "user/user.h"
// #include "kernel/riscv.h"

// int
// main(int argc, char *argv[])
// {
//   // your code here.  you should write the secret to fd 2 using write
//   // (e.g., write(2, secret, 8)
//   if(argc != 1){
//     printf("Usage: secret the-secret\n");
//     exit(1);
//   }
//   char *end = sbrk(PGSIZE*32);
//   end = end + 8 * PGSIZE;
//   fprintf(2, end+16, 8);
//   // char *h = "hello\n";
//   // fprintf(1, h, 6);
//   exit(1);
// }
