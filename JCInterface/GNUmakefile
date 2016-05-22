CC = gcc
CFLAGS = -Wno-discarded-qualifiers -Wno-unused-parameter -fPIC -c -Wall -Wextra \
-Wno-maybe-uninitialized -Wno-sign-compare \
-I/usr/lib/jvm/java-7-openjdk-amd64/include \
-I/usr/lib/jvm/java-8-openjdk-amd64/include \
-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux \
-I../LZW/

ifeq ($(OS),Windows)
	DEL = del
	LIBEXT = .dll
else
	DEL = rm
	LIBEXT = .so
endif


build: java_setup libcompression.so

java_setup:
	javac CompressionLib.java
	javah -jni CompressionLib
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

libcompression$(LIBEXT): compression.o LZW.o bitfile.o
	$(CC) -Wno-unused-parameter -shared LZW.o bitfile.o compression.o -o libcompression$(LIBEXT)

compression.o: compression.c ../LZW/LZW.h
	$(CC) $(CFLAGS) compression.c -o compression.o

LZW.o: ../LZW/LZW.c ../LZW/LZW.h
	$(CC) $(CFLAGS) $<

bitfile.o:  ../LZW/bitfile.c ../LZW/bitfile.h
	$(CC) $(CFLAGS) $<

run-lzw:
	echo > output_encode
	java -Djava.library.path=. CompressionLib encode LZW ../input ../output_encode
	java -Djava.library.path=. CompressionLib decode LZW ../output_encode ../output_decode
	diff ../input ../output_decode

run-debug:
	gcc ../LZW/debug.c -o lzw -lcompression -I/home/smith/Dropbox/IP/bandit-archiver/JCInterface -L/home/smith/Dropbox/IP/bandit-archiver/JCInterface
	./lzw c ../input ../output_encode
	./lzw d ../output_encode ../output_decode
	diff ../input ../output_decode

clean:
	$(DEL) *.class
	$(DEL) *.so *.o *.a
	$(DEL) exq_debug ../output_encode ../output_decode
