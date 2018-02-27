#include "print.h"

#include "lock.h"

static struct lock console_lock;

void console_init()
{
	lock_init(&console_lock);
	put_str("console init done\n");
}


void console_acquire()
{
	lock_acquire(&console_lock);
}


void console_release()
{
	lock_release(&console_lock);
}

void console_put_str(char *str)
{
	console_acquire();
	put_str(str);
	console_release();
}


void console_put_char(unsigned char ch)
{
	console_acquire();
	put_char(ch);
	console_release();
}

void console_put_int(unsigned int num)
{
	console_acquire();
	put_int(num);
	console_release();
}



