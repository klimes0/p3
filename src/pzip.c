#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pzip.h"

/**
 * pzip() - zip an array of characters in parallel
 *
 * Inputs:
 * @n_threads:		   The number of threads to use in pzip
 * @input_chars:		   The input characters (a-z) to be zipped
 * @input_chars_size:	   The number of characaters in the input file
 *
 * Outputs:
 * @zipped_chars:       The array of zipped_char structs
 * @zipped_chars_count:   The total count of inserted elements into the zippedChars array.
 * @char_frequency[26]: Total number of occurences
 *
 * NOTE: All outputs are already allocated. DO NOT MALLOC or REASSIGN THEM !!!
 *
 */

static void slice(const char* str, char* result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
}

static int alpha_pos (char c) {
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < 26; i++) {
		if (c == alphabet[i]) {
			return i;
		}
	}
	return -1; // if we're out here, something has gone very wrong
 }

typedef struct {
    char key;
    pthread_mutex_t value;
} map_t;

typedef struct {
	char *input_chars;
	int input_chars_size;
	int *char_frequency;
	pthread_mutex_t* pthread_map;
} cc_args;

pthread_barrier_t barrier;

static void* count_chars(void* cnct_args) {
	cc_args* unpacked_args = (cc_args*) cnct_args;
	struct zipped_char* return_values = (struct zipped_char*) malloc(unpacked_args->input_chars_size*sizeof(map_t));
	int current_array_pos = 0;
	int current_count = 0;
	char current_char = '!';
	int current_index = -1;
	char prev_char = '?';

	for (int i = 0; i < unpacked_args->input_chars_size; i++) {
		current_char = unpacked_args->input_chars[i]; // keep updating character to check for new ones

		if (current_char != prev_char) { // if this is the first time we're encountering the character
			prev_char = current_char; // update the previous character

			pthread_mutex_unlock(&unpacked_args->pthread_map[current_index]); // unlock the previous mutex
			current_index = alpha_pos(current_char); // get the index of the character in the alphabet
			pthread_mutex_lock(&unpacked_args->pthread_map[current_index]); // lock the current mutex

			struct zipped_char current_pair = {prev_char, current_count}; // initialize a new pair to add to the array

			return_values[current_array_pos] = current_pair; // add the pair to the next position in the array
			current_array_pos += 1; // move to next position in array
			
			current_count = 0; // reset character counter

		}
		
		unpacked_args->char_frequency[current_index] += 1; // update value in frequency
		current_count += 1; // update count for struct
		
	}
	pthread_mutex_unlock(&unpacked_args->pthread_map[current_index]);

	return return_values;

}




void pzip(int n_threads, char *input_chars, int input_chars_size,
	  struct zipped_char *zipped_chars, int *zipped_chars_count,
	  int *char_frequency)
{
	//printf("WORLD");
	// create array for the threads 
	pthread_t *thread_id[n_threads];

	// create all the mutexes
	pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_c = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_d = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_e = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_f = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_g = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_h = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_i = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_j = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_k = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_l = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_n = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_o = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_p = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_q = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_r = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_t = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_u = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_v = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_w = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_x = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_y = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_z = PTHREAD_MUTEX_INITIALIZER;
	
	
	// for each unique input character, create a semaphore
	pthread_mutex_t mutex_arr[26] = {mutex_a,mutex_b, mutex_c, mutex_d, mutex_e, mutex_f, mutex_g, mutex_h, mutex_i, mutex_j,mutex_k, mutex_l, mutex_m, mutex_n, mutex_o,mutex_p, mutex_q, mutex_r, mutex_s, mutex_t, mutex_u, mutex_v, mutex_w, mutex_x, mutex_y, mutex_z};

	// divide array into relatively equal parts
	int chunk_size = input_chars_size/n_threads;
	int start_pos = 0;
	int end_pos = chunk_size;
	
	// FINE C I'll initialize them manually
	cc_args *chars_args = NULL;
	chars_args->input_chars = input_chars;
	chars_args->input_chars_size = chunk_size;
	chars_args->char_frequency = char_frequency;
	chars_args->pthread_map = mutex_arr;

	

	pthread_barrier_init(&barrier, NULL, n_threads);
	for (int i = 0; i< n_threads; i++){
		// split off the chunk needed
		char* chunk = malloc(chunk_size*sizeof(char));
		slice(input_chars,chunk,start_pos,end_pos);
		chars_args->input_chars = chunk; // stick the chunk in the struct
		printf("%s",chunk);

		// make a new thread for each thread we have
		pthread_create(thread_id[i], NULL, count_chars, chars_args);

		// move the end pos
		end_pos += chunk_size;
		free(chunk); // hopefully this doesn't break anything
	}

	pthread_barrier_wait(&barrier); // wait for all the threads to finish

	int array_pos = 0;
	for (int i =0; i< n_threads; i++) {
		void* result_values;
		pthread_join(*thread_id[i], &result_values);

		struct zipped_char* typed_results = (struct zipped_char*) result_values;

		for (int j = 0; j<sizeof(result_values); j++) {
			zipped_chars[array_pos] = typed_results[j];
			zipped_chars_count++;
			array_pos++;
		}

		free(result_values);
	}


	// need to free retrun value of each function, chunk
	pthread_barrier_destroy(&barrier);
}
