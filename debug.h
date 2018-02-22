#ifndef _DEBUG_H
#define _DEBUG_H
#include "print.h"
#include "interrupt.h"

void panic_spin(char *filename,int line, const char* func,const char*con);

#define PANIC(...) panic_spin(__FILE__,__LINE__,__func__,__VA_ARGS__);



#ifdef NDEBUG
#define ASSERT(CON) ((void)0)
#else
#define	ASSERT(CON) \
		if(CON){  \
		}else{    \
			PANIC(#CON)		\
		} 

#endif


#endif
