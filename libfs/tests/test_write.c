#include <stdio.h>

int main() {
        FILE *fp;
        char* filename = "/mlfs/test_file_access.txt";

        if ((fp = fopen(filename, "w+")) == NULL) {
                printf("Error opening %s to write.\n", filename);
                return 0;
        }

        fputs("Hello world! This is a file that is contained inside the Assise filesystem. Enjoy!\n", fp);
        fclose(fp);
}
