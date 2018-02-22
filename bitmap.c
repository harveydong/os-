#include "string.h"
#include "bitmap.h"
#include "debug.h"

void bitmap_init(struct bitmap *bmap)
{
	memset(bmap->bits,0,bmap->bitmap_bytes_len);

}

bool bitmap_scan_test(struct bitmap* bmap,unsigned int bit_idx)
{
	unsigned int byte_idx = bit_idx  / 8;
	unsigned int bit_odd = bit_idx % 8;
	return (bmap->bits[byte_idx] & (BITMAP_MASK << bit_odd));
}

int bitmap_scan(struct bitmap *bmap,unsigned int cnt)
{
	unsigned int idx_byte = 0;
	while((0xff == bmap->bits[idx_byte]) && (idx_byte < bmap->bitmap_bytes_len)){
		idx_byte++;
	}

	ASSERT(idx_byte < bmap->bitmap_bytes_len);
	if(idx_byte == bmap->bitmap_bytes_len){
		return -1;
	}
	int idx_bit = 0;
	while((unsigned char)(BITMAP_MASK << idx_bit) & bmap->bits[idx_byte]){
		idx_bit++;
	}

	int bit_idx_start = idx_byte * 8 + idx_bit;
	if(cnt == 1){
		return bit_idx_start;
	}	
	unsigned int bit_left = (bmap->bitmap_bytes_len * 8 - bit_idx_start);
	unsigned int next_bit = bit_idx_start + 1;
	unsigned int count = 1;
	
	bit_idx_start = -1;
	while(bit_left-- > 0){
		if(!(bitmap_scan_test(bmap,next_bit))){
			count++;
		}else{
			count  = 0;
		}
		

		if(count == cnt){
			bit_idx_start = next_bit - cnt + 1;
			break;
		}

		next_bit++;
	}

	return bit_idx_start;
}



void bitmap_set(struct bitmap *bmap,unsigned int bit_idx,char value)
{
	ASSERT(value == 0 || value == 1);
	unsigned int byte_idx = bit_idx / 8;
	unsigned int bit_odd = bit_idx % 8;
	
	if(value){
		bmap->bits[byte_idx] |= (BITMAP_MASK << bit_odd);
	}else{
		bmap->bits[byte_idx] &= ~(BITMAP_MASK << bit_odd);
	}
}
