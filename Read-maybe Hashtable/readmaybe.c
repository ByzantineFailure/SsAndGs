#include "readmaybe.h"

read_maybe_hashtable *readmaybe_create_readmaybe()
{
	read_maybe_hashtable *newTable = (read_maybe_hashtable *)malloc(sizeof(read_maybe_hashtable));
	void **contents = (void **)malloc(sizeof(void *) * HASH_TABLE_SIZE);
	int i;

	for(i = 0; i < HASH_TABLE_SIZE; i++)
	{
		contents[i] = 0;
	}

	newTable->size = HASH_TABLE_SIZE;
	newTable->content = contents;
	return newTable;
}

int readmaybe_hash(void *data, size_t size)
{
	int hash = 0;
	unsigned i, currentValue;
	char *summableData = (char *)data;

	for(i = 0; i < size; i++)
	{
		currentValue = summableData[i];
		hash += currentValue;
	}

	hash = hash % HASH_TABLE_SIZE;
	srand((unsigned)time(NULL));	
	hash = hash * rand();
	return hash;
}

void *readmaybe_attempt_to_get_data(read_maybe_hashtable *table, void *key, size_t keySize)
{
	int hash = readmaybe_hash(key, keySize);
	return table->content[hash];
}