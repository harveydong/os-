#ifndef _INTERRUPT_H_
#define	_INTERRUPT_H_
enum intr_status{
	INTR_OFF,
	INTR_ON
};
typedef void *intr_handler;

enum intr_status intr_get_status(void);
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);
void idt_init();
void register_handler(unsigned char vec_no,intr_handler func);

#endif
