#ifndef _IO_H_
#define _IO_H_


static inline void outb(unsigned short int port,unsigned char data)
{
	asm volatile ("outb %b0,%w1"::"a"(data),"Nd"(port));

}

static inline void outsw(unsigned short int port ,const void*addr,unsigned int word_cnt)
{
	asm volatile ("cld; rep outsw":"+S"(addr),"+c"(word_cnt):"d"(port));
}


static inline unsigned char inb(unsigned short int port)
{
	unsigned char data;
	
	asm volatile("inb %w1,%b0":"=a"(data):"Nd"(port));
	return data;
}


static inline void insw(unsigned short int port,void*addr,unsigned int word_cnt)
{
	asm volatile("cld; rep insw":"+D"(addr),"+c"(word_cnt):"d"(port):"memory");
}


#endif
