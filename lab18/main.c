#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define SIZE_BUF 200
#define TIME_LENGTH 200
#define MODE_LENGTH 11
#define ERROR -1
#define CURRENT "."
#define NUM_OF_ARGS 1
#define SUCCESS 0

void getMode(struct stat stat) {
    char mode[MODE_LENGTH] = { 0 };
    switch (stat.st_mode & S_IFMT) {
        case S_IFDIR: mode[0] = 'd'; break;
        case S_IFREG: mode[0] = '-'; break;
        default: mode[0] = '?';
    }
    //user
    mode[1] = (stat.st_mode & S_IRUSR) ? 'r' : '-';
    mode[2] = (stat.st_mode & S_IWUSR) ? 'w' : '-';
    mode[3] = (stat.st_mode & S_IXUSR) ? 'x' : '-';
    //group
    mode[4] = (stat.st_mode & S_IRGRP) ? 'r' : '-';
    mode[5] = (stat.st_mode & S_IWGRP) ? 'w' : '-';
    mode[6] = (stat.st_mode & S_IXGRP) ? 'x' : '-';
    //other
    mode[7] = (stat.st_mode & S_IROTH) ? 'r' : '-';
    mode[8] = (stat.st_mode & S_IWOTH) ? 'w' : '-';
    mode[9] = (stat.st_mode & S_IXOTH) ? 'x' : '-';
    mode[10] = '\0';
    printf("%s", mode);
}

void getNumberLinks(struct stat stat) {
    printf("%4d", stat.st_nlink);
}

int getOwner(struct stat stat) {
    struct passwd* pwd;
    pwd = getpwuid(stat.st_uid);
    if (pwd == NULL) {
        printf("getpwuid error\n");
        return ERROR;
    } else {
        printf(" %s", pwd->pw_name);
    }
    return SUCCESS;
}

int getGroup(struct stat stat) {
    struct group* grp;
    grp = getgrgid(stat.st_gid);
    if (grp == NULL) {
        printf("getgrgid error");
        return ERROR;
    } else {
        printf(" %s", grp->gr_name);
    }
}

void getFileSize(struct stat stat){
    printf("    %ld", stat.st_size);
}

int getTime(struct stat stat) {
    char *time = ctime(&stat.st_mtime);
    if (time == NULL) {
        printf("ctime error");
        return ERROR;
    } else {
        printf(" %s", time);
    }
}

void getFileName(char** dirs, int i) {
    printf(" %s\n", dirs[i]);
}

int lsCommand(char** dirs) {
    struct stat stat;
// struct stat {
//     mode_t        st_mode;     /* тип файла и режим (права доступа) */
//     ino_t         st_ino;      /* (inode) номер индексного узла */
//     dev_t         st_dev;      /* номер устройства (файловой системы) */
//     dev_t         st_rdev;     /* номер устройства для спец файлов */
//     nlink_t       st_nlink;    /* количество ссылок */
//     uid_t         st_uid;      /* идентификатор пользователя владельца */
//     gid_t         st_gid;      /* идентификатор группы владельца */
//     off_t         st_size;     /* размер в байтах, для обычного файла*/
//     time_t        st_atime;    /* время последнего обращения к файлу */
//     time_t        st_mtime;    /* время последнего изменения файла */
//     time_t        st_ctime;    /* время последнего изменения состояния файла */
//     blksize_t     st_blksize;  /* оптимальный размер блока ввода-вывода */
//     blkcnt_t      st_blocks;   /* количество занятых дисковых блоков */
// };

    for (int i = 0; dirs[i] != NULL; ++i) {
        if (lstat(dirs[i], &stat)) {
            perror("lstat() failed");
            return ERROR;
        }
        getMode(stat);
        getNumberLinks(stat);
        if (getOwner(stat) == ERROR) {
            return ERROR;
        }
        if (getGroup(stat) == ERROR) {
            return ERROR;
        }
        getFileSize(stat);
        if (getTime(stat) == ERROR) {
            return ERROR;
        }
        getFileName(dirs, i);
    }
    return SUCCESS;
}

int main (int argc, char* argv[]) {
    char* args[] = {CURRENT, NULL};
    char** dirs = NULL;
    if (argc > NUM_OF_ARGS) {
        dirs = argv + 1;
    } else {
        dirs = args;
    }
    if (lsCommand(dirs) == ERROR) {
        return ERROR;
    }
    return SUCCESS;
}
