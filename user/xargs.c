#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

// not used
void combine_str(int argc, char *argv[], char * ret) {
  int pos = 0;
  for (int i=1; i<argc; i++) {
    printf("origin: %d/%d: %s ", i, argc-1, argv[i]);
    char* argv_pos = argv[i];
    while(*argv_pos != 0) {
      ret[pos++] = *argv_pos;
      argv_pos++;
    }
    ret[pos++] = ' ';
  }

  ret[pos] = 0; // mark end
  printf("to %s", ret);
}

void run_exec(char *cmd, char *argv[], int argc){
  // printf("cmd: %s, ", cmd);
  // for (int i=1; i<argc; i++) {
  //   printf("%s ", argv[i]); 
  // }
  int pid = fork();
  if (pid > 0)
  {
    wait(0);
  } else{
    exec(cmd, argv); 
  }
}

// learn: exec will exit process
// todo: now using one char* as xargs new params, 
//   can split by ' ' to multi char* s
int main(int argc, char *argv[])
{
  char *sub_argv[MAXARG];// = {"echo","1234 5674"};
  // exec("echo", sub_argv);
  // exit(0);

  for (int i=1; i<argc; i++) {
    sub_argv[i-1] = argv[i]; 
  }

  int pos = 0;
  // int next_argc = argc;
  char para[512];
  char c;
  while(read(0, &c, 1) == 1) {
    // printf("read: '%c'\n", c);
    if (c != '\n') {
      para[pos++] = c;
    } else {
      // printf("param: %s\n", para);
      sub_argv[argc-1] = para;
      para[pos] = 0;
      pos = 0;
      run_exec(sub_argv[0], sub_argv, argc);
    }
  }
  
  exit(0);
}
