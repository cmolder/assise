#include <stdio.h>

int main(int argc, char *argv[]) {
        if( argc != 1 ) {
            print("Supply one argument: ./test_write <filepath>\n");
        }  

        FILE *fp;
        char* filename = argv[0];

        if ((fp = fopen(filename, "w+")) == NULL) {
                printf("Error opening %s to write.\n", filename);
                return 0;
        }

        fputs("Hello world! This is a file that is contained inside the Assise filesystem. Enjoy!\n", fp);
        fclose(fp);
}
