#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 1000
#define MAXNAMELEN 1000

enum FILENAME {A,B}; // specify the file

void do_ls(char[]);
void do_stat(char *);
int filename_cmp(const void *, const void *);
void string_unified(char *, enum FILENAME);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

struct dirent *sorted_file_set[MAXLEN];
char filenamea[MAXNAMELEN]; // unified filename
char filenameb[MAXNAMELEN];

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_ls(".");
    } else {
        while (--argc) {
            printf("%s:\n", *++argv);
            do_ls(*argv);
        }
    }
}


void do_ls(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;
    int index = 0;

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls2: cannot open %s \n", dirname);
    } else {
        while (index < MAXLEN && (direntp = readdir(dir_ptr)) != NULL) {
            sorted_file_set[index++] = direntp;
        }
        printf("total %d\n", index);
        // sort by filename
        qsort(sorted_file_set, index, sizeof(sorted_file_set[0]), filename_cmp);

        // display the info about sorted filename
        for (int i = 0; i < index; ++i) {
            do_stat(sorted_file_set[i]->d_name);
        }

        closedir(dir_ptr);
    }
}

void do_stat(char *filename) {
    struct stat info;
    if (stat(filename, &info) == -1) {
        perror(filename);
    } else {
        show_file_info(filename, &info);
    }
}


void show_file_info(char *filename, struct stat * info_p) {
    char modestr[11];

    mode_to_letters(info_p->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4 + ctime(&info_p->st_mtime));
    printf("%s\n", filename);
}


int filename_cmp(const void *filepa, const void *filepb) {
    char *originnamea, *originnameb;
    originnamea = (*(struct dirent**)filepa)->d_name;
    originnameb = (*(struct dirent**)filepb)->d_name;

    string_unified(originnamea, A);
    string_unified(originnameb, B);

    return strcmp(filenamea, filenameb);
}


void string_unified(char *str, enum FILENAME name) {
    char *filename = NULL;
    if (name == A) {
        filename = filenamea;
    } else if (name == B) {
        filename = filenameb;
    }

    memset(filename, 0, MAXNAMELEN);
    if (str[0] != '.')
        filename[0] = '.';

    strcat(filename, str);
    for (int i = strlen(filename) - 1; i >= 0; --i) {
        filename[i] = tolower(filename[i]);
    }
}

void mode_to_letters(int mode, char str[]) {
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';

    if (mode & S_IRUSR) str[1] = 'r'; // 3 bits for user
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r'; // 3 bits for group
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r'; // 3 bits for other
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}


#include <pwd.h>


char * uid_to_name(uid_t uid) {
    struct passwd * getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}

#include <grp.h>


char * gid_to_name(gid_t gid) {
    struct group * grp_ptr;
    static char numstr[10];
    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
}