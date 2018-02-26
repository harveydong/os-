#ifndef _SWITCH_TO
#define _SIWTCH_TO
#include "thread.h"

void switch_to(struct task_struct *cur,struct task_struct *next);
#endif
