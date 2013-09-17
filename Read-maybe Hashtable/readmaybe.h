#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>

#define HASH_TABLE_SIZE 1000

typedef struct read_maybe_hashtable {
	int size;
	void **content;
} read_maybe_hashtable;

read_maybe_hashtable *readmaybe_create_readmaybe();

void *readmaybe_attempt_to_get_data(read_maybe_hashtable *table, void *key, size_t keySize);

int readmaybe_hash(void *data, size_t length);