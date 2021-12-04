#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
        if(argc != 3) {
            printf("Supply two argument: ./test_rename <old-path> <new-path>\n");
            return 0;
        } 

        char* oldpath = argv[1];
        char* newpath = argv[2];

        int r;
        printf("Trying rename %s %s", oldpath, newpath);
        if((r = rename(oldpath, newpath)) != 0){
                printf("Error doing rename on %s -> %s\n", oldpath, newpath);
        }

        return 0;
}
