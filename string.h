#ifndef _STRING_H
#define	_STRING_H

#undef NULL
#if defined(__cplusplus)
#define	NULL 0 
#else
#define	NULL ((void *)0)
#endif


void memset(void *dst_,unsigned char value,unsigned int size);
void memcpy(void *dst_,const void *src_,unsigned int size);
int memcmp(const void *a_,const void *b_,unsigned int size);
char *strcpy(char *dst_,const char*src_);
unsigned int strlen(const char*str);
unsigned char strcmp(const char *a,const char*b);
char *strchr(const char*str,const unsigned char ch);
char *strrchr(const char*str,const unsigned char ch);
char *strcat(char *dst_,const char*src_);
unsigned int strchrs(const char*str,unsigned char ch);
#endif
