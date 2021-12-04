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

int get_secondary_groups(uid_t uid, gid_t **groups, int *ngroups);
int should_group_bits_apply(uid_t uid, gid_t primary_gid, gid_t inode_gid);
int violates_sticky_bit(uid_t uid, uid_t parent_uid, uid_t target_uid, uint16_t parent_perms);
int permission_check(uid_t inode_uid, gid_t inode_gid, uid_t check_uid, gid_t check_gid, uint16_t perms, enum permcheck_type check);
int parse_uid_gid(int libfs_id, uid_t *uid, gid_t *gid);
int check_owner(uid_t uid, uid_t inode_uid);
int check_root(uid_t uid);
int check_group_membership(uid_t uid, gid_t target_gid);


#ifdef __cplusplus
}
#endif

#endif
