#include "global/perms.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

#include "filesystem/shared.h"
#include "distributed/peer.h"

/* Following setup in https://linux.die.net/man/3/getgrouplist */
int get_secondary_groups(uid_t uid, gid_t **buf, int *buflen) {
    struct passwd *pw;

	buf = malloc(NGROUPS * sizeof(gid_t));
	if (!buf) {
		mlfs_debug("%s", "[DEBUG] Could not allocate groups array\n");
		return -1;
	} 

	// Fetch passwd structure
	pw = getpwuid(uid);
	if (!pw) {
		mlfs_debug("%s", "[DEBUG] Could not get passwd struct\n");
		return -1;
	}

	if (getgrouplist(pw->pw_name, pw->pw_gid, *buf, buflen) == -1) {
		mlfs_debug("%s", "[DEBUG] Could not get group list\n");
		return -1;
	}

	return 0;
}

int should_group_bits_apply(uid_t uid, gid_t primary_gid, gid_t inode_gid) {
	if (primary_gid == inode_gid)
		return 1;

	int secondary_grp_count;
	gid_t *secondary_grp_list;
	if (get_secondary_groups(uid, &secondary_grp_list, &secondary_grp_count) != 0) {
		/* XXX: Swallowing error and failing */
		mlfs_debug("[DEBUG] get_secondary_groups failed for uid %d, primary_gid %d, inode_gid %d", uid, primary_gid, inode_gid);
		mlfs_free(secondary_grp_list);
		return 0;
	}

	for (int i = 0; i < secondary_grp_count; i++) {
		if (secondary_grp_list[i] == inode_gid) {
			return 1;
		}
	}
	return 0;
}

int permission_check(uid_t inode_uid, gid_t inode_gid, uid_t check_uid, gid_t check_gid, uint16_t perms, enum permcheck_type check)
{
	mlfs_printf("Checking %d perms for uid %d, gid %d, perm %d against inode uid %d, gid %d, mode %o\n", 
				check, check_uid, check_gid, check, inode_uid, inode_gid, perms);

	if (check_uid == 0 && check != PC_EXECUTE)
		return 1;

  	if (inode_uid == check_uid) {
    	switch (check) {
			case PC_READ: return (perms & S_IRUSR) != 0;
			case PC_WRITE: return (perms & S_IWUSR) != 0;
			case PC_EXECUTE: return (perms & S_IXUSR) != 0;
		}
  	// } else if (should_group_bits_apply(check_uid, check_gid, inode_gid)) {
  	} else if (check_gid == inode_gid) {
    	switch (check) {
			case PC_READ: return (perms & S_IRGRP) != 0;
			case PC_WRITE: return (perms & S_IWGRP) != 0;
			case PC_EXECUTE: return (perms & S_IXGRP) != 0;
    	}
  	} else {
    	switch (check) {
			case PC_READ: return (perms & S_IROTH) != 0;
			case PC_WRITE: return (perms & S_IWOTH) != 0;
			case PC_EXECUTE: return (perms & S_IXOTH) != 0;
    	}
  	}
	return 0;
}

/* Get effective uid/gid of requesting LibFS ID. */
int parse_uid_gid(int req_id, uid_t *uid, gid_t *gid) {
	pid_t pid = g_peers[req_id]->pid;
	int MAX_PID_PATH = 100;
	int MAX_BUF = 100;
	char path[MAX_PID_PATH];
    char buf_read[MAX_BUF];

    snprintf(path, MAX_PID_PATH, "/proc/%d/status", pid);
    FILE *status = fopen(path, "r");

	uid_t ruid;
    uid_t euid;
    uid_t suid;
    uid_t fuid;

    gid_t rgid;
    gid_t egid;
    gid_t sgid;
    gid_t fgid;

    // UID is line 8, GUID is line 9
    int items = 0;
    char *end;
    while (items < 1 && end != NULL) {
        end = fgets(buf_read, MAX_BUF, status);
        items = sscanf(buf_read, "Uid: %d %d %d %d", &ruid, &euid, &suid, &fuid);
    }
    fscanf(status, "Gid: %d %d %d %d\n", &rgid, &egid, &sgid, &fgid);
	fclose(status);

	*uid = euid;
	*gid = egid;
	return 0;
}
