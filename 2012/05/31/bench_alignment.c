#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define TEST_ALIGNED 0
#define REPEAT_COUNT 100000
uint16_t indexes[100000];
uint32_t data1[32768];
uint32_t data2[32768];
uint32_t data3[32768];
uint32_t data4[32768];

uint32_t accumulate_non_aligned(uint8_t *data1, uint8_t *data2, uint8_t *data3, uint8_t *data4, uint16_t *indexes, uint32_t size) {
	int i, j;
	uint32_t result = 0;
	for (j=0; j<REPEAT_COUNT; j++) {
		for (i=0; i<size; i++) {
			result += *((volatile uint32_t *)(data1+indexes[i]));
			result += *((volatile uint32_t *)(data2+indexes[i]));
			result += *((volatile uint32_t *)(data3+indexes[i]));
			result += *((volatile uint32_t *)(data4+indexes[i]));
		}
	}
	return result;
}
uint32_t accumulate_aligned(uint32_t *data1, uint32_t *data2, uint32_t *data3, uint32_t *data4, uint16_t *indexes, uint32_t size) {
	int i, j;
	uint32_t result = 0;
	for (j=0; j<REPEAT_COUNT; j++) {
		for (i=0; i<size; i++) {
			result += *((volatile uint32_t *)(data1+indexes[i]));
			result += *((volatile uint32_t *)(data2+indexes[i]));
			result += *((volatile uint32_t *)(data3+indexes[i]));
			result += *((volatile uint32_t *)(data4+indexes[i]));
		}
	}
	return result;
}

int main(int argc, char **argv) {
	int i;
	uint32_t result = 0;
	clock_t start, end;
	/* Setup the arrays.  The seed for the random number has to be provided as a command line parameter. */
	srand(1);
	for (i=0;i<100000;i++) {
	#ifdef TEST_ALIGNED
		indexes[i] = rand() % 32768;
	#else
		if (i & 0x1) {
		indexes[i] = (rand() % (32768/8))*8*4 + 30;
		} else {
		indexes[i] = (rand() % (32768/8))*8*4 + 30;
		}
	#endif
	}
	for (i=0;i<32768;i++) {
		data1[i] = rand();
		data2[i] = rand();
		data3[i] = rand();
		data4[i] = rand();
	}
	/* Run the test many times and measure time */
	start = clock();
	#ifdef TEST_ALIGNED
	result += accumulate_aligned(data1, data2, data3, data4, indexes, 100000);
	#else
	result += accumulate_non_aligned((uint8_t *)data1, (uint8_t *)data2, (uint8_t *)data3, (uint8_t *)data4, indexes, 100000);
	#endif
	end = clock();
	printf("result = %d, time = %d\n", result, ((end-start)/CLOCKS_PER_SEC));
}
