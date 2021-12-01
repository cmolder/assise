#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
        if(argc != 3) {
            printf("Supply two arguments: ./test_chmod <path> <mode>\n");
            return 0;
        } 

        FILE *fp;
        char* filename = argv[1];
        char* mode = argv[2]; // e.g. "0600" : No read for non-owner

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





