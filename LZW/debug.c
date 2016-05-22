#include <stdlib.h>
#include <string.h>
#include "LZW.h"

int main(int argc, char *argv[])
{
	if (!strcmp(argv[1], "c") || !strcmp(argv[1], "encode"))
		compress_lzw(argv[2], argv[3]);
	if (!strcmp(argv[1], "d") || !strcmp(argv[1], "decode"))
		decompress_lzw(argv[2], argv[3]);
	return 0;
}
