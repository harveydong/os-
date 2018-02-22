#ifndef _BITMAP_H
#define _BITMAP_H
#define	BITMAP_MASK 1
typedef _Bool bool;

struct bitmap{

	unsigned int bitmap_bytes_len;
	unsigned char *bits;

};


void bitmap_init(struct bitmap *bmap);
int bitmap_scan(struct bitmap *bmap,unsigned int cnt);
void bitmap_set(struct bitmap *bmap,unsigned int bit_idx,char vaule);

#endif
