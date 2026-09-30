#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Error: use .\\myls __OR[flag] [dirname]");
    return 1;
  }
  int fileindex = 1;
  char filename = NULL;
  int flag_l = 0;
  int flag_a = 0;
  
  

  DIR *dir_name = opendir(argv[2]);

  closedir(dir_name);
  return 0;
}