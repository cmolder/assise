#ifndef _LEASE_PROT_H_
#define _LEASE_PROT_H_

#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "distributed/replication.h"
#include "concurrency/synchronization.h"
#include "filesystem/shared.h"
#include "global/global.h"
#include "global/types.h"
#include "global/util.h"
#include "ds/list.h"
#include "ds/stdatomic.h"
#include "io/block_io.h"

//#include "filesystem/fs.h"

typedef struct lease_info {
	int dev;
	uint32_t inum;
	uint8_t state;

	uint16_t errcode; //LibFS only; indicates whether lease RPC returned an error;

	//uint8_t dirty[g_n_max_libfs];	// Lease state for inode (read, write, digest_to_acquire, free)


	uint8_t holders;	// Number of LibFSes holding a lease (only read leases allow multiple holders)

	struct timespec time;	// Time when lease was last acquired
	uint16_t lversion;	// Log version when exclusive lease was released
	uint64_t lblock;	// Log block position when exclusive lease was released

	pthread_spinlock_t mutex;

	uint8_t hid;		// LibFS ID for last lease holder
	uint8_t mid;		// KernFS ID for lease manager
	mlfs_hash_t hash_handle;

} mlfs_lease_t;

//registered memory region identifiers
enum lease_type {
	LEASE_FREE = 0,
	LEASE_READ,
	LEASE_WRITE,
};


typedef struct SharedGuts {
	void *base;
	size_t size;
	pthread_spinlock_t mutex;
	char *next_allocation;
	mlfs_lease_t *hash;
} SharedGuts;

typedef struct SharedTable {
	int fd;
	void *base;
	size_t size;
	struct SharedGuts *guts;  // actually points into the shared memory segment
} SharedTable;

typedef struct ParsedId {
	int uid;
	int gid;
} ParsedId;

extern SharedTable *lease_table;

static SharedGuts *SharedGuts_create(void *base, size_t size);
static SharedGuts *SharedGuts_mock();
SharedTable *SharedTable_create(const char *name, size_t arena_size);
SharedTable *SharedTable_mock();
SharedTable *SharedTable_subscribe(const char *name);
void SharedTable_unsubscribe(SharedTable *t);
static void *SharedTable_malloc(SharedTable *t, size_t size);
static void SharedTable_free(SharedTable *t, void *p);



#if MLFS_PERMISSIONS

enum permcheck_type {
	PC_READ,
	PC_WRITE,
	PC_EXECUTE
};

static int permission_check(struct inode *inode, uid_t check_uid, gid_t check_gid, enum permcheck_type perm)
{
  if (check_uid == 0 && perm != PC_EXECUTE) {
    return 1;
  }
  mlfs_debug("ckuid %d ckgid %d iuid %d igid %d imode %o\n", check_uid, check_gid, inode->uid, inode->gid, inode->perms);
  if (inode->uid == check_uid) {
    switch (perm) {
    case PC_READ: return (inode->perms & S_IRUSR) != 0;
    case PC_WRITE: return (inode->perms & S_IWUSR) != 0;
    // case PC_EXECUTE: return (inode->perms & S_IXUSR) != 0;
    }
//   } else if (should_group_bits_apply(check_gid, inode->gid)) {
  } else if (check_gid == inode->gid) {
	switch (perm) {
    case PC_READ: return (inode->perms & S_IRGRP) != 0;
    case PC_WRITE: return (inode->perms & S_IWGRP) != 0;
    // case PC_EXECUTE: return (inode->perms & S_IXGRP) != 0;
    }
  } else {
    switch (perm) {
    case PC_READ: return (inode->perms & S_IROTH) != 0;
    case PC_WRITE: return (inode->perms & S_IWOTH) != 0;
    // case PC_EXECUTE: return (inode->perms & S_IXOTH) != 0;
    }
  }
  return 0;
}

#endif

#if 0
static inline int lease_downgrade(int request, int current)
{
	assert(request != LEASE_NONE && current != LEASE_NONE);

	if(request == LEASE_WRITE) {
		//assert(current == LEASE_WRITE);
		return LEASE_NONE;
	}
	else if(request == LEASE_READ && current == LEASE_WRITE) {
		return LEASE_READ;
	}
	else
		return 0;
}

static inline int lease_valid(int request, int current)
{
	assert(request != LEASE_NONE);

	if(request >= current) {
		return 1;
	}
	else
		return 0;
}
#endif

int init_lease();


void lupdate(mlfs_lease_t *ls);
int read_ondisk_lease(uint32_t inum, mlfs_lease_t *ls);
int write_ondisk_lease(mlfs_lease_t *ls);
void update_remote_ondisk_lease(uint8_t node_id, mlfs_lease_t *ls);
int acquire_family_lease(uint32_t inum, int type, char *path);
int acquire_parent_lease(uint32_t inum, int type, char *path);
int acquire_lease(uint32_t inum, int type, char *path);
int update_lease_manager(uint32_t inum, uint32_t new_kernfs_id);
int mark_lease_revocable(uint32_t inum);
int revoke_lease(int sockfd, uint32_t seq_n, uint32_t inum);
int report_lease_error(uint32_t inum);
int clear_lease_checkpoints(int req_id, int version, addr_t log_block);
int discard_leases();
void shutdown_lease_protocol();
ParsedId parse_uid_gid(int req_id);

//void wait_on_replicating();

int purge_dir_caches(uint32_t inum);

int modify_lease_state(int libfs_id, int inum, int req_type, int log_version, addr_t log_block);

int resolve_lease_conflict(int sockfd, char *path, int type, uint32_t seqn);

#endif
