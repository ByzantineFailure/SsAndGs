#include "trie.h"

#define TRIE_NODE_CHECK_SUCCESS 1
#define TRIE_NODE_CHECK_FAILURE -1

//void trie_freeNode(trie_node *toFree);
int trie_getKey_checkNode(trie_node *toCheck);
trie_search_result* trie_getKey_getSearchResult(trie *getFrom, trie_key *get, trie_get_result *result, int ignoreContentCheck);
void trie_getKey_traverseNode(trie_node *check, trie_key *get, long int depth, trie_search_result *search);
trie_node *trie_addNode_createNodeForKey(trie *addTo, trie_key *key, void *content);
trie_node *trie_addNode_createNodeForKey_recursiveCall(trie_node *addTo, trie_key *key, void *content, long int depth);
trie_node *trie_createNode(char *key, int keyLength);
char *trie_addNode_createPartialKey(trie_key *key, int partialLength);
int trie_freeNode_internalNodeFree(trie_node *toFree, int freeEntireTrie);

//Public
trie* trie_newTrie(long int dataSize, long int radixDepth)
{
	int i;
	trie *retVal = (trie *)malloc(sizeof(trie));
	retVal->radixDepth = radixDepth;
	retVal->dataSize = dataSize;
	retVal->nodes = (trie_node **)malloc(sizeof(trie_node *) * TRIE_CHILDREN_PER_NODE);

	for(i = 0; i < TRIE_CHILDREN_PER_NODE; i++)
	{
		retVal->nodes[i] = 0;
	}

	return retVal;
}

//Public
trie_get_result* trie_getKey(trie *getFrom, trie_key *get)
{
	char *debugKey = GET_DEBUG_KEY(get);

	//This line for some reason breaks compile.  Investigate.
	//DEBUG_LOG(2, "Entering trie_getKey for key %s, having depth %l\n", debugKey, get->length);

	trie_get_result *result = (trie_get_result *)malloc(sizeof(trie_get_result));
	trie_search_result *search = trie_getKey_getSearchResult(getFrom, get, result, 0);
	free(search);
	DEBUG_LOG(2, "Exiting trie_getKey for key %s, having depth %d\n", debugKey, get->length);
	FREE_DEBUG_KEY(debugKey);
	return result;
}

//Private
trie_search_result* trie_getKey_getSearchResult(trie *getFrom, trie_key *get, trie_get_result *result, int	ignoreContentCheck)
{
	trie_search_result*	search = (trie_search_result *)malloc(sizeof(trie_search_result));
	result->result = 0;
	result->status = TRIE_GET_STATUS_DNE;
	search->result = result;
	search->node = 0;

	if(getFrom->nodes[get->key[0]] == 0)		
	{
		DEBUG_LOG(3, "trie_getKey failed at first position in radix\n");
		return search;
	}
	else if(get->length == 1)
	{
		search->node = getFrom->nodes[get->key[0]];
		if(trie_getKey_checkNode(getFrom->nodes[get->key[0]]) == TRIE_NODE_CHECK_FAILURE)
		{
			DEBUG_LOG(3, "trie_getKey on depth 1, depth 1 node has no content\n");
			search->result->status = TRIE_GET_STATUS_EXISTS_NO_CONTENT;
			return search;
		}
		else
		{
			DEBUG_LOG(3, "trie_getKey on depth 1, depth 1 node has content, success\n");
			search->result->status = TRIE_GET_STATUS_SUCCESS;
			search->result->result = getFrom->nodes[get->key[0]]->content;
			return search;
		}
	}
	else
	{
		if(getFrom->nodes[get->key[0]]->childrenHaveContent == 0 && !ignoreContentCheck)
		{
			DEBUG_LOG(3, "Top level node marked as having no children with content\n");
			return search;
		}
		else
		{
			DEBUG_LOG(3, "trie_getKey entering depth >1 traversal\n");
			trie_getKey_traverseNode(getFrom->nodes[get->key[0]], get, 1, search);
			return search;
		}
	}

}

//Private
void trie_getKey_traverseNode(trie_node *check, trie_key *get, long int depth, trie_search_result *search)
{
	char *debugKey = GET_DEBUG_KEY(get); 
	if(depth > get->length)
	{
		DEBUG_LOG(3, "trie_getKey for key %s, depth %d now below depth of radix.  Failed!\n", debugKey, depth - 1);
		FREE_DEBUG_KEY(debugKey);
		return;
	}
	else if(depth == get->length)
	{
		DEBUG_LOG(3, "Checking final depth for trie_getKey %s, depth %d\n", debugKey, depth - 1);
		if(check->content != 0)
		{
			DEBUG_LOG(3, "trie_getKey for key %s successful!\n", debugKey);
			search->result->result = check->content;	
			search->result->status = TRIE_GET_STATUS_SUCCESS;
			search->node = check;
			FREE_DEBUG_KEY(debugKey);
			return;
		}
		else
		{
			DEBUG_LOG(3, "trie_getKey for key %s failed!\n", debugKey);
			search->node = check;
			search->result->status = TRIE_GET_STATUS_EXISTS_NO_CONTENT;
			FREE_DEBUG_KEY(debugKey);
			return;
		}
	}
	else
	{
		DEBUG_LOG(3, "trie_getKey looking for key value %c at depth %d from key %s\n", get->key[depth], depth, debugKey);
		if(check->childrenHaveContent == 0)
		{
			DEBUG_LOG(3, "trie_getKey failed due to node at depth %d being marked as having no children with content\n", depth - 1);
			FREE_DEBUG_KEY(debugKey);
			return;
		}
		else if(check->children[get->key[depth]] == 0)
		{
			DEBUG_LOG(3, "trie_getKey failed due to no node of key value %c existing at depth %d\n", get->key[depth], depth);
			FREE_DEBUG_KEY(debugKey);
			return;
		}
		else
		{
			DEBUG_LOG(3, "trie_getKey at depth %d moving down to next depth for key %s\n", depth, debugKey);
			FREE_DEBUG_KEY(debugKey);
			trie_getKey_traverseNode(check->children[get->key[depth]], get, depth + 1, search);
		}
	}
}

//Private
int trie_getKey_checkNode(trie_node *toCheck)
{
	char *debugKey = GET_DEBUG_KEY(toCheck->key);
	if(toCheck->content == 0)
	{
		DEBUG_LOG(3, "trie_getKey_checkNode failed for check node of key %s\n", toCheck->key);
		FREE_DEBUG_KEY(debugKey);
		return TRIE_NODE_CHECK_FAILURE;
	}
	else
	{
		DEBUG_LOG(3, "trie_getKey_checkNode success for check node of key %s\n", toCheck->key);
		FREE_DEBUG_KEY(debugKey);
		return TRIE_NODE_CHECK_SUCCESS;
	}
}

//Public
int trie_addNode(trie *addTo, void *content, trie_key *key)
{
	char *debugKey = GET_DEBUG_KEY(key);
	//DEBUG_LOG(2, "Entering trie_addNode for key %s\n", debugKey);
	trie_search_result *check;
	trie_get_result *result = (trie_get_result *)malloc(sizeof(trie_get_result));

	//DEBUG_LOG(3, "Checking for existing node/duplicate\n");
	check = trie_getKey_getSearchResult(addTo, key, result, 1);

	if(check->result->status != TRIE_GET_STATUS_DNE) 
	{
		//DEBUG_LOG(3, "Node exists at key %s\n", debugKey);
		if(check->result != 0)
		{
			//DEBUG_LOG(3, "trie_addNode failed.  Node already has content at key %s\n", debugKey);
			free(result);
			free(check);
			FREE_DEBUG_KEY(debugKey);
			DEBUG_LOG(2, "Exiting trie_addNode for key %s\n", debugKey);
			return TRIE_ADD_STATUS_DUPLICATE;
		}
		else
		{
			//DEBUG_LOG(3, "trie_addNode success.  No content for pre-existing node at key %s\n", debugKey);
			check->node->content = content;
			free(result);
			free(check);
			FREE_DEBUG_KEY(debugKey);
			DEBUG_LOG(2, "Exiting trie_addNode for key %s\n", debugKey);
			return TRIE_ADD_STATUS_SUCCESS;
		}
	}
	
	//DEBUG_LOG(3, "Trie does not contain node for key %s\n", debugKey);	
	free(result);
	free(check);
	trie_addNode_createNodeForKey(addTo, key, content);
	FREE_DEBUG_KEY(debugKey);
	DEBUG_LOG(2, "Exiting trie_addNode for key %s\n", debugKey);
	return TRIE_ADD_STATUS_SUCCESS;
}

//Private
//Do not call without ensuring no duplicate exists for key
trie_node *trie_addNode_createNodeForKey(trie *addTo, trie_key *key, void *content)
{
	trie_node *first;
	first = addTo->nodes[key->key[0]];
	if(first == 0)
	{
		//DEBUG_LOG(2, "Node for first character of key not found.  Creating...\n");
		char *newKey = trie_addNode_createPartialKey(key, 1);
		first = trie_createNode(newKey, 1);
		addTo->nodes[key->key[0]] = first;
	}
	
	if(key->length == 1)
	{
		DEBUG_LOG(3, "Key is one character long.  Returning first node of trie.\n");
		first->content = content;
		return first;	
	}
	DEBUG_LOG(2, "Beginning recursive addNode call...\n");
	return trie_addNode_createNodeForKey_recursiveCall(first, key, content, 2);
}

trie_node *trie_addNode_createNodeForKey_recursiveCall(trie_node *addTo, trie_key *key, void *content, long int depth)
{
	char *debugKey = trie_getPrintableKey(key);	
	trie_node *currentNode = addTo->children[key->key[depth - 1]];

	addTo->childrenHaveContent = 1;

	if(currentNode == 0)
	{
		//DEBUG_LOG(3, "No node exists at depth %d for key %s.  Creating new node.\n", depth, debugKey);
		char *newKey = trie_addNode_createPartialKey(key, depth);
		currentNode = trie_createNode(newKey, depth);
		addTo->children[key->key[depth - 1]] = currentNode;
	}

	if(depth == key->length)
	{
		DEBUG_LOG(3, "Reached key length's depth.  Associating content with current node.\n");
		currentNode->content = content;	
		FREE_DEBUG_KEY(debugKey);
		return currentNode;
	}
	else
	{
		//DEBUG_LOG(3, "Percolating call down to next depth for key %s\n", debugKey);
		FREE_DEBUG_KEY(debugKey);
		return trie_addNode_createNodeForKey_recursiveCall(currentNode, key, content, depth + 1);
	}	
}

//Private
//Does not associate data
trie_node *trie_createNode(char *key, int keyLength)
{
	trie_node *newNode = (trie_node *)malloc(sizeof(trie_node));	
	trie_key *newKey = (trie_key *)malloc(sizeof(trie_key));
	int i;
	newKey->key = key;
	newKey->length = keyLength;
	newNode->key = newKey;
	newNode->childrenHaveContent = 0;
	newNode->children = (trie_node **)malloc(sizeof(trie_node *) * TRIE_CHILDREN_PER_NODE);
	
	//Apparently msvc doesn't do this?
	for(i = 0; i < TRIE_CHILDREN_PER_NODE; i++)
	{
		newNode->children[i] = 0;
	}

	return newNode;
}

//Private
char *trie_addNode_createPartialKey(trie_key *key, int partialLength)
{
	char *debugKey = GET_DEBUG_KEY(key);
	
	//Also breaks compile for some reason
	//DEBUG_LOG(3, "Creating partial key of length %d for key %s\n", partialLength, debugKey);

	char *newKey = (char *)malloc(sizeof(char) * partialLength);
	int i;

	for(i = 0; i < partialLength; i++)
	{
		newKey[i] = key->key[i];
	}

	FREE_DEBUG_KEY(debugKey);
	return newKey;
}

//Public
int trie_freeNode(trie *freeFrom, trie_key *toFree, int freeContent)
{
	char *debugKey =  GET_DEBUG_KEY(toFree);
	int freeResult;
	//DEBUG_LOG(2, "Calling trie_FreeNode on key %s\n", debugKey);
	
	trie_get_result *result = (trie_get_result *)malloc(sizeof(trie_get_result));
	trie_search_result *search = trie_getKey_getSearchResult(freeFrom, toFree, result, 0);
	
	/*
	if(search->result->status == TRIE_GET_STATUS_DNE)
	{
		DEBUG_LOG(1, "Attempted to free node that does not exist!  Key: %s\n", debugKey);
	}
	*/
	free(result);
	if(freeContent)
	{
		if(search->node->content)
			free(search->node->content);
		//else
			//DEBUG_LOG(3, "No content to free for node at key %s\n", debugKey);
	}

	freeResult = trie_freeNode_internalNodeFree(search->node, 0);
	
	free(search);

	if(freeResult == TRIE_NODEFREE_STATUS_NOTFREED)
	{
		DEBUG_LOG(1, "Failed to free node for some reason.  Key: %s\n", debugKey);
	}

	FREE_DEBUG_KEY(debugKey);

	return freeResult;
}

//Private
int trie_freeNode_internalNodeFree(trie_node *toFree, int freeEntireTrie)
{
	char *debugKey = GET_DEBUG_KEY(toFree->key);
	//DEBUG_LOG(3, "Performing internalNodeFree on node of key %s\n", debugKey);
	trie_node *currentNode;
	int i;
	int freeStatus;
	int hasChildren = 0;

	for(i = 0; i < TRIE_CHILDREN_PER_NODE; i++)
	{
		currentNode = toFree->children[i];
		
		if(currentNode == 0)
		{
			continue;
		}
		
		freeStatus = trie_freeNode_internalNodeFree(currentNode, 0);
		
		if(freeStatus = TRIE_NODEFREE_STATUS_FREED)
		{
			toFree->children[i] = 0;
		}
		else
		{
			hasChildren = 1;
		}
	}

	if((toFree->content == 0 && toFree->childrenHaveContent == 0 && hasChildren == 0)
		|| freeEntireTrie == 1)
	{
		free(toFree->key->key);
		free(toFree->key);
		free(toFree->children);
		
		//Only called in the instance of freeing entire trie
		if(toFree->content != 0)
		{
			free(toFree->content);
		}
	
		free(toFree);
	
		if(freeEntireTrie == 0)
		{
			DEBUG_LOG(3, "internalNodeFree found no content and is not marked as having children with content, freeing node with key %s\n", debugKey);
		}
		else
		{
			DEBUG_LOG(3, "internalNodeFree freed node in freeEntireTrie operation for key %s\n", debugKey);
		}
		FREE_DEBUG_KEY(debugKey);
		return TRIE_NODEFREE_STATUS_FREED;
	}
	else
	{
		DEBUG_LOG(3, "internalNodeFree marked as having children with content or node has content, retaining node with key %s\n", debugKey);
		FREE_DEBUG_KEY(debugKey);
		return TRIE_NODEFREE_STATUS_NOTFREED;
	}
}

//Public
//Will free any content pointed to within the tree.
void trie_destroy(trie *toDestroy)
{
	//DEBUG_LOG(2, "Entering trie_destroy\n");
	int i;
	for(i = 0; i < TRIE_CHILDREN_PER_NODE; i++)
	{
		trie_freeNode_internalNodeFree(toDestroy->nodes[i], 1);
	}

};

//Public
char *trie_getPrintableKey(trie_key *key)
{
	int i;
	char *printableKey = (char *)malloc(sizeof(char) * (key->length + 1));
	for(i = 0; i < key->length; i++)
	{
		printableKey[i] = key->key[i];
	}
	printableKey[key->length] = '\0';
	return printableKey;
}
