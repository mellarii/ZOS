#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s mode file...\n", argv[0]);
        return 1;
    }

    char *mode_str = argv[1];
    int is_octal = 1;

    for (int i = 0; mode_str[i] != '\0'; i++) {
        if (!isdigit(mode_str[i])) {
            is_octal = 0;
            break;
        }
    }

    if (is_octal) {
        char *end;
        mode_t mode = strtol(mode_str, &end, 8);
        if (*end!='\0') {
            perror("Oshibochka");
            return 1;
        }
        for (int i = 2; i < argc; i++) {
            if (chmod(argv[i], mode) == -1) {
                perror(argv[i]);
            }
        }
    } else {
        for (int i = 2; i < argc; i++) {
            struct stat st;
            if (stat(argv[i], &st) == -1) {
                perror(argv[i]);
                continue;
            }
            
            mode_t new_mode = st.st_mode;
            int who = 0;   
            int op = 0;
            int perms = 0;

            for (int j = 0; mode_str[j] != '\0'; j++) {
                char c = mode_str[j];
                if (c == 'u') who |= 1;
                else if (c == 'g') who |= 2;
                else if (c == 'o') who |= 4;
                else if (c == 'a') who |= 7;
                else if (c == '+') op = 1;
                else if (c == '-') op = 2;
                else if (c == '=') op = 3;
                else if (c == 'r') perms |= 4;
                else if (c == 'w') perms |= 2;
                else if (c == 'x') perms |= 1;
            }
            if (who == 0) who = 7;

            mode_t add_mask = 0;
            if (who & 1) {
                if (perms & 4) add_mask |= S_IRUSR;
                if (perms & 2) add_mask |= S_IWUSR;
                if (perms & 1) add_mask |= S_IXUSR;
            }
            if (who & 2) {
                if (perms & 4) add_mask |= S_IRGRP;
                if (perms & 2) add_mask |= S_IWGRP;
                if (perms & 1) add_mask |= S_IXGRP;
            }
            if (who & 4) {
                if (perms & 4) add_mask |= S_IROTH;
                if (perms & 2) add_mask |= S_IWOTH;
                if (perms & 1) add_mask |= S_IXOTH;
            }

            if (op == 1) {
                new_mode |= add_mask;
            } else if (op == 2) {
                new_mode &= ~add_mask;
            } else if (op == 3) {
                mode_t clear_mask = 0;
                if (who & 1) clear_mask |= S_IRWXU;
                if (who & 2) clear_mask |= S_IRWXG;
                if (who & 4) clear_mask |= S_IRWXO;
                new_mode &= ~clear_mask;
                new_mode |= add_mask;
            }

            if (chmod(argv[i], new_mode) == -1) {
                perror(argv[i]);
            }
        }
    }

    return 0;
}