#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int is_point(char *path) {
  int len = strlen(path);
  if (path[len-1] == '.') {
    return 1;// false
  }
  return 0;
}

char* filename(char *path) {
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  // printf("path %s filename: %s\n", path, p);
  return p;
}

void print_stat(char *path, char *name) {
  if (strcmp(filename(path), name) == 0){
    printf("%s\n", path);
  }
}

void
find(char *path, char *name)
{
  // printf("find %s, in %s\n", name, path);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    print_stat(path, name);
    break;

  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_DIR && is_point(buf) != 1){
        find(buf, name);
        // continue;
      }
      print_stat(buf, name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: file dir name\n");
    exit(0);
  }
  
  find(argv[1], argv[2]);
  exit(0);
}
