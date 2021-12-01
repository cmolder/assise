#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
        if(argc != 2) {
            print("Supply two arguments: ./test_mkdir <dirpath> <mode>\n");
        }    

        FILE *fp;
        char* dirname = argv[0];
        char* mode = argv[1]; // e.g. 0777: Anyone can read/write/execute stuff inside.

        int i = strtol(mode, 0, 8);
        int r;

        printf("Trying mkdir %s (mode %s)", dirname, mode);
        if((r = mkdir(dirname, i)) != 0){
                printf("Error doing mkdir on %s\n", dirname);
                return 0;
        }

        struct stat info;
        r = stat(dirname, &info);
        if (r != 0) {
                printf("Error reading stat on %s\n", dirname);
        }
        printf("Directory owner: %d\n", info.st_uid);
        printf("Directory group: %d\n", info.st_gid);
        printf("Directory perms: %o\n", info.st_mode);
        printf("Directory ctime: %d\n", info.st_ctime);
}