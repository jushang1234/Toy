/* CSci 4061 F2014 Assignment 3
 * Section 3
 * Date: 10/10/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#include <stdio.h>
#include <stdlib.h>

#include "mm.h"

int main() {
	struct timeval time_s, time_e;
	void* chunks[NUM_CHUNKS];
	mm_t test_mm;
	mm_init(&test_mm, NUM_CHUNKS, CHUNK_SIZE);
	
	gettimeofday(&time_s, NULL);
	
	int i;
	for (i = 0; i < NUM_CHUNKS; i++) {
		chunks[i] = mm_get(&test_mm);
	}
	for (i = 0; i < NUM_CHUNKS; i++) {
		mm_put(&test_mm, chunks[i]);
	}
	
	gettimeofday(&time_e, NULL);
	
	fprintf(stderr, "Time taken = %f msec\n", comp_time(time_s, time_e) / 1000.0);
	
	return 0;
}
	
	
