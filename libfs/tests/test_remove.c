#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <grp.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
        if(argc != 2) {
            printf("Supply one argument: ./test_remove <path>\n");
            return 0;
        } 

        FILE *fp;
        char* filename = argv[1];

        int r;
        printf("Trying remove %s", filename);
        if((r = remove(filename)) != 0){
                printf("Error doing remove on %s\n", filename);
        }

        return 0;
}
