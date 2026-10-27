#include <stdio.h>
#include <string.h>

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

void search_in_file(FILE *file, const char *filename, const char *pattern, int show_filename) {
    char line[4096];
    size_t pattern_len = strlen(pattern);
    while (fgets(line, sizeof(line), file) != NULL) {
        char *match = strstr(line, pattern);       
        if (match != NULL) {
            if (show_filename) {
                printf("%s%s : %s", COLOR_GREEN, filename, COLOR_RESET);
            }
            char *current = line;
            while (match != NULL) {
                fwrite(current, 1, match - current, stdout);
                printf(COLOR_RED);
                fwrite(match, 1, pattern_len, stdout);
                printf(COLOR_RESET);
                current = match + pattern_len;
                match = strstr(current, pattern);
            }
            fputs(current, stdout);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pattern [filename]\n", argv[0]);
        return 1;
    }
    const char *pattern = argv[1];
    int file_count = argc - 2;
    size_t pattern_len = strlen(pattern);
    if (pattern_len == 0) {
        return 0;
    }
    if (file_count == 0) {
        search_in_file(stdin, NULL, pattern, 0);
    } else {
        int show_filename = (file_count > 1) ? 1 : 0;
        for (int i = 2; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror(argv[i]);
                continue; 
            }
            search_in_file(file, argv[i], pattern, show_filename);
            fclose(file);
        }
    }

    return 0;
}