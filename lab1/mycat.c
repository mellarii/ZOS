#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int flag_E = 0;
    int flag_n = 0;
    int flag_b = 0;
    int file_index = 1;

    while (file_index < argc && argv[file_index][0] == '-') {
        if (strcmp(argv[file_index], "-E") == 0) {
            flag_E = 1;
        } else if (strcmp(argv[file_index], "-n") == 0) {
            flag_n = 1;
        } else if (strcmp(argv[file_index], "-b") == 0) {
            flag_b = 1;
        } else {
            fprintf(stderr, "Unknown flag %s\n", argv[file_index]);
            return 1;
        }
        file_index++;
    }

    if (flag_b) {
        flag_n = 0;
    }

    FILE *file = stdin;
    if (file_index < argc) {
        file = fopen(argv[file_index], "r");
        if (!file) {
            perror(argv[file_index]);
            return 1;
        }
    }

    int ch;
    int line_num = 1;
    int is_start = 1;
    int has_content = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (is_start) {
            if (flag_n || (flag_b && ch != '\n')) {
                printf("%6d\t", line_num++);
            }
            is_start = 0;
            has_content = (ch != '\n');
        } else if (has_content == 0 && ch != '\n') {
            has_content = 1;
        }

        if (flag_E && ch == '\n') {
            putchar('$');
        }
        putchar(ch);

        if (ch == '\n') {
            is_start = 1;
            has_content = 0;
        }
    }

    if (file != stdin) {
        fclose(file);
    }
    return 0;
}
