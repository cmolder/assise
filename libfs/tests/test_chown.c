#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
        if(argc != 4) {
            printf("Supply three arguments: ./test_chown <path> <owner> <group> (\"null\" for owner/group means no change)\n");
            return 0;
        } 

        char* filename = argv[1];
        char* owner = argv[2];
        char* group = argv[3];

        uid_t target_owner = -1;
        gid_t target_group = -1;

        if (strcmp(owner, "null") != 0) {
                struct passwd *p;
                if ((p = getpwnam(owner)) == NULL) {
                        printf("Error retreiving uid of target owner %s\n", owner);
                        return 0;
                }
                target_owner = p->pw_uid;
        }

        if (strcmp(group, "null") != 0) {
                struct group *g;
                if ((g = getgrnam(group)) == NULL) {
                        printf("Error retreiving gid of target group %s\n", group);
                        return 0;
                }
                target_group = g->gr_gid;
        }

        printf("Trying chown %s to owner %s (%d), group %s (%d)", filename, owner, target_owner, group, target_group);
        if((chown(filename, target_owner, target_group)) != 0){
                printf("Error doing chmod on %s (owner=%d, group=%d)\n", filename, target_owner, target_group);
                return 0;
        }

        struct stat info;
        if (stat(filename, &info) != 0) {
                printf("Error reading stat\n");
        }
        printf("File owner: %d\n", info.st_uid);
        printf("File group: %d\n", info.st_gid);
        printf("File perms: %o\n", info.st_mode);
        printf("File ctile: %d\n", info.st_ctime);

        return 0;
}
