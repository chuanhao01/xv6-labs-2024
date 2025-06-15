#include "kernel/types.h"
#include "user/user.h"

#define PRIME_NUM 280

// void primes(int) __attribute__((noreturn));

void primes(int rp)
{
  // rp, read pipe fd
  short p;
  if (read(rp, &p, 2) == 0)
  {
    close(rp);
    exit(0);
  }
  // First num is always prime
  printf("prime %d\n", p);
  int pp[2];
  pipe(pp);
  short n;
  while (read(rp, &n, 2) != 0)
  {
    if (n % p != 0)
    {
      write(pp[1], &n, 2);
    }
  }

  close(rp);
  close(pp[1]);
  // primes(pp[0]);
  // Does not need this, but its good
  int pid = fork();
  if (pid == 0)
  {
    primes(pp[0]);
  }
  else
  {
    wait(0);
  }
}

int main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid < 0)
  {
    printf("fork error\n");
    exit(1);
  }
  if (pid == 0)
  {
    // Child
    close(p[1]);
    // int num;
    // int status = read(p[0], &num, 4);
    // int n = num;
    // printf("prime %d\n", num);
    // while (status != 0)
    // {
    //   status = read(p[0], &num, 4);
    //   if (num%n != 0){
    //     // could be prime pass it down the pipe
    //     printf("prime %d\n", num);
    //   }
    // }
    // close(p[0]);
    primes(p[0]);
    exit(0);
  }

  close(p[0]);
  for (short i = 2; i < PRIME_NUM; i++)
  {
    write(p[1], &i, 2);
  }
  close(p[1]);
  wait(0);
  // printf("all child completed\n");
  exit(0);
}
