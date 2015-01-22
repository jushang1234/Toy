/* CSci 4061 F2014 Assignment 3
 * Section 3
 * Date: 10/10/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#include <stdio.h>
#include <stdlib.h>

#include "mm.h"

/* Return usec */
double comp_time(struct timeval time_s, struct timeval time_e) {

  double elap = 0.0;

  if (time_e.tv_sec > time_s.tv_sec) {
    elap += (time_e.tv_sec - time_s.tv_sec - 1) * 1000000.0;
    elap += time_e.tv_usec + (1000000 - time_s.tv_usec);
  }
  else {
    elap = time_e.tv_usec - time_s.tv_usec;
  }
  return elap;
}

int findChunk(mm_t* mm, void* chunk) {
  int index = (chunk - mm->mem_ptr) / mm->chunk_size;
  return index; 
}

/* TODO - Implement.  Return 0 for success, or -1 and set errno on fail. */
int mm_init(mm_t *mm, int hm, int sz) {

  mm->num_chunks = hm;
  mm->chunk_size = sz;
  if ((mm->mem_ptr = malloc(mm->num_chunks * mm->chunk_size)) == NULL) {
    fprintf(stderr, "Memory manager failed to allocate memory.\n");
    return -1;
  }
  if ((mm->mem_table = (int*) calloc(mm->num_chunks, sizeof(int))) == NULL) {
    fprintf(stderr, "Memory manager failed to allocate memory.\n");
    return -1;
  } 
  mm->nextUnused = 0;

  return 0;  /* TODO - return the right value */
}

/* TODO - Implement */
void *mm_get(mm_t *mm) {

  int start = mm->nextUnused;

  while (mm->mem_table[mm->nextUnused] != 0) {

    mm->nextUnused++;

    if (mm->nextUnused >= mm->num_chunks) {
      mm->nextUnused = 0;
    }

    if (mm->nextUnused == start) {
      fprintf(stderr, "There is no available memory.\n");
      return NULL;
    }

  }
  
  mm->mem_table[mm->nextUnused] = 1;
  void* chunk = mm->mem_ptr + mm->nextUnused * mm->chunk_size;
  mm->nextUnused++;

  return chunk;
}

/* TODO - Implement */
void mm_put(mm_t *mm, void *chunk) {
  int index = findChunk(mm, chunk);
  if (index >= mm->num_chunks) {
    fprintf(stderr, "Memory menager cannot find the memory chunk.\n");
    return;
  }
  mm->mem_table[index] = 0;
}

/* TODO - Implement */
void mm_release(mm_t *mm) {
  free(mm->mem_ptr);
  free(mm->mem_table);
}

/*
 * TODO - This is just an example of how to use the timer.  Notice that
 * this function is not included in mm_public.h, and it is defined as static,
 * so you cannot call it from other files.  Instead, just follow this model
 * and implement your own timing code where you need it.
 */
static void timer_example() {
  struct timeval time_s, time_e;
  mm_t test_mm;
  mm_init(&test_mm, NUM_CHUNKS, CHUNK_SIZE);
  void* chunks[NUM_CHUNKS];

  /* start timer */
  gettimeofday (&time_s, NULL);

  /* TODO - code you wish to time goes here */
  int i;
  for (i = 0; i < NUM_CHUNKS; i++) {
    chunks[i] = mm_get(&test_mm);
    //chunks[i] = malloc(CHUNK_SIZE);
  }
  for (i = 0; i < NUM_CHUNKS; i++) {
    mm_put(&test_mm, chunks[i]);
    //free(chunks[i]);
  }

  gettimeofday(&time_e, NULL);

  fprintf(stderr, "Time taken = %f msec\n",
          comp_time(time_s, time_e) / 1000.0);
}

