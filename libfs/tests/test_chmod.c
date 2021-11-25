#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main() {
        FILE *fp;
        char* filename = "/mlfs/test_file_access.txt";
        char* mode = "0600"; // No read for non-user

        int i = strtol(mode, 0, 8);
        int r;

        printf("Trying chmod %s to mode %s", filename, mode);
        if((r = chmod(filename, i)) != 0){
                printf("Error doing chmod on %s\n", filename);
        }

        struct stat info;
        r = stat(filename, &info);
        if (r != 0) {
                printf("Error reading stat\n");
        }
        printf("File owner: %d\n", info.st_uid);
        printf("File group: %d\n", info.st_gid);
        printf("File perms: %o\n", info.st_mode);
        printf("File ctile: %d\n", info.st_ctime);
}





