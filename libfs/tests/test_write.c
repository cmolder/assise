#include <stdio.h>

int main() {
        FILE *fp;
        fp = fopen("/mlfs/test_file_access.txt", "w+");
        fputs("Hello world! This is a file that is contained inside the Assise filesystem. Enjoy!\n", fp);
        fclose(fp);
}
