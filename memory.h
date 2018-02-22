#ifndef _MEMORY_H
#define	_MEMORY_H

#include "bitmap.h"
#define	PG_P_1 1
#define	PG_P_0 0
#define	PG_RW_R 0
#define	PG_RW_W 2
#define	PG_US_S 0
#define PG_US_U 4

struct virtual_addr{
	struct bitmap vaddr_bitmap;
	unsigned int vaddr_start;	

};

enum pool_flags{
	PF_KERNEL = 1,	
	PF_USER = 2

};

void mem_init(void);
void *get_kernel_pages(unsigned int cnt);

#endif
