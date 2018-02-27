#include "interrupt.h"
#include "print.h"
#include "io.h"

#define KBD_PORT 0x60

static void intr_keyboard_handler(void)
{
	unsigned char code = inb(KBD_PORT);
	
	put_int(code);

	return;
}

void keyboard_init()
{
	put_str("keyboard init start\n");
	register_handler(0x21,intr_keyboard_handler);
	put_str("keyboard init done\n");
}


