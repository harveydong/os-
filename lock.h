#ifndef _LOCK_H
#define _LOCK_H
#include "list.h"
#include "thread.h"

struct semaphore {
	unsigned char value;
	struct list waiters;

};

struct lock {
	struct task_struct *holder;
	struct semaphore semaphore;
	unsigned int holder_repeat_nr;

};

void lock_acquire(struct lock*);
void lock_release(struct lock *);
void lock_init(struct lock *);
#endif
