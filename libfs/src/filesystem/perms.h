#ifndef _PERMS_H_
#define _PERMS_H_

#include <sys/types.h>
#include "filesystem/shared.h"

#define NGROUPS 64 // Maximum number of secondary groups to consider.

#ifdef __cplusplus
extern "C" {
#endif

enum permcheck_type {
	PC_READ,
	PC_WRITE,
	PC_EXECUTE
};

static int get_secondary_groups(uid_t uid, gid_t **buf);
static int should_group_bits_apply(uid_t uid, gid_t primary_gid, gid_t inode_gid);
static int permission_check(struct inode *inode, uid_t check_uid, gid_t check_gid, enum permcheck_type perm);
static int get_libfs_user_group(int libfs_id, uid_t *uid, gid_t *gid);

#ifdef __cplusplus
}
#endif

#endif
