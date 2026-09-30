#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Error: use .\\mygrep [word] [filename].");
    return 1;
  }

  printf("Search word: %s.\n", argv[1]);
  FILE *file = stdin;
  
  if (argc >=3) {
    printf("In File: %s.\n", argv[2]);
    file = fopen(argv[2], "r");
    if (file == 0) { 
      printf("Error: File is empty ot We cant open file");
      return 1;
    }
    printf("File is open.\n");
  } else {
    printf("Reading from standart input...\n");
  }

  char line[1024];
  int cnt = 0;
  while (fgets(line, sizeof(line), file) != 0) {
    if (strstr(line, argv[1])) { printf("%s", line); }
  }

  if (file != stdin) {
    fclose(file);
  }
  return 0;
}