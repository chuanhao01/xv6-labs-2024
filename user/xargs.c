
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  if (argc < 2){
    fprintf(2, "Usage: xargs [program] ...\n");
    exit(1);
  }

  int xargc = argc - 1;
  char *xargv[MAXARG];
  // We are setting all *xargv which has alignment of sizeof(char *)
  memset(xargv, 0, MAXARG * sizeof(char *));
  // Skip first argv which is xargs, copy the reset into xargv
  memcpy(xargv, argv +  1,  xargc * sizeof(char *));

  // read from stdin
  char buf[500];
  int i = 0;
  // echo always adds a \n char at the end before the null terminated byte
  // almost all programs will place the prompt on a newline so all stdout will end with a \n
  while (read(0, &buf[i], 1) > 0){
    i++;
    // Done processing a line, should fork and exec
    if (buf[i-1] == '\n'){
      // Could have a arg in the buf
      if(i-1 != 0){
        // Process the arg in the buf
        buf[i-1] = 0; // Override the last \n with null byte term
        char *arg = malloc(strlen(buf) + 1); // strlen only gives the length of the string without null pointer
        strcpy(arg, buf); // copies the str including the null pointer
        xargv[xargc] = arg;
        xargc++;
      }
      int pid = fork();
      if (pid < 0){
        fprintf(2, "forkexec error");
        exit(1);
      }
      if (pid == 0){
        // Debug code
        // printf("child running:");
        // for (int ii=0; ii < xargc; ii++){
        //   printf(" %s", xargv[ii]);
        // }
        // printf("\n");
        // exit(0);

        // child to exec the new xarg line
        exec(argv[1], xargv);
        fprintf(2, "failed to exec xarg"); // If control was returned, exec failed
        exit(1);
      }
      wait(0);
      xargc = argc - 1;
      i = 0;
      continue;
    }
    if (buf[i-1] == ' '){
      // meet a white space, process an xarg
      if (i-1 == 0){
        // Previous was a whitespace or nothing
        i = 0;
        continue;
      }
      // buf = [c a t ' '], i = 4
      // add a null terminator
      buf[i-1] = 0; // Override last ' ' with null byte term
      // char *arg = malloc(i + 1);
      // memcpy(arg, buf, i + 1);
      // printf("str len: %d\n", strlen(buf));
      char *arg = malloc(strlen(buf) + 1); // strlen only gives the length of the string without null pointer
      strcpy(arg, buf); // copies the str including the null pointer
      xargv[xargc] = arg;
      xargc++;
      i = 0;
      continue;
    }
  }
  // printf("%s\n", buf);
  exit(0);
}
