#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int compare(char* path, char* file) {
    char *p, *q;
    for (p = path + strlen(path); p >= path && *p != '/'; --p)
        ;
    ++p;
    q = file;
    for (; *p != 0 && *q != 0; ++p, ++q) {
        if (*p != *q) {
            return -1;
        }
    }
    if (*p != 0 || *q != 0) {
        return -1;
    }
    return 0;
}

void find(char* path, char* file) 
{
    // fprintf(1, "find: path [%s]\n", path);
    int fd;
    char buf[512];
    struct stat st;
    struct dirent de;
    char* p;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
    case T_FILE:
        if (compare(path, file) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof de) == sizeof de) {
            if (de.inum == 0) continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file);
        }
        break;
    default:
        break;
    }
    close(fd);
}

int
main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find [path] [filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

