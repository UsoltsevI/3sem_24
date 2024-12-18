#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv);
        return 1;
    }

    struct stat fileStat;
    if (stat(argv[1], &fileStat) < 0) {
        perror("stat");
        return 1;
    }

    printf("Information for %s\n", argv[1]);
    printf("---------------------------\n");

    // File size
    printf("File Size: \t\t%d bytes\n", fileStat.st_size);

    // Number of links
    printf("Number of Links: \t%d\n", fileStat.st_nlink);

    // File inode
    printf("File inode: \t\t%d\n", fileStat.st_ino);

    // File permissions
    printf("File Permissions: \t");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");

    // Check if it is a symbolic link
    printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");

    // Last access, modification, and status change times
    printf("Last access: %s", ctime(&fileStat.st_atime));
    printf("Last modification: %s", ctime(&fileStat.st_mtime));
    printf("Last status change: %s", ctime(&fileStat.st_ctime));

    return 0;
}
