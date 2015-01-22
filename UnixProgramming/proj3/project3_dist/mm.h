/* CSci 4061 F2014 Assignment 3
 * Section 3
 * Date: 10/10/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#ifndef __MM_H
#define __MM_H

#include <sys/time.h>

#define INTERVAL 0
#define INTERVAL_USEC 500000
#define CHUNK_SIZE 64
#define NUM_CHUNKS 1000000


/* TODO - Fill this in */
typedef struct {

	void* mem_ptr;
	int num_chunks;
	int chunk_size;
	int* mem_table;
	int nextUnused;

} mm_t;

int findChunk(mm_t* mm, void* chunk);




/* TODO - Implement these in mm.c */
double comp_time(struct timeval time_s, struct timeval time_e);
int mm_init(mm_t *mm, int num_chunks, int chunk_size);
void *mm_get(mm_t *mm);
void mm_put(mm_t *mm, void *chunk);
void mm_release(mm_t *mm);

#endif
