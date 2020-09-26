#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int p[2];
  if (pipe(p) < 0)
  {
    printf("error create pipe");
    exit(1);
  }

  int pid = fork();
  char c;
  if (pid > 0)
  {
    // parent
    if (write(p[1], "x", 1) != 1) {
      printf("parent write to p1 failed\n");
      exit(1);
    }
    pid = wait(0);
    if (read(p[0], &c, 1) != 1){
      printf("parent read form pipe fail\n");
      exit(1);
    }
    printf("%d: received pong\n", 5);
  } else {
    if (read(p[0], &c, 1) != 1) {
      printf("child read form pipe fail\n");
      exit(1);
    }
    printf("%d: received ping\n", 4);

    if (write(p[1], "y", 1) != 1) {
      printf("child write to p1 failed\n");
      exit(1);
    }
  }
  exit(0);
}
