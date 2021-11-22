#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main() {
        FILE *fp;
        char* filename = "/mlfs/test_file_access.txt";

        char buff[255];

        fp = fopen(filename, "r");
        fgets(buff, 255, (FILE*)fp);
        printf("First 255 chars : %s\n", buff );

        struct stat info;
        int r = stat(filename, &info);
        if (r != 0) {
                printf("Error reading stat\n");
        }
        printf("File owner: %d\n", info.st_uid);
        printf("File group: %d\n", info.st_gid);
        printf("File perms: %o\n", info.st_mode);
        printf("File ctile: %d\n", info.st_ctime);
}
