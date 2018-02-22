#include "print.h"
#include "init.h"
#include "debug.h"
#include "memory.h"

int main(void)
{
	put_str("hello world,I am a linux lover by dhw\n");
//	put_int(10);
//	put_char(0xd);
//	put_int(110);
//	put_char(0xd);
//	put_int(0x12345);
	put_str("\n");
	init_all();
//	asm volatile("sti");

	void *addr = get_kernel_pages(3);
	put_str("\n get kernle pages start vaddr is ");
	put_int((unsigned int)addr);
	put_str("\n");
//	ASSERT(1==2);
	
	while(1);
	return 0;
}
