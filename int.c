#include "print.h"
#include "io.h"
#include "interrupt.h"


#define	IDT_DESC_CNT 0x21
#define	EFLAGS_IF 0x00000200
#define	GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0":"=g"(EFLAG_VAR))



enum intr_status intr_enable(){

	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;
		return old_status;
	}else {
		old_status = INTR_OFF;
		asm volatile ("sti");
		return old_status;
	}
}

enum intr_status intr_disable(){
	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;	
		asm volatile ("cli");
		return old_status;
	}else{
		old_status = INTR_OFF;
		return old_status;
	}
}
enum intr_status intr_set_status(enum intr_status status)
{
	return status & INTR_ON ?intr_enable():intr_disable();
}

enum intr_status intr_get_status()
{
	unsigned int eflags = 0;
	GET_EFLAGS(eflags);
	return ((EFLAGS_IF & eflags) >> 9);

}


typedef void * intr_handler;

struct gate_desc {
	unsigned short int func_low_word;
	unsigned short int sel;
	unsigned char dcount;
	unsigned char attr;
	unsigned short int func_high_word;
};

static void make_idt_desc(struct gate_desc *pdesc,unsigned char attr,intr_handler func);
static struct gate_desc idt[IDT_DESC_CNT];

extern intr_handler intr_entry_table[IDT_DESC_CNT];

static void make_idt_desc(struct gate_desc *pdesc,unsigned char attr,intr_handler func)
{
	pdesc->func_low_word = (unsigned int)func & 0x0000FFFF;
	pdesc->sel = 0x08;
	pdesc->dcount = 0;
	pdesc->attr = attr;
	pdesc->func_high_word = ((unsigned int)func & 0xFFFF0000) >>16;

}



static void idt_desc_init(void)
{
	int i;	

	for(i = 0; i < IDT_DESC_CNT;i++){

		make_idt_desc(&idt[i],0x8e,intr_entry_table[i]);
		put_str("the i is:");
		put_int(i);
		put_str("\n");
		put_str("the intr entry table addr is:");
		put_int((int)intr_entry_table[i]);
		put_str("\n");
	}
	
	put_str("idt desc init done\n");

	

	put_str("idt init done\n");
}
static void pic_init(void)
{
	outb(0x20,0x11);
	outb(0x21,0x20);
	
	outb(0x21,0x04);
	outb(0x21,0x01);
	
	outb(0xa0,0x11);
	outb(0xa1,0x28);
	outb(0xa1,0x02);
	outb(0xa1,0x01);


	outb(0x21,0xfe);
	outb(0xa1,0xff);

}
void idt_init()
{
	put_str("idt init start\n");
	idt_desc_init();
	pic_init();

	unsigned long long int idt_operand = (sizeof(idt) - 1) | ((unsigned long long int)((unsigned int ) idt << 16));
	asm volatile("lidt %0"::"m"(idt_operand));	
}


