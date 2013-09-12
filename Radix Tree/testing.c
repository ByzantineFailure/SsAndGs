#define LOG_OUTPUT stderr
#define DEBUG_LOGGING
#define STANDARD_LOGGING
#include "logging.h"
#include "trie.h"


int main(int argc, char *argv)
{
	char *testData = (char *)malloc(sizeof(char) * 100);
	char *duplicateKeyData = (char *)malloc(sizeof(char) * 100);
	char *testKey = (char *)malloc(sizeof(char) * 100);
	int testKeyLength;
	int addResult;
	trie *newTrie = trie_newTrie(100, 256);
	trie_key *key = (trie_key *)malloc(sizeof(trie_key));	
	trie_get_result *result;

	strcpy(testKey, "TESTING KEY");
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData, "Look, ma, it's test data!");
	strcpy(duplicateKeyData, "This is duplicate data, somethin' done gon wrong.\n");

	key->key = testKey;
	key->length = testKeyLength;

	trie_addNode(newTrie, testData, key);

	result = trie_getKey(newTrie, key);

	printf("GetKey result is: %s\n", (char *)result->result);

	printf("Attempting duplicate key insert...\n");
	
	addResult = trie_addNode(newTrie, duplicateKeyData, key);
	
	fprintf(stdout, "Add Result is %d\n", addResult);
	
	result = trie_getKey(newTrie, key);
	
	fprintf(stdout, "AFter duplicate add, data is: %s\n", (char *)result->result);

	free(key->key);
	free(key);
	free(result);
	getchar();
}