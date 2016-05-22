cd ..; ./run_linux.sh
cd testing
if [ ! -z "$1" ]; then
	dd if=/dev/zero of=$1 bs="$1M" count=1 &> /dev/null
	printf "Uncompressed:\t$1M\n"
	\time -f "Compression:\t%E sec" ../LZW/lzw c $1 output
	printf "Compressed:\t$(ls -lh output | awk '{print $5}')\n"
	\time -f "Decompression:\t%E sec" ../LZW/lzw d output test
	if [ "$(diff $1 test)" = "" ]; then
		echo PASS
	else
		echo FAIL
	fi
	rm -f $1
else
	dd if=/dev/zero of=1M bs=1M count=1 &> /dev/null
	echo Size 1M
	printf "Uncompressed:\t1M\n"
	\time -f "Compression:\t%E sec" ../LZW/lzw c 1M output
	printf "Compressed:\t$(ls -lh output | awk '{print $5}')\n"
	\time -f "Decompression:\t%E sec" ../LZW/lzw d output test
	if [ "$(diff 1M test)" = "" ]; then
		echo PASS
	else
		echo FAIL
	fi
	rm -f 1M
fi

rm -f input output test
