#include "print.h"
#include "init.h"
#include "debug.h"
#include "memory.h"
#include "thread.h"
#include "interrupt.h"
void k_thread_a(void*);
void k_thread_b(void*);

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

//	void *addr = get_kernel_pages(3);
//	put_str("\n get kernle pages start vaddr is ");
//	put_int((unsigned int)addr);
//	put_str("\n");
//	ASSERT(1==2);

	thread_start("k_thread_a",10,k_thread_a,"arga ");		
	thread_start("k_thread_b",5,k_thread_b,"argb ");	
//	put_str("ok main 1 \n");
	intr_enable();	
	put_str("ok here main\n");
	while(1)
	{
		intr_disable();
		put_str("main ");
		intr_enable();
	}
	return 0;
}

void k_thread_a(void *arg)
{

	char *para = arg;
	
	while(1){
		intr_disable();
		put_str(para);
		intr_enable();
		
	}
}


void k_thread_b(void *arg)
{

	char *para = arg;
	
	while(1){
		intr_disable();
		put_str(para);
		intr_enable();
		
	}
}
