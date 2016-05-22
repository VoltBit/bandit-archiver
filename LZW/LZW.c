#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "LZW.h"
#include "bitfile.h"

#define BUF_SIZE	32

static dictionary_t dictionary[DICT_SIZE];
static int encr = 1;

int get_word(bit_file_t *src_file)
{
	unsigned char byte;
	int code;

	if ((code = BitFileGetChar(src_file)) == EOF) {
	/*printf("received: %c\n", byte);*/
		return EOF;
	}
	/*printf("received: %c\n", byte);*/
	if (BitFileGetBits(src_file, &byte, CODE_MS_BITS) == EOF) {
		return EOF;
	}
	code |= ((int)byte) << CODE_MS_BITS;
	return code;
}

void write_word(int code, bit_file_t *dst_file)
{
	unsigned char byte;

	byte = code & 0xFF;
	BitFilePutChar(byte, dst_file);
	byte = (code >> CODE_MS_BITS) & MS_BITS_MASK;
	BitFilePutBits(dst_file, &byte, CODE_MS_BITS);
}

unsigned char decode_word(unsigned int code, FILE *dst_file)
{
	unsigned char byte;
	unsigned char firstChar;

	if (code >= FIRST_CODE) {
		byte = dictionary[code - FIRST_CODE].byte;
		code = dictionary[code - FIRST_CODE].prefix;
		firstChar = decode_word(code, dst_file);
	} else {
		byte = code;
		firstChar = code;
	}

	fputc(byte, dst_file);
	return firstChar;
}

int search_string(unsigned int prefix, unsigned char c)
{
	int index = 0;
	int key;

	key = (prefix << CHAR_BIT) | c;
	key %= DICT_SIZE;
	index = key;
	do {
		if ((dictionary[index].prefix == prefix) &&
			(dictionary[index].byte == c)) {
			return index;
		}
		if (EMPTY == dictionary[index].word) {
			return index;
		}
		index++;
		index %= DICT_SIZE;

	} while (key != index);
	return index;
}

int compress_lzw(char *src_path, char *dst_path)
{
	FILE *src_file, *dst_file;
	bit_file_t *bit_dst_file;

	int code;
	int nextCode;
	int byte;
	int index;
	char rbyte;

	if (NULL == (src_file = fopen(src_path, "r"))) {
		perror(src_path);
		return FALSE;
	}

	if (NULL == (dst_file = fopen(dst_path, "wb"))) {
		perror(dst_path);
		return FALSE;
	}

	for (index = 0; index < DICT_SIZE; index++) {
		dictionary[index].word = EMPTY;
	}

	nextCode = FIRST_CODE;
	code = fgetc(src_file);
	// fprintf(dst_file, "%c", code);
	rbyte = code;
	do {
		byte = rbyte;
		if (encr) {
			fprintf(dst_file, "%c", rbyte);
		} else {
			index = search_string(code, byte);
			if ((dictionary[index].word != EMPTY) &&
				(dictionary[index].prefix == (unsigned)code) &&
				(dictionary[index].byte == byte)) {
				code = dictionary[index].word;
			} else {
				if (nextCode < MAX_CODES) {
					dictionary[index].word = nextCode;
					dictionary[index].prefix = code;
					dictionary[index].byte = byte;
					nextCode++;
				}
				write_word(code, bit_dst_file);
				code = byte;
			}
		}
		rbyte = fgetc(src_file);
	} while (!feof(src_file));

	// write_word(code, bit_dst_file);
	fclose(src_file);
	fclose(dst_file);
	// BitFileClose(bit_dst_file);

	return TRUE;
}

int decompress_lzw(char *src_path, char *dst_path)
{
	// bit_file_t *src_file;
	// FILE *dst_file;

	// unsigned int nextCode;
	// unsigned int lastCode;
	// unsigned int code;
	// unsigned char byte;

	// if (dst_path == NULL) {
	// 	dst_file = stdout;
	// } else  if (!encr) {
	// 	if (!(dst_file = fopen(dst_path, "wb"))) {
	// 		perror(dst_path);
	// 		return FALSE;
	// 	}
	// } else {
	// 	return 0;
	// }
	// nextCode = FIRST_CODE;

	// lastCode = get_word(src_file);
	// byte = lastCode;
	// fputc(lastCode, dst_file);

	// while ((code = get_word(src_file)) != EOF) {
	// 	if (code < nextCode) {
	// 		byte = decode_word(code, dst_file);
	// 	} else {
	// 		unsigned char tmp;
	// 		tmp = byte;
	// 		byte = decode_word(lastCode, dst_file);
	// 		fputc(tmp, dst_file);
	// 	}

	// 	if (nextCode < MAX_CODES) {
	// 		dictionary[nextCode - FIRST_CODE].prefix = lastCode;
	// 		dictionary[nextCode - FIRST_CODE].byte = byte;
	// 		nextCode++;
	// 	}

	// 	lastCode = code;
	// }

	// fclose(dst_file);
	// BitFileClose(src_file);
	return TRUE;
}
