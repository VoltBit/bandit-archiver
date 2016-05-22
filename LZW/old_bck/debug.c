#include <stdlib.h>
#include <stdio.h>
#include "LZW.h"

int main(void)
{
	printf("Debug mode\n____________________________________\n");
	compress_lzw("../test_file", "../test_file_out");
	return 0;
}