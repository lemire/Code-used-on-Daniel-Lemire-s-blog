batchload: batchload.c
	cc -O3 -o batchload batchload.c
cachemiss:
	cc -O3 -o batchload1 batchload1.c
	cc -O3 -o batchload2 batchload2.c
	echo "normal version (sum of random access)"
	perf stat -B -e cache-misses ./batchload1
	echo "version where we first compute the indexes and then sum"
	perf stat -B -e cache-misses ./batchload2

clean:
	rm -r -f batchload
