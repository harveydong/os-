#include "print.h"
#include "io.h"
#include "thread.h"
#include "debug.h"
#include "interrupt.h"


#define	IRQ0_FREQUENCY 100
#define	INPUT_FREQUENCY 1193180
#define	COUNTER0_VALUE INPUT_FREQUENCY / IRQ0_FREQUENCY
unsigned int ticks;

static void intr_timer_handler(void)
{
	struct task_struct *cur_thread = running_thread();
	ASSERT(cur_thread->stack_magic == 0x19890223);
	
	cur_thread->elapsed_ticks++;
	ticks++;
//	put_str("into intr timer handler\n");
	if(cur_thread->ticks == 0)
	{
		schedule();
	}else{
		cur_thread->ticks--;
	}

	
}

static void frequency_set(unsigned char counter_port,unsigned char counter_no,unsigned rwl,unsigned char counter_mode \
,unsigned short int counter_value)
{
	outb(0x43,(unsigned char)(counter_no << 6|rwl << 4|counter_mode << 1));
	outb(counter_port,(unsigned char)counter_value);
	outb(counter_port,(unsigned char)counter_value >> 8);
	
}

void timer_init()
{
	put_str("timer_init start\n");
	frequency_set(0x40,0,3,2,COUNTER0_VALUE);

	register_handler(0x20,intr_timer_handler);
	put_str("timer init done\n");
}


