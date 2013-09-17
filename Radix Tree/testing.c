#define LOG_OUTPUT stderr
#define DEBUG_LOGGING
#define STANDARD_LOGGING
#include "logging.h"
#include "trie.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_SUCCESS 1
#define TEST_FAILED 0
#define TEST_UNFINISHED -1

#define ADD_NODE_NONDUPLICATE_TEST_DATA "Look, ma, it's test data!"
#define ADD_NODE_DUPLICATE_TEST_DATA "This is duplicate data"

#define REMOVE_NODE_SECOND_ADD_TEST_DATA "This is the test data for RemoveNode AddRemoveAdd"

#define ADD_NODE_TEST_KEY_1 "TESTING_KEY"
#define ADD_NODE_CHILD_KEY_1 "TESTING_KEY1"
#define ADD_NODE_PARENT_KEY_1 "TESTING_KE"
#define ADD_NODE_NULL_STRING_KEY ""

int test_addNode_duplicate();
int test_addNode_single();
int test_removeNode();
int test_addAfterRemove();

int main(int argc, char *argv)
{
	int testResult;

	testResult = test_addNode_single();

	if(testResult == TEST_FAILED)
	{
		printf("test_addNode_single failed!\n");	
	}
	else
	{
		printf("test_addNode_single passed!\n");
	}

	testResult = test_addNode_duplicate();

	if(testResult == TEST_FAILED)
	{
		printf("test_addNode_duplicate failed!\n");	
	}
	else
	{
		printf("test_addNode_duplicate passed!\n");
	}

	testResult = test_removeNode();

	if(testResult == TEST_FAILED)
	{
		printf("test_removeNode failed!\n");	
	}
	else
	{
		printf("test_removeNode passed!\n");
	}

	testResult = test_addAfterRemove();

	if(testResult == TEST_FAILED)
	{
		printf("test_addAfterRemove failed!\n");
	}
	else
	{
		printf("test_addAfterRemove passed!\n");
	}
	getchar();
}

int test_addNode_single()
{
	//Initialize
	char *testData = (char *)malloc(sizeof(char) * 100);
	char *testKey = (char *)malloc(sizeof(char) * 100);
	int testKeyLength;
	int addResult1;
	int testResult = TEST_UNFINISHED;
	trie *newTrie = trie_newTrie(100, 256);
	trie_key *key = (trie_key *)malloc(sizeof(trie_key));	
	trie_get_result *result;

	strcpy(testKey, ADD_NODE_TEST_KEY_1);
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData, ADD_NODE_NONDUPLICATE_TEST_DATA);
	
	key->key = testKey;
	key->length = testKeyLength;
	
	//Test
	addResult1 = trie_addNode(newTrie, testData, key);
	
	result = trie_getKey(newTrie, key);
	
	if(addResult1 != TRIE_ADD_STATUS_SUCCESS || strcmp(ADD_NODE_NONDUPLICATE_TEST_DATA, (char *)result->result) != 0)
	{
		testResult = TEST_FAILED;
	}
	else
	{
		testResult = TEST_SUCCESS;
	}
	
	//Cleanup
	free(key->key);
	free(key);
	free(result);
	trie_destroy(newTrie);

	return testResult;
}

int test_addNode_duplicate()
{
	//Initialize
	char *testData = (char *)malloc(sizeof(char) * 100);
	char *duplicateTestData = (char *)malloc(sizeof(char) * 100);
	char *testKey = (char *)malloc(sizeof(char) * 100);
	int testKeyLength;
	int addResult;
	int testResult = TEST_UNFINISHED;
	trie *newTrie = trie_newTrie(100, 256);
	trie_key *key = (trie_key *)malloc(sizeof(trie_key));	
	trie_get_result *result;

	strcpy(testKey, ADD_NODE_TEST_KEY_1);
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData, ADD_NODE_NONDUPLICATE_TEST_DATA);

	strcpy(duplicateTestData, ADD_NODE_DUPLICATE_TEST_DATA);
	
	key->key = testKey;
	key->length = testKeyLength;

	//Test
	addResult = trie_addNode(newTrie, testData, key);

	if(addResult != TRIE_ADD_STATUS_SUCCESS)
	{
		 testResult = TEST_FAILED;
	}
	
	addResult = trie_addNode(newTrie, duplicateTestData, key);
	
	if(addResult != TRIE_ADD_STATUS_DUPLICATE)
	{
		testResult = TEST_FAILED;
	}
	
	result = trie_getKey(newTrie, key);

	if(strcmp((char *)(result->result), ADD_NODE_NONDUPLICATE_TEST_DATA) != 0)
	{
		testResult = TEST_FAILED;
	}
	if(testResult != TEST_FAILED)
	{
		testResult = TEST_SUCCESS;
	}

	//Cleanup
	free(key->key);
	free(key);
	trie_destroy(newTrie);
	free(duplicateTestData);

	return testResult;
}

int test_addNode_nullString()
{
	return TEST_FAILED; 
}

int test_removeNode()
{
	//Initialize
	char *testData = (char *)malloc(sizeof(char) * 100);
	char *testKey = (char *)malloc(sizeof(char) * 100);
	int testKeyLength;
	int addResult1;
	int testResult = TEST_UNFINISHED;
	trie *newTrie = trie_newTrie(100, 256);
	trie_key *key = (trie_key *)malloc(sizeof(trie_key));	
	trie_remove_result *result;
	trie_get_result *retrievalAttempt;

	strcpy(testKey, ADD_NODE_TEST_KEY_1);
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData, ADD_NODE_NONDUPLICATE_TEST_DATA);
	key->key = testKey;
	key->length = testKeyLength;
	


	//Test
	addResult1 = trie_addNode(newTrie, testData, key);

	if(addResult1 == TRIE_ADD_STATUS_FALURE)
	{
		printf("test_removeNode failed on initial add!\n");
		testResult = TEST_FAILED;
	}

	result = trie_removeNode(newTrie, key);

	if(result->status != TRIE_REMOVE_STATUS_SUCCESS)
	{
		printf("test_removeNode failed remove!\n");
		testResult = TEST_FAILED;
	}

	retrievalAttempt = trie_getKey(newTrie, key);
	
	if(retrievalAttempt->status != TRIE_GET_STATUS_DNE)
	{
		printf("test_removeNode failed by finding removed node!\n");
		printf("test_removeNode found data at node to be %s\n", (char *)retrievalAttempt->result);
		printf("test_removeNode return status for getKey is %d\n", retrievalAttempt->status);
		testResult = TEST_FAILED;
	}
	else
	{
		testResult = TEST_SUCCESS;
	}

	//Cleanup
	free(retrievalAttempt->result);
	free(key->key);
	free(key);
	trie_destroy(newTrie);
	free(result);
	return testResult;
}

int test_addAfterRemove()
{
	//Initialize
	char *testData = (char *)malloc(sizeof(char) * 100);
	char *testKey = (char *)malloc(sizeof(char) * 100);
	char *testData2 = (char *)malloc(sizeof(char) * 100);
	char *testKey2 = (char *)malloc(sizeof(char) * 100);
	int testKeyLength;
	int addResult1, addResult2;
	int testResult = TEST_UNFINISHED;
	trie *newTrie = trie_newTrie(100, 256);
	trie_key *key = (trie_key *)malloc(sizeof(trie_key));	
	trie_key *key2 = (trie_key *)malloc(sizeof(trie_key));
	trie_remove_result *result;
	trie_get_result *retrievalAttempt;
	trie_get_result *retrievalAttempt2;

	strcpy(testKey, ADD_NODE_TEST_KEY_1);
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData, ADD_NODE_NONDUPLICATE_TEST_DATA);
	key->key = testKey;
	key->length = testKeyLength;
	
	strcpy(testKey2, ADD_NODE_TEST_KEY_1);
	testKeyLength = strlen(testKey) + 1;
	strcpy(testData2, REMOVE_NODE_SECOND_ADD_TEST_DATA);
	key2->key = testKey2;
	key2->length = testKeyLength;

	//Test
	addResult1 = trie_addNode(newTrie, testData, key);

	if(addResult1 == TRIE_ADD_STATUS_FALURE)
	{
		printf("test_addAfterRemove failed on initial add!\n");
		testResult = TEST_FAILED;
	}

	result = trie_removeNode(newTrie, key);

	if(result->status != TRIE_REMOVE_STATUS_SUCCESS)
	{
		printf("test_addAfterRemove failed remove!\n");
		testResult = TEST_FAILED;
	}

	retrievalAttempt = trie_getKey(newTrie, key);
	
	if(retrievalAttempt->status != TRIE_GET_STATUS_DNE)
	{
		printf("test_addAfterRemove failed by finding removed node!\n");
		printf("test_addAfterRemove found data at node to be %s\n", (char *)retrievalAttempt->result);
		printf("test_addAfterRemove return status for getKey is %d\n", retrievalAttempt->status);
		testResult = TEST_FAILED;
	}
	else
	{
		testResult = TEST_SUCCESS;
	}

	addResult2 = trie_addNode(newTrie, testData2, key2);

	if(addResult2 == TRIE_ADD_STATUS_FALURE)
	{
		printf("test_addAfterRemove failed on second add by general failure!\n");
		testResult = TEST_FAILED;
	}
	if(addResult2 == TRIE_ADD_STATUS_DUPLICATE)
	{
		printf("test_addAfterRemove failed on second add by finding a duplicate!\n");
		testResult = TEST_FAILED;
	}

	retrievalAttempt2 = trie_getKey(newTrie, key2);

	if(retrievalAttempt2->status != TRIE_GET_STATUS_SUCCESS)
	{
		printf("test_addAfterRemove failed on getting after second add!\n");
		testResult = TEST_FAILED;
	}
	if(strcmp((char *)retrievalAttempt2->result, REMOVE_NODE_SECOND_ADD_TEST_DATA) != 0)
	{
		printf("test_addAfterRemove failed because second add test data was not equal to expected value!\n");
		testResult = TEST_FAILED;
	}

	if(testResult != TEST_FAILED)
	{
		testResult = TEST_SUCCESS;
	}

	//Cleanup
	free(retrievalAttempt);
	free(retrievalAttempt2);
	free(key->key);
	free(key);
	free(key2->key);
	free(key2);
	trie_destroy(newTrie);
	free(result);
	return testResult;
}


