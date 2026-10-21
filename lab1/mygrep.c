#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pattern [filename]\n", argv[0]);
        return 1;
    }

    FILE *file = stdin;

    if (argc >= 3) {
        file = fopen(argv[2], "r");
        if (!file) {
            perror(argv[2]);
            return 1;
        }
    }

    char line[4096];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, argv[1]) != NULL) {
            fputs(line, stdout);
        }
    }

    if (file != stdin) {
        fclose(file);
    }
    return 0;
}