#ifndef LZW_H_
#define LZW_H_
#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#define EMPTY			-1

#define CODE_LEN		16
#define FIRST_CODE		(1 << CHAR_BIT)

#define MAX_CODES		(1 << CODE_LEN)

#define DICT_SIZE		(MAX_CODES - FIRST_CODE)

#define CODE_MS_BITS	(CODE_LEN - CHAR_BIT)
#define MS_BITS_MASK	(UCHAR_MAX << (CHAR_BIT - CODE_MS_BITS))


/*	- mai eficient: pun indicii si valorile in array-uri separate pt ca daca am
	un int langa un char impreuna structura nu o sa ocupe 5 bytes ci 8 pentru
	aliniere
	- si mai eficient: folosesc un arbore
	http://warp.povusers.org/EfficientLZW/part4.html
	- nu e ok ca redimensionez cu puteri ale lui 2
*/

typedef struct
{
	int word;
	unsigned char byte;
	unsigned int prefix;
} dictionary_t;

int compress_lzw(char *src_path, char *dst_path);
int decompress_lzw(char *src_path, char *dst_path);

#endif
