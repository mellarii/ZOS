#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#ifndef S_ISLNK
#define S_ISLNK(m) 0
#endif
#ifndef S_IXGRP
#define S_IXGRP 0
#endif
#ifndef S_IXOTH
#define S_IXOTH 0
#endif

int compare_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_colored_name(const char *name, struct stat *st, int flag_l, const char *full_path) {
    if (S_ISDIR(st->st_mode)) {
        printf("\033[1;34m%s\033[0m", name);
    } else if (S_ISLNK(st->st_mode)) {
        printf("\033[1;36m%s\033[0m", name);
    } else if (st->st_mode & S_IXUSR) {
        printf("\033[1;32m%s\033[0m", name);
    } else {
        printf("%s", name);
    }

    if (flag_l && S_ISLNK(st->st_mode)) {
        char link_target[1024];
        ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);
        if (len != -1) {
            link_target[len] = '\0';
            printf(" -> %s", link_target);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int flag_l = 0, flag_a = 0, opt;

    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt) {
            case 'l': flag_l = 1; break;
            case 'a': flag_a = 1; break;
            default: return 1;
        }
    }

    char *dirname = (optind < argc) ? argv[optind] : ".";
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("Error opening directory");
        return 1;
    }

    char **filenames = NULL;
    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!flag_a && entry->d_name[0] == '.') continue;

        char **tmp = realloc(filenames, sizeof(char *) * (count + 1));
        if (!tmp) {
            perror("realloc");
            for (int i = 0; i < count; i++) free(filenames[i]);
            free(filenames);
            closedir(dir);
            return 1;
        }
        filenames = tmp;
        filenames[count] = strdup(entry->d_name);
        if (!filenames[count]) {
            perror("strdup");
            for (int i = 0; i < count; i++) free(filenames[i]);
            free(filenames);
            closedir(dir);
            return 1;
        }
        count++;
    }
    closedir(dir);

    if (count > 0)
        qsort(filenames, count, sizeof(char *), compare_names);

    for (int i = 0; i < count; i++) {
        char full_path[2048];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirname, filenames[i]);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            perror(filenames[i]);
            free(filenames[i]);
            continue;
        }

        if (flag_l) {
            printf((S_ISDIR(st.st_mode)) ? "d" : (S_ISLNK(st.st_mode)) ? "l" : "-");
            printf((st.st_mode & S_IRUSR) ? "r" : "-");
            printf((st.st_mode & S_IWUSR) ? "w" : "-");
            printf((st.st_mode & S_IXUSR) ? "x" : "-");
            printf((st.st_mode & S_IRGRP) ? "r" : "-");
            printf((st.st_mode & S_IWGRP) ? "w" : "-");
            printf((st.st_mode & S_IXGRP) ? "x" : "-");
            printf((st.st_mode & S_IROTH) ? "r" : "-");
            printf((st.st_mode & S_IWOTH) ? "w" : "-");
            printf((st.st_mode & S_IXOTH) ? "x" : "-");

            char time_buf[80];
            struct tm *tm_info = localtime(&st.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);

            printf(" %2ld %-8d %-8d %8ld %s ",
                   (long)st.st_nlink, st.st_uid, st.st_gid, (long)st.st_size, time_buf);
        }

        print_colored_name(filenames[i], &st, flag_l, full_path);

        free(filenames[i]);
    }

    free(filenames);
    return 0;
}
