/* Dimitra Ktistaki, csd3617 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "BinarySearchTree.h"

int N;
int tree_size = 0;

pthread_barrier_t barrier_1st_phase;
pthread_barrier_t barrier_1st_phase_prints;
pthread_barrier_t barrier_2nd_phase;
pthread_barrier_t barrier_3nd_phase;
pthread_barrier_t barrier_4th_phase;

treeNode *tree_root = NULL;
queuenode_info *list_head = NULL;
int count_nodes = 0;

void *work(void *arg) {
	int j = (uintptr_t) arg;
	int load_songID;
	int search_songID;
	int selected_queue[N];
	int i;
  
	for(i=0; i<N; i++)
		selected_queue[i] = -1;

	for (i = 0; i < N; ++i) {
		load_songID = i * N + j;
		printf("Thread: %d is inserting Song: %d \n", j, load_songID);
		Insert(load_songID, NULL);
	} 
	
	pthread_barrier_wait(&barrier_1st_phase);

	if(j == 0) {

		printf("\n\n------------------FIRST CHECK------------------");
		int Y = Treesizecheck(tree_root);
		if(tree_size == Y)
			printf("\n~~>Total size check passed (expected: %d, found: %d)\n", tree_size, Y );
		else {
			fprintf(stderr, "\n~~>Total size check failed (expected: %d, found: %d)\n", tree_size, Y );
			exit(-1);
		}

		Y = Totalkeysumcheck(tree_root);
		if(tree_size*(N-1)*(N+1)/2 == Y) 
			printf("~~>Total keysum check passed (expected: %d, found: %d)\n", tree_size*(N-1)*(N+1)/2, Y );
		else {
			fprintf(stderr, "~~>Total keysum check failed (expected: %d, found: %d)\n", tree_size*(N-1)*(N+1)/2, Y );
			exit(-1);
		}

	}

	pthread_barrier_wait(&barrier_1st_phase_prints);

	int found = 0;
	int queue_id;
	for (i = N*j; i <= (N*j)+(N-1); i++) {
		search_songID = i;
		printf("\nThread: %d is searching Song: %d ", j, search_songID);
		found = Search(search_songID, NULL);
		if(found) {
			if(selected_queue[j] == -1) {
				queue_id = (j+1)%(N/2);
				enqueue(search_songID, queue_id);
			}
			else {
				if(selected_queue[j]+1 < N/2) {
					queue_id = selected_queue[j] + 1;
				}
				else {
					queue_id = 0;
				}
				enqueue(search_songID, queue_id);
			}
			printf(" Insert it into queue: %d", queue_id);
			selected_queue[j] = queue_id;
		}
	} 

	pthread_barrier_wait(&barrier_2nd_phase);
	
	int count = 0;
	if(j == 0) {
		printf("\n\n------------------SECOND CHECK------------------");
		count = Queuesizecheck(N/2);
		if(count == tree_size)
			printf("\n~~>Queues’ total size check passed (expected: %d, found: %d)\n", tree_size, count);
		else {
			fprintf(stderr, "~~>Queues’ total size check failed (expected: %d, found: %d)\n", tree_size, count);
			exit(-1);
		}

		count = Totalkeysumcheck2(N/2);
		if(count == tree_size*(N-1)*(N+1)/2) 
			printf("~~>Total keysum check passed (expected: %d, found: %d)", tree_size*(N-1)*(N+1)/2, count);
		else {
			fprintf(stderr, "~~>Total keysum check failed (expected: %d, found: %d)", tree_size*(N-1)*(N+1)/2, count);
			exit(-1);
		}

		printf("\n\nThe tree contains these songs:\n");
		printInorder(tree_root);

		printf("\n\nThe queues contain these songs:");
		printqueues(N/2);
	}

	for(i=0; i<N; i++)
		selected_queue[i] = -1;

	found = 0;
	count = 0;

	pthread_barrier_wait(&barrier_3nd_phase);

	for (i=0; i<N/2; ++i) {
		if(selected_queue[j] == -1) {
			queue_id = (j+1)%(N/2);
		}
		else {
			if(selected_queue[j]+1 < N/2) {
				queue_id = selected_queue[j] + 1;
			}
			else {
				queue_id = 0;
			}
		}
		search_songID = dequeue(queue_id);
		printf("\nThread: %d Delete from queue: %d", j , queue_id);
		selected_queue[j] = queue_id;
		found = Search(search_songID, NULL);
		if(found) {
			//Delete(search_songID, NULL);
		}
		LL_Insert(search_songID);
	}

	pthread_barrier_wait(&barrier_4th_phase); 

	if(j == 0) {
	  
	  printf("\n\nThe tree contains these songs:\n");
	  printInorder(tree_root);
	  
	  printf("\n\nThe queues contain these songs:");
	  printqueues(N/2);
	  
	  printf("\nThe list contains: \n");
	  printlist();
	  
		printf("\n\n------------------LAST CHECK------------------");
	  
		count = Queuesizecheck(N/2);
		if(count == tree_size/2)
			printf("\n~~>Queues’ total size check passed (expected: %d, found: %d)\n", tree_size/2, count);
		else {
			fprintf(stderr, "\n~~>Queues’ total size check failed (expected: %d, found: %d)\n", tree_size/2, count);
			exit(-1);
		}

		count = Listssizecheck();
		if(count == tree_size/2)
			printf("~~>List’s size check passed  (expected: %d, found: %d)\n", tree_size/2, count);
		else {
			fprintf(stderr, "~~>List’s size check failed (expected: %d, found: %d)\n", tree_size/2, count);
			exit(-1);
		}

		count_nodes = 0;
		count = Treesizecheck(tree_root);
		if(tree_size/2 == count)
			printf("~~>Tree's size check passed (expected: %d, found: %d)\n", tree_size/2, count );
		else {
			fprintf(stderr, "~~>Tree's size check failed (expected: %d, found: %d)\n", tree_size/2, count);
			exit(-1);
		}
	} 
}
    
int main(int argc, char const **argv) {
  int i;
  
	if(argc != 2) {
		printf("Type number of threads next time..\n");
		exit(-1);
	}

	N = atoi(argv[1]);
	if(N%2 == 1 || N <= 0) {
		printf("Only even integers are accepted except zero..\n");
		exit(-1);
	}
	tree_size = N * N;

	init_queues(N/2);

	pthread_barrier_init(&barrier_1st_phase, NULL, N);
	pthread_barrier_init(&barrier_1st_phase_prints, NULL, N);
	pthread_barrier_init(&barrier_2nd_phase, NULL, N);
	pthread_barrier_init(&barrier_3nd_phase, NULL, N);
	pthread_barrier_init(&barrier_4th_phase, NULL, N);

	pthread_t threads[N];

	for ( i = 0; i < N; ++i) {
		pthread_create(&threads[i], NULL, work, (void *)(intptr_t) i);
	}	
	
	for (i = 0; i < N; ++i) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
