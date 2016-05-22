#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CompressionLib.h"
#include "LZW.h"


JNIEXPORT void JNICALL Java_CompressionLib_compress (
	JNIEnv *env, jobject obj, jstring type, jstring src_path, jstring dest_path)
{
	int ret;
	char *alg_type = (*env)->GetStringUTFChars(env, type, NULL);
	char *src_file = (*env)->GetStringUTFChars(env, src_path, NULL);
	char *dst_file = (*env)->GetStringUTFChars(env, dest_path, NULL);
	printf("Starting compression algorithm: %s for file %s\n", alg_type,
		src_file);
	if (strcmp(alg_type, "LZW") == 0) {
		ret = compress_lzw(src_file, dst_file);
		if (ret < 0) {
			printf("Compression failed\n");
		}
	} else if (strcmp(alg_type, "huffman") == 0) {
		printf("Huuuuuufffmaaaan\n");
		/*
		compress_huff()
		*/
	}
}

JNIEXPORT void JNICALL Java_CompressionLib_decompress
  (JNIEnv *env, jobject obj, jstring type, jstring src_path, jstring dest_path)
{
	int ret;
	char *alg_type = (*env)->GetStringUTFChars(env, type, NULL);
	char *src_file = (*env)->GetStringUTFChars(env, src_path, NULL);
	char *dst_file = (*env)->GetStringUTFChars(env, dest_path, NULL);
	printf("Starting decompression algorithm: %s for file %s\n", alg_type,
		src_file);
	if (strcmp(alg_type, "LZW") == 0) {
		ret = decompress_lzw(src_file, dst_file);
		if (ret < 0) {
			printf("Decompression failed\n");
		}
	} else if (strcmp(alg_type, "huffman") == 0) {
		printf("Huffman rulzeza fin\n");
		/*
		decompress_huff()
		*/
	}
}

int main(void)
{
	return 0;
}
