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
char *intr_name[IDT_DESC_CNT];
intr_handler idt_table[IDT_DESC_CNT];


static void general_intr_handler(unsigned char vec_nr)
{
	if(vec_nr == 0x27 || vec_nr == 0x2f){
		return;
	}

	set_cursor(0);
	int cursor_pos = 0;
	while(cursor_pos < 320){
		put_char(' ');
		cursor_pos++;
	}

	set_cursor(0);
	put_str("!!!!!! excetion message begin!!!!\n");
	set_cursor(88);
	put_str(intr_name[vec_nr]);
	if(vec_nr == 14){
		int page_fault_vaddr = 0;
		asm ("movl %%cr2,%0":"=r"(page_fault_vaddr));
		put_str("\n page fault addr is ");
		put_int(page_fault_vaddr);
	}

	put_str("\n!!! excetion message end!!!!\n");

	while(1);
}

void register_handler(unsigned char vec_no,intr_handler func)
{
	idt_table[vec_no] = func;
}

static void make_idt_desc(struct gate_desc *pdesc,unsigned char attr,intr_handler func)
{
	pdesc->func_low_word = (unsigned int)func & 0x0000FFFF;
	pdesc->sel = 0x08;
	pdesc->dcount = 0;
	pdesc->attr = attr;
	pdesc->func_high_word = ((unsigned int)func & 0xFFFF0000) >>16;

}

static void exception_init(void)
{
	int i;
	for(i = 0; i < IDT_DESC_CNT;i++){
		idt_table[i] = general_intr_handler;	
		intr_name[i] = "unknown";
	}

	intr_name[0] = "#DE Divide Error";
	intr_name[1] = "#DB Debug Exception";
   	intr_name[2] = "NMI Interrupt";
   	intr_name[3] = "#BP Breakpoint Exception";
   	intr_name[4] = "#OF Overflow Exception";
   	intr_name[5] = "#BR BOUND Range Exceeded Exception";
   	intr_name[6] = "#UD Invalid Opcode Exception";
	intr_name[7] = "#NM Device Not Available Exception";
   	intr_name[8] = "#DF Double Fault Exception";
   	intr_name[9] = "Coprocessor Segment Overrun";
   	intr_name[10] = "#TS Invalid TSS Exception";
   	intr_name[11] = "#NP Segment Not Present";
   	intr_name[12] = "#SS Stack Fault Exception";
   	intr_name[13] = "#GP General Protection Exception";
   	intr_name[14] = "#PF Page-Fault Exception";
   	// intr_name[15] 第15项是intel保留项，未使用
   	intr_name[16] = "#MF x87 FPU Floating-Point Error";
   	intr_name[17] = "#AC Alignment Check Exception";
   	intr_name[18] = "#MC Machine-Check Exception";
   	intr_name[19] = "#XF SIMD Floating-Point Exception";
}

static void idt_desc_init(void)
{
	int i;	

	for(i = 0; i < IDT_DESC_CNT;i++){

		make_idt_desc(&idt[i],0x8e,intr_entry_table[i]);
	//	put_str("the i is:");
	//	put_int(i);
	//	put_str("\n");
	//	put_str("the intr entry table addr is:");
	//	put_int((int)intr_entry_table[i]);
	//	put_str("\n");
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
	exception_init();
	pic_init();

	unsigned long long int idt_operand = (sizeof(idt) - 1) | ((unsigned long long int)((unsigned int ) idt << 16));
	asm volatile("lidt %0"::"m"(idt_operand));	
}


