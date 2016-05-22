/*
http://michael.dipperstein.com/lzw/
http://warp.povusers.org/EfficientLZW/part2.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LZW.h"
#include "file_manager.h"

#define	INIT_SIZE		256
#define EMPTY	-1
#define BEST_VAL	345

static dictionary dict;
static int dict_size;
static int current_index;
static int two_pow;

static void display_dictionary(void)
{
	int i;

	printf("\nDictionary:\n");
	for (i = 256; i < current_index; ++i) {
		if (dict[i].prefix == EMPTY)
			printf("[EMPTY]->%c\n", dict[i].byte);
		else
			printf("[%i]->%c\n", dict[i].prefix, dict[i].byte);
	}
}

static dictionary make_dictionary(void)
{
	int i;

	dict = malloc(sizeof(dict_entry) * INIT_SIZE);
	for (i = 0; i < INIT_SIZE; ++i) {
		dict[i].prefix = EMPTY;
		dict[i].byte = (char)i;
	}
	dict_size = INIT_SIZE;
	current_index = INIT_SIZE;
	two_pow = 8;
	printf("Made: %i\n", dict_size);
	return dict;
}

static void destroy_dictionary(void)
{
	int i;

	printf("Destroying: %i\n", dict_size);
	free(dict);
	dict = NULL;
}

static void *resize_dictionary(os_size new_size)
{
	void *ret;
	ret = realloc(dict, new_size);
	return ret;
}

static int search_string(int prefix, char byte)
{
	int i;
	// printf("current_index: %lu\n", current_index);
	// for (i = current_index; i >= 0; --i) {
	for (i = 256; i < current_index; ++i) {
		// printf("%lu vs %lu\t%c vs %c", dict[i].prefix, prefix, dict[i].byte, byte);
		if (dict[i].prefix == prefix && dict[i].byte == byte) {
			return i;
		}
	}
	return EMPTY;
}

static int add_string(int prefix, char byte)
{
	++current_index;
	if (current_index >= dict_size) {
		++two_pow;
		dict = resize_dictionary(((1 << two_pow) - 1) * sizeof(dict_entry));
		if (dict == NULL) {
			perror("reize failed");
			exit(-1);
		}
		dict_size = (1 << two_pow) - 1;
	}
	dict[current_index].prefix = prefix;
	dict[current_index].byte = byte;
	return current_index;
}

int compress_lzw(const char *src_path, const char *dst_path)
{
	handle_t src_file, dst_file;
	void *dict_ret;
	unsigned char current_byte, next_byte;
	os_size bytes_read;
	int next_code, current_code, index;

	src_file = open_file(src_path, SOURCE);
	dst_file = open_file(dst_path, DESTINATION);
	dict_ret = make_dictionary();

	bytes_read = read_file(src_file, &current_byte, 1);
	current_code = current_byte;

	while (1) {
		bytes_read = read_file(src_file, &next_byte, 1);
		if (!bytes_read)
			break;
		index = search_string(current_code, next_byte);
		if (index != EMPTY) {
			current_code = index;
		} else {
			printf("%i ", current_code);
			write_file(dst_file, &current_code, sizeof(int));
			next_code = add_string(current_code, next_byte);
			current_code = next_byte;
		}
		current_byte = next_byte;
	}
	printf("\n");
	// display_dictionary();
	// printf("Compressing file %s...\n", src_path);
	// printf("Done! File written at %s\n", dst_path);

	destroy_dictionary();
	close_file(src_file);
	close_file(dst_file);
	return 0;
}

static char get_byte(int word)
{
	int i;

	if (word != EMPTY) {
		get_byte(dict[word].prefix);
		printf("%c", dict[word].byte);
	} else {
		printf("%c", word);
		return word;
	}
	return EMPTY;
}

int decompress_lzw(const char *src_path, const char *dst_path)
{
	handle_t src_file, dst_file;
	int it = 0;
	void *dict_ret;
	char current_word[BEST_VAL];
	unsigned char current_byte, next_word;
	os_size bytes_read;
	int next_code, current_code, index;

	src_file = open_file(src_path, SOURCE);
	dst_file = open_file(dst_path, DESTINATION);
	dict_ret = make_dictionary();
	printf("\n\n[LZW] Started decompression\n");

	bytes_read = read_file(src_file, &current_byte, sizeof(int));
	if (bytes_read) {
		current_code = current_byte;
		printf("%C", dict[current_code].byte);
		while (1) {
			bytes_read = read_file(src_file, &next_word, sizeof(int));
			if (!bytes_read)
				break;
			fflush(0);
			if (next_word < 255 && next_word > 0) {
				// printf("Test\n");
				current_byte = dict[next_word].byte;
			}
			else {
				// printf("Test2\n");
				current_byte = get_byte(next_word);
			}
			printf("%c", current_byte);
			// if (current_byte != EMPTY)
			// 	printf("decoded: %c\n", current_byte);
			// else
			// 	printf("\n>>>nasol<<<\n");
			// printf("decoded: %c\n", ((dictionary)dict_ret)[current_code]);
		}
	}
	printf("Decompression done!\n");
	close_file(src_file);
	close_file(dst_file);
	return 0;
}
