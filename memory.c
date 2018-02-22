#include "print.h"
#include "bitmap.h"
#include "memory.h"
#include "string.h"
#include "debug.h"

#define PG_SIZE 4096

/*0xc0009f00 is kernel thread stack top, 0xc0009e00 is kernel main thread pcb*/

#define MEM_BITMAP_BASE 0xc0009a00 /*contain of 0xc0009a00,0xc0009b00,0xc0009c00,0xc0009d00*/


#define K_HEAP_START 0xc0100000


#define	PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define	PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

struct pool {
	struct bitmap pool_bitmap;
	unsigned int phy_addr_start;
	unsigned int pool_size;

};

struct pool kernel_pool,user_pool;
struct virtual_addr kernel_vaddr;

static void * vaddr_get(enum pool_flags pf, unsigned int pg_cnt)
{
	int vaddr_start = 0,bit_idx_start = -1;
	unsigned int cnt = 0;
	
	if(pf == PF_KERNEL){
		bit_idx_start = bitmap_scan(&kernel_vaddr.vaddr_bitmap,pg_cnt);
		if(bit_idx_start == -1){
			return NULL;
		}
		while(cnt < pg_cnt){
			bitmap_set(&kernel_vaddr.vaddr_bitmap,bit_idx_start + cnt++,1);
			
		}

		vaddr_start = kernel_vaddr.vaddr_start + bit_idx_start * PG_SIZE;
	}else{

	}

	return (void *)vaddr_start;
}

unsigned int *pte_ptr(unsigned int vaddr)
{
	unsigned int *pte = (unsigned int*)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + PTE_IDX(vaddr) * 4);
	
	return pte;
}

unsigned int *pde_ptr(unsigned int vaddr)
{
	unsigned int *pde = (unsigned int*)((0xfffff000) + PDE_IDX(vaddr) * 4);
	return pde;
}

static void *palloc(struct pool * m_pool)
{
	int bit_idx = bitmap_scan(&m_pool->pool_bitmap,1);
	if(bit_idx == -1){
		return NULL;
	}

	bitmap_set(&m_pool->pool_bitmap,bit_idx,1);
	unsigned int page_phyaddr = ((bit_idx * PG_SIZE) + m_pool->phy_addr_start);
	return (void *)page_phyaddr;

}

static void page_table_add(void * _vaddr,void *_page_phyaddr)
{
	unsigned int vaddr = (unsigned int)_vaddr,page_phyaddr = (unsigned int)_page_phyaddr;
	unsigned int *pde = pde_ptr(vaddr);
	unsigned int *pte = pte_ptr(vaddr);

	if(*pde & 0x00000001){
		
		ASSERT(!(*pte & 0x00000001));
		if(!(*pte * 0x00000001)){
			*pte = (page_phyaddr | PG_US_U|PG_RW_W|PG_P_1);
		}else{
			PANIC("pte repeat");
			*pte = (page_phyaddr | PG_US_U|PG_RW_W|PG_P_1);
		}
	}else{
		unsigned int pde_phyaddr = (unsigned int)palloc(&kernel_pool);
		*pde = (pde_phyaddr | PG_US_U| PG_RW_W|PG_P_1);
		
		memset((void *)((int)pte & 0xfffff000),0,PG_SIZE);
		
		ASSERT(!(*pte & 0x00000001));
		*pte = (page_phyaddr|PG_US_U|PG_RW_W|PG_P_1);

	
	}
}

void * malloc_page(enum pool_flags pf,unsigned int pg_cnt)
{
	ASSERT(pg_cnt > 0 && pg_cnt < 3840);
	
	void *vaddr_start = vaddr_get(pf,pg_cnt);
	if(vaddr_start == NULL){
		
		return NULL;
	
	}

	unsigned int vaddr = (unsigned int)vaddr_start,cnt = pg_cnt;
	struct pool *mem_pool = pf & PF_KERNEL ? &kernel_pool:&user_pool;
	
	while(cnt-- > 0)
	{
		void *page_phyaddr = palloc(mem_pool);
		if(page_phyaddr == NULL)
		{
			put_str("\n page phyaddr is NULL\n");
			return NULL;
		}
		page_table_add((void *)vaddr,page_phyaddr);
		vaddr += PG_SIZE;
	}


	return vaddr_start;
}

void *get_kernel_pages(unsigned int pg_cnt)
{
	void *vaddr = malloc_page(PF_KERNEL,pg_cnt);
	if(vaddr != NULL)
	{
		memset(vaddr,0,pg_cnt * PG_SIZE);
	}	
	put_str("get kernle page vaddr: ");
	put_int((unsigned int)vaddr);
	put_str("\n");	
	return vaddr;
}


static void mem_pool_init(unsigned int all_mem)
{
	put_str("mem pool init start\n");
	unsigned int page_table_size = PG_SIZE * 256;
	unsigned int used_mem = page_table_size + 0x100000;
	unsigned int free_mem = all_mem - used_mem;
	unsigned short int all_free_pages = free_mem / PG_SIZE;
	unsigned short int kernel_free_pages = 	all_free_pages / 2;	
	unsigned short int user_free_pages = all_free_pages - kernel_free_pages;
	unsigned int kbm_length = kernel_free_pages / 8;
	unsigned int ubm_length = user_free_pages / 8;
	unsigned int kp_start = used_mem;
	unsigned int up_start = kp_start + kernel_free_pages * PG_SIZE;
	
	kernel_pool.phy_addr_start = kp_start;
	user_pool.phy_addr_start = up_start;

	kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
	user_pool.pool_size = user_free_pages * PG_SIZE;
	
	kernel_pool.pool_bitmap.bitmap_bytes_len = kbm_length;
	user_pool.pool_bitmap.bitmap_bytes_len = ubm_length;
	
	kernel_pool.pool_bitmap.bits = (void *)MEM_BITMAP_BASE;
	user_pool.pool_bitmap.bits = (void *)(MEM_BITMAP_BASE + kbm_length);
	
	put_str("kernel pool bitmap start: ");
	put_int((int)kernel_pool.pool_bitmap.bits);
	put_str("\nkernel pool phy addr start:");
	put_int(kernel_pool.phy_addr_start);
	put_str("\n");
	put_str("user pool bitmap start:");
	put_int((int)user_pool.pool_bitmap.bits);	
	put_str("\nuser pool phy addr start:");
	put_int(user_pool.phy_addr_start);
	put_str("\n");
	bitmap_init(&kernel_pool.pool_bitmap);
	put_str("kernel pool bitmap size: ");
	put_int(kernel_pool.pool_bitmap.bitmap_bytes_len);
	put_str("\n");
	bitmap_init(&user_pool.pool_bitmap);
	
	kernel_vaddr.vaddr_bitmap.bitmap_bytes_len = kbm_length;
	kernel_vaddr.vaddr_bitmap.bits = (void*)(MEM_BITMAP_BASE + kbm_length + ubm_length);
	
	kernel_vaddr.vaddr_start = K_HEAP_START;
	bitmap_init(&kernel_vaddr.vaddr_bitmap);
	put_str("\nmempool init done\n");
	
}


void mem_init()
{
	put_str("mem init start\n");
	unsigned int mem_bytes_total = 32*1024*1024;
	mem_pool_init(mem_bytes_total);
	put_str("mem init done\n");
}


