#ifndef _THREAD_H
#define	_THREAD_H
#include "list.h"

typedef void thread_func(void*);

enum task_status{
	TASK_RUNNING,	
	TASK_READY,
	TASK_BLOCKED,
	TASK_WAITING,
	TASK_HANGING,
	TASK_DIED
};

struct intr_stack{
	unsigned int vec_no;
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp_dummy;
	unsigned int ebx;
	unsigned int edx;	
	unsigned int ecx;
	unsigned int eax;
	unsigned int gs;
	unsigned int fs;
	unsigned int es;
	unsigned int ds;
/*cpu from low priviledge to high priviledge, cpu auto push below content to stack*/	
	unsigned int err_code;
	void (*eip)(void);
	unsigned int cs;
	unsigned int eflags;
	void *esp;
	unsigned int ss;	
};

struct thread_stack {

	unsigned int ebp;
	unsigned int ebx;
	unsigned int edi;
	unsigned int esi;
	
	void (*eip)(thread_func * func,void *func_arg);
	void (*unused_retaddr);
	thread_func *function;
	void *func_arg;
};

struct task_struct {
	unsigned int * self_kstack;
	enum task_status status;
	unsigned char priority;
	
	char name[16];

	unsigned char ticks;
	unsigned int elapsed_ticks;
	struct list_elem general_tag;
	struct list_elem all_list_tag;

	unsigned int *pgdir;
	

	unsigned int stack_magic;

};

struct task_struct *thread_start(char *name,int prio,thread_func function,void*func_arg);
void thread_init(void);
struct task_struct *running_thread();
void schedule();
#endif
