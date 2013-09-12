#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#define LOG_OUTPUT stderr
#define DEBUG_LOGGING
#include "logging.h"

#define DEBUG_KEYS

#ifdef DEBUG_KEYS
#define GET_DEBUG_KEY(x) trie_getPrintableKey(x)
#define FREE_DEBUG_KEY(x) free(x)
#else
#define GET_DEBUG_KEY(x) 0
#define FREE_DEBUG_KEY(x)
#endif

#define TRIE_CHILDREN_PER_NODE 256 //Keys are a string, each level is the next char in the string, therefore, each level has 2^8 children

#define TRIE_UNINITIALIZED_NODE 0xDEADBEEF

#define TRIE_ADD_STATUS_SUCCESS 1
#define TRIE_ADD_STATUS_FALURE -1
#define TRIE_ADD_STATUS_DUPLICATE 2
#define TRIE_ADD_STATUS_OHGODWHATDIDYOUDO -100

#define TRIE_GET_STATUS_DNE 0
#define TRIE_GET_STATUS_SUCCESS 1
#define TRIE_GET_STATUS_EXISTS_NO_CONTENT 2

#define TRIE_NODEFREE_STATUS_FREED 1
#define TRIE_NODEFREE_STATUS_NOTFREED 2

typedef struct trie_key {
	char *key;
	long int length;
} trie_key;

typedef struct trie_node {
	void *content;
	struct trie_node **children;
	trie_key *key;
	int childrenHaveContent;
} trie_node;

typedef struct trie_get_result {
	void *result;
	int status;
} trie_get_result;

typedef struct trie_search_result {
	trie_get_result *result;
	trie_node *node;
} trie_search_result;

typedef struct trie {
	trie_node **nodes;
	long int dataSize;
	long int radixDepth;
} trie;

//Create a new trie
trie* trie_newTrie(long int dataSize, long int radixDepth);

//Destroy a trie
void trie_freeTrie(trie *toFree);

//Add a node to the trie.  The key you pass in need not be null 
//terminated (and in fact it's preferable if it's not)
int trie_addNode(trie *addTo, void *content, trie_key *key);

//Get the data from the trie for a specific key
trie_get_result* trie_getKey(trie *getFrom, trie_key *key);

//Null-terminates then copies key into a separate array.  Must be freed by the user.
//Allows the use of standard string functions with a trie_key.
char *trie_getPrintableKey(trie_key *key);