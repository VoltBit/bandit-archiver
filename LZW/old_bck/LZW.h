#ifndef LZW_H_
#define LZW_H_


int compress_lzw(const char *src_path, const char *dst_path);
int decompress_lzw(const char *src_path, const char *dst_path);

/*	- mai eficient: pun indicii si valorile in array-uri separate pt ca daca am
	un int langa un char impreuna structura nu o sa ocupe 5 bytes ci 8 pentru
	aliniere
	- si mai eficient: folosesc un arbore
	http://warp.povusers.org/EfficientLZW/part4.html
	- nu e ok ca redimensionez cu puteri ale lui 2
*/

typedef struct dict_entry{
	int prefix;
	char byte;
} dict_entry, *dictionary;

#endif
