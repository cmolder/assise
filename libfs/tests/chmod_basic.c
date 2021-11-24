#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mlfs/mlfs_interface.h>

int main(int argc, char ** argv)
{
    int fd;
    // int bytes, ret;
    // char buffer[BUF_SIZE], str[BUF_SIZE];
    // int write_count;
    fd = open("/mlfs/perm_testfile", O_RDWR|O_CREAT, 0600);

    if (fd < 0) {
        perror("open with O_CREAT");
        return 1;
    }

    char buffer[10] = "01234578";

    write(fd, buffer, strlen(buffer));

    close (fd);

    chmod("/mlfs/perm_testfile", 0777);



    return 0;
}

