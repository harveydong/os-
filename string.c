#include "debug.h"
#include "string.h"

void memset(void * dst,unsigned char value,unsigned int size)
{
	ASSERT(dst != NULL);
	unsigned char* in_dst = (unsigned char*)dst;
	while(size-- > 0)
	{
		*in_dst++ = value;
	}
}

void memcpy(void *dst_,const void * src_,unsigned int size)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	unsigned char *dst = dst_;
	const unsigned char * src = src_;
	while(size-- > 0)
		*dst++ = *src++;

}


int memcmp(const void * a_,const void * b_,unsigned int size)
{
	const char *a = a_;
	const char *b = b_;
	ASSERT(a!= NULL || b != NULL);
	while(size-- > 0)
	{
		if(*a != *b){
			return *a > *b ?1:-1;
		}
	

		a++;
		b++;
	}


}


char * strcpy(char *dst_,const char *src_)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	char * r = dst_;
	while(*dst_++ = *src_++);
	return r;
}

unsigned int strlen(const char *str)
{
	ASSERT(str != NULL);
	const char *p = str;
	while(*p++);
	
	return (p - str - 1);
}

unsigned char strcmp(const char *a,const char *b)
{
	ASSERT(a != NULL && b != NULL);
	while(*a != 0 && *a == *b)
	{
		a++;
		b++;
	}

	return *a < *b ? -1: *a > *b;
}

char *strchr(const char * str,const unsigned char ch)
{
	ASSERT(str != NULL);
	while(*str != 0){
		if(*str == ch){
			return (char *)str;
	
		}
		str++;
	}

	return NULL;

}


char *strrchr(const char *str,const unsigned char ch)
{
	ASSERT(str != NULL);
	const char *last_char = NULL;
	while(*str != 0){
		if(*str == ch)
			last_char = str;


		str++;
	}


	return (char *)last_char;

}

char *strcat(char *dst_,const char *src_)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	char *str = dst_;
	while(*str++);
	--str;
	while(*str++ = *src_++);
	return dst_;
}


unsigned int strchrs(const char* str,unsigned char ch)
{
	ASSERT(str != NULL);
	unsigned int ch_cnt = 0;
	const char* p = str;
	while( *p != 0){
		if(*p == ch)
			ch_cnt++;
	p++;
	}

	return ch_cnt;
}



