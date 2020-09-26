#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void read_and_write(int *p, int level)
{
  // printf("reach level %d\n", level);
  // sleep(level); // slow down creating pipe, but not work

  char first = 0; // char in uint8[0-255]
  char c = 127;
  if (read(p[0], &c, 1) == 1)
  {
    first = c;
    printf("prime %d\n", c);
  }
  else
  {
    printf("can not get more input\n");
    exit(0);
  }

  int p_child[2];
  while (pipe(p_child) < 0) // wait for other release resource, not work
  {
    printf("error create pipe, in level %d", level);
    sleep(100);
  }

  int left = 0;
  // parent
  while (read(p[0], &c, 1) == 1)
  {
    left++;
    // continue;
    if (c % first != 0)
    {
      // printf("in level %d, get %d, prime %d\n", level, c, first);
      if (write(p_child[1], &c, 1) != 1)
      {
        printf("child write to p1 failed, in level %d\n", level);
        exit(1);
      }
    }
  }

  close(p[0]);
  close(p_child[1]);

  int pid = fork();
  if (pid > 0)
  {
    if (left != 0)
      wait(0);
  }
  else
  {
    // sleep(10);
    read_and_write(p_child, level + 1);
  }
  exit(0);
}

// a prime algorithm
int main(int argc, char *argv[])
{
  int p[2];
  if (pipe(p) < 0)
  {
    printf("error create pipe");
    exit(1);
  }

  for (char i = 2; i < 35; i++)
  {
    if (write(p[1], &i, 1) != 1)
    {
      printf("child write to p1 failed\n");
      exit(1);
    }
  }
  close(p[1]);

  // learned: the read return value 0 is useful for subprocess 
  // fast catch the exit status.

  // from pingpong: p[1] for read, p[0] for write
  //   : in case self write be read by self, add wait 

  // from sleep: sleep is in 1/10 second

  // learned: we should wirte all data in to pipe, and 
  // then start subprocess, so that only several 
  // fd in use;
  int pid = fork();
  if (pid > 0)
  {
    wait(0);
  }
  else
  {
    read_and_write(p, 1);
  }
  exit(0);
}
