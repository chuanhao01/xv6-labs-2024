#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *search_str)
{
  char buf[500], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if ((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    return;
  }
  switch (st.type)
  {
  case T_DEVICE:
  case T_FILE:
    // Should not be used to find, stderr
    fprintf(2, "find: should not be a file");
    break;
  case T_DIR:
    // Find a dir, we go through all inodes?
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/'; // Setting buf to be "[buf]/"
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      // Skip all inodes that don't exists? for 0
      // strcmp return 0 if true
      // skip . and ..
      if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      {
        continue;
      }
      memmove(p, de.name, DIRSIZ); // This does not move p, moves the de.name into "[buf]/[de.name]"
      p[DIRSIZ] = 0;               // Adding null point
      if (stat(buf, &st) < 0)
      {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      switch (st.type)
      {
      case T_DEVICE:
      case T_FILE:
        if (!strcmp(de.name, search_str))
        {
          printf("%s\n", buf);
        }
        break;
      case T_DIR:
        find(buf, search_str);
        break;
      }
    }
    break;
  }

  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: find [path] [file str]");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
