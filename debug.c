#include "debug.h"
#include "interrupt.h"
#include "print.h"

void panic_spin(char *filename,int line, const char* func,const char*con)
{
	intr_disable();
	
	put_str("\n\n!!!!error!!!!\n");
	put_str("filename:");put_str(filename);put_str("\n");
	put_str("line:0x");put_int(line);put_str("\n");
	put_str("func:");put_str((char*)func);put_str("\n");
	put_str("condition:");put_str((char*)con);put_str("\n");
	while(1);	
}


