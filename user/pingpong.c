#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char b = 'b';
  int pp[2];
  int cp[2];

  pipe(pp);
  pipe(cp);
  int pid = fork();
  if (pid == 0){
    // Child
    close(pp[1]);
    char buf[20]; // Just to print and check
    read(pp[0], &buf, 1);
    // printf("buf: %d\n", buf[0]);
    printf("%d: received ping\n", getpid());
    close(pp[0]);

    close(cp[0]);
    write(cp[1], &b, 1);
    close(cp[1]);
  } else if (pid > 0){
    // Parent
    close(cp[1]);
    close(pp[0]);
    write(pp[1], &b, 1);
    close(pp[1]);
    wait(0);

    char buf[20];
    read(cp[0], &buf, 1);
    // printf("buf: %d\n", buf[0]);
    printf("%d: received pong\n", getpid());
    close(cp[0]);
  } else {
    printf("fork error\n");
    exit(1);
  }
  exit(0);

}
