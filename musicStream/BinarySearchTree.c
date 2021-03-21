#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#include "BinarySearchTree.h"

#define FALSE 0
#define TRUE 1

pthread_mutex_t tree_lock;
int sum = 0;

treeNode* newNode(int song_id) {

	treeNode* temp = (treeNode*)malloc(sizeof(treeNode));

	if(temp == NULL) {
		fprintf(stderr, "Couldnt allocate memory for new node");
		return temp;
	}

	temp->songID = song_id;
	temp->lc = NULL;
	temp->rc = NULL;
	pthread_mutex_init(&temp->lock, NULL);

	//printf("Allocation of new node done\n");

	return temp;
}

void Insert(int song_id, treeNode* root) { // first call always with root=NULL

	if(root == 	NULL) {

		pthread_mutex_lock(&tree_lock);

		if(tree_root == NULL) {
			tree_root = newNode(song_id);
			pthread_mutex_unlock(&tree_lock);
			return;
		}

		// if tree isn't empty, store the root and go on
		pthread_mutex_lock(&tree_root->lock);
		root = tree_root; 
		pthread_mutex_unlock(&tree_lock);	
	}

	if(song_id < root->songID) {
		if (root->lc == NULL) {
			root->lc = newNode(song_id);
			pthread_mutex_unlock(&root->lock);
		} else {
			pthread_mutex_lock(&root->lc->lock);
			pthread_mutex_unlock(&root->lock);
			Insert(song_id, root->lc); 
		}
	}
	else if (song_id > root->songID) {
		if (root->rc == NULL) {
			root->rc = newNode(song_id);
			pthread_mutex_unlock(&root->lock);
		} else {
			pthread_mutex_lock(&root->rc->lock);
			pthread_mutex_unlock(&root->lock);
			Insert(song_id, root->rc);
		}
	} else {
		printf("This song already exists!");
		assert(0);
	}	
}

int Search(int song_id, treeNode* root) {

	if(root == 	NULL) {
		pthread_mutex_lock(&tree_lock);

		if(tree_root == NULL) {
			printf("Search failed for song with songID %d\n", song_id);
			pthread_mutex_unlock(&tree_lock);
			return 0;
		}

		pthread_mutex_lock(&tree_root->lock);
		root = tree_root; 
		pthread_mutex_unlock(&tree_lock);	
	}

	if(song_id < root->songID) {
		if (root->lc == NULL) {
			printf("Search failed for node with value %d\n", song_id);
			pthread_mutex_unlock(&root->lock);
			return 0;
		} else {
			pthread_mutex_lock(&root->lc->lock);
			pthread_mutex_unlock(&root->lock);
			Search(song_id, root->lc); 
		}
	}
	else if (song_id > root->songID) {
		if (root->rc == NULL) {
			printf("Search failed for node with value %d\n", song_id);
			pthread_mutex_unlock(&root->lock);
			return 0;
		} else {
			pthread_mutex_lock(&root->rc->lock);
			pthread_mutex_unlock(&root->lock);
			Search(song_id, root->rc);
		}
	} else {
		pthread_mutex_unlock(&root->lock);
		return 1;
	}	
}

treeNode* FindMin(treeNode* root) {

	while(root->lc != NULL) {
		root = root->lc;
	}

 	return root;
}

treeNode* Delete(int song_id, treeNode* root) {

	if(root == 	NULL) {
		pthread_mutex_lock(&tree_lock);

		if(tree_root == NULL) {
			printf("Delete failed for song with songID %d\n", song_id);
			pthread_mutex_unlock(&tree_lock);
			return NULL;
		}

		pthread_mutex_lock(&tree_root->lock);
		root = tree_root; 
		pthread_mutex_unlock(&tree_lock);	
	}

	if(song_id < root->songID) {
		pthread_mutex_lock(&root->lc->lock);
		pthread_mutex_unlock(&root->lock);
		root->lc = Delete(song_id, root->lc);
	}
	else if (song_id > root->songID) {
		pthread_mutex_lock(&root->rc->lock);
		pthread_mutex_unlock(&root->lock);
		root->rc = Delete(song_id, root->rc);
	} else {

		if (root->lc == NULL && root->rc == NULL) {
			root = NULL;
		}
		else if (root->lc == NULL) {
			if(root == tree_root) {
				tree_root = root->rc;
			}

			root = root->rc;
			pthread_mutex_unlock(&root->lock);
		}
		else if(root->rc == NULL) {
			if(root == tree_root) {
				tree_root = root->lc;
				pthread_mutex_unlock(&tree_root->lock);	
			}

			root = root->lc;
			pthread_mutex_unlock(&root->lock);
		}
		else {
			treeNode* replace_with = FindMin(root->rc);
			root->songID = replace_with->songID;
			pthread_mutex_lock(&root->rc->lock);
			root->rc = Delete(replace_with->songID, root->rc);
			pthread_mutex_unlock(&root->lock);
		}
		
		return root;
	}	
} 

/* Given a binary tree, print its nodes in inorder*/
void printInorder(treeNode* node) {

    if (node == NULL) 
        return; 
  
    /* first recur on left child */
    printInorder(node->lc); 

    /* then print the data of node */
    printf("%d ", node->songID); 
  
    /* now recur on right child */
    printInorder(node->rc); 
} 
  

int Treesizecheck(treeNode* node) {

	if (node == NULL) 
        return count_nodes; 

	count_nodes = count_nodes + 1;  

    /* first recur on left child */
    Treesizecheck(node->lc); 

    /* now recur on right child */
    Treesizecheck(node->rc);

}

int Totalkeysumcheck(treeNode* node) {
	if (node == NULL) 
        return sum; 
  
    /* first recur on left child */
    Totalkeysumcheck(node->lc); 

    /* then print the data of node */
    sum = sum + node->songID; 
  
    /* now recur on right child */
    Totalkeysumcheck(node->rc); 
} 

void init_queues(int M) {
  int i ;
	head = NULL;

	queuenode_info* prev = NULL;

	for(i = 0; i<M; i++) {
		queuenode_info* info_node = (queuenode_info*)malloc(sizeof(queuenode_info));

		if(info_node == NULL) {
			fprintf(stderr, "Couldnt allocate memory for list");
			return;
		}

		info_node->queueID = i;
		queue* queue_node_entry = (queue*)malloc(sizeof(queue));
		if(info_node == NULL) {
			fprintf(stderr, "Couldnt allocate memory for new queue");
			return;
		}
		queue_node_entry->Head = NULL;
		queue_node_entry->Tail = NULL;
		pthread_mutex_init(&queue_node_entry->headLock, NULL);
		pthread_mutex_init(&queue_node_entry->tailLock, NULL);

		info_node->queue_pointer = queue_node_entry;
		info_node->next = NULL;

		if(i==0) 
			list_head = info_node;
		else 
			prev->next = info_node;

		prev = info_node;
	}
	prev->next = NULL;
}

void enqueue(int song_id, int queue_id) { // queue_id = where to insert

	queueNode* item = (queueNode*)malloc(sizeof(queueNode));

	if(item == NULL) {
		fprintf(stderr, "Couldnt allocate memory for new item");
		return;
	}

	item->songID = song_id;
	item->next = NULL;

	queuenode_info* temp = list_head;
	while(temp != NULL && temp->queueID != queue_id){
		temp = temp->next;
	}

	if(temp==NULL) {
		fprintf(stderr, "This queue does not exist\n");
		exit(-1);
	}

	pthread_mutex_lock(&temp->queue_pointer->tailLock);

	if(temp->queue_pointer->Head == NULL && temp->queue_pointer->Tail == NULL) {
		pthread_mutex_lock(&temp->queue_pointer->headLock);
		temp->queue_pointer->Head = item;
		temp->queue_pointer->Tail = item;
		pthread_mutex_unlock(&temp->queue_pointer->headLock);
	}
	else {
		temp->queue_pointer->Tail->next = item;
		temp->queue_pointer->Tail = item;
	}
	pthread_mutex_unlock(&temp->queue_pointer->tailLock);
}

int dequeue(int queue_id) { 

	queuenode_info* temp = list_head;
	while(temp != NULL && temp->queueID != queue_id){
		temp = temp->next;
	}

	if(temp==NULL) {
		fprintf(stderr, "This queue does not exist\n");
		exit(-1);
	}

	pthread_mutex_lock(&temp->queue_pointer->headLock);

	int delete_song_id = temp->queue_pointer->Head->songID;

	if(temp->queue_pointer->Head->next==NULL) {
		temp->queue_pointer->Head = NULL;
		temp->queue_pointer->Tail = NULL;
	}
	else {
		temp->queue_pointer->Head = temp->queue_pointer->Head->next;
	}

	pthread_mutex_unlock(&temp->queue_pointer->headLock);
	return delete_song_id;
}

int Queuesizecheck(int M){
	int count = 0;
	queuenode_info *temp = list_head;
  int i;
	queueNode* traverse = temp->queue_pointer->Head;

	for(i=0; i<M; i++) {	
		while(traverse!=NULL) {
			count = count + 1;
			traverse = traverse->next;
		}
		if(temp->next != NULL)
			traverse = temp->next->queue_pointer->Head;
		else 
			break;	
	}

	return count;
}

int Totalkeysumcheck2(int M){
	int sum = 0;
	queuenode_info *temp = list_head;
  int i;
	queueNode* traverse = temp->queue_pointer->Head;

	for(i=0; i<M; i++) {	
		while(traverse!=NULL) {
			sum = sum + traverse->songID;
			traverse = traverse->next;
		}
		if(temp->next != NULL)
			traverse = temp->next->queue_pointer->Head;	
		else
			break;
	}

	return sum;
}

void printqueues(int M) {
	queuenode_info *temp = list_head;
	queueNode* traverse;
  int i;
  
	for(i=0; i<M; i++) {	

		printf("\nQueue %d contains:\n", i);
		traverse = temp->queue_pointer->Head;

		while(traverse!=NULL) {
			printf("%d ", traverse->songID);
			traverse = traverse->next;
		}

		if(temp->next != NULL)
			temp = temp->next;
		else
			break;
		
	}
	printf("\n");
}

bool validate(listNode* pred, listNode* curr) {

	listNode* tmp = head;

	while(tmp->songID <= pred->songID) {
		if (tmp == pred)  {
			if(pred->next == curr)
				return TRUE;
			else 
				return FALSE;
		}
		tmp = tmp->next;
	}
	return FALSE;
}

int LL_Insert(int song_id) {
	listNode* pred;
	listNode* curr;
	int result;
	int return_flag = 0;

	if(head == NULL) {
		listNode* temp = (listNode*)malloc(sizeof(listNode));
		temp->songID = song_id;
		pthread_mutex_init(&temp->lock, NULL);
		temp->next = NULL;
		head = temp;
		return 1;
	}
	
	while(1) {
		pred = head;
		curr = pred;
		while(curr != NULL && curr->songID < song_id){
			pred = curr;
			curr = curr->next;
		}
		pthread_mutex_lock(&pred->lock);
		if(curr != NULL && curr != pred)
			pthread_mutex_lock(&curr->lock);
		if(validate(pred, curr) == TRUE || curr == head) {
			if(curr != NULL && song_id == curr->songID) {
				result = 0;
				return_flag = 1;
			}
			else {
				listNode* temp = (listNode*)malloc(sizeof(listNode));
				temp->next = curr;
				temp->songID = song_id;
				pthread_mutex_init(&temp->lock, NULL);
				if(pred != curr)
					pred->next = temp;
				else
					head = temp;
				result = 1;
				return_flag = 1;
			}
		}

		pthread_mutex_unlock(&pred->lock);
		if(curr != NULL && curr != pred)
			pthread_mutex_unlock(&curr->lock);

		if(return_flag) return result;
	}
}

void printlist(){
	listNode* curr = head;	

	while(curr!=NULL) {
		printf("%d ", curr->songID );
		curr = curr->next;
	}
	printf("\n");
	return;
}

int Listssizecheck() {
	listNode* curr = head;	
	int count = 0;

	while(curr!=NULL) {
		count++;
		curr = curr->next;
	}

	return count;
}
