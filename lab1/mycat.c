#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  printf("Arguments count: %d\n", argc);
  if (argc < 2) {
    printf("Error: specify a file\n");
    return 1;
  }

  int flag_E = 0;
  int flag_n = 0;
  int flag_b = 0;
  int file_index = 1;
  char *filename = NULL;

  while (file_index < argc && argv[file_index][0] == '-') {
    if (strcmp(argv[file_index], "-E") == 0) { 
      flag_E = 1;
    } else if (strcmp(argv[file_index], "-n") == 0) {
      flag_n = 1;
    } else if (strcmp(argv[file_index], "-b") == 0) {
      flag_b = 1;
    } else {
      printf("Unknown flag %s", argv[file_index]);
      return 1;
    }
    file_index++;
  }

  if (flag_b) {
    flag_n = 0;
  }

  if (file_index >= argc) {
    printf("Error: specify a file");
    return 1;
  }


  FILE *file = fopen(argv[file_index], "r");
  if (file == 0) {
    printf("Error: Cant open file %s\n", file);
    return 1;
  }

  int ch;
  int line_num = 1;
  int is_start = 1;

  while ((ch = fgetc(file)) != EOF) {
    if (is_start) {
      if (flag_n || (flag_b && ch != '\n')) {
        printf("%6d\t", line_num++);
      }
      is_start = 0;
    }

    if (flag_E && ch == '\n') {
      putchar('$');
    }
    putchar(ch);

    if (ch == '\n') {
      is_start = 1;
    }
  }

  fclose(file);
  return 0;
}