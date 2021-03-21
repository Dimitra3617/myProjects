typedef struct treeNode {
	int songID; 
	struct treeNode *lc;
	struct treeNode *rc;
	pthread_mutex_t lock;
}treeNode;

extern treeNode *tree_root;
extern int count_nodes;

treeNode* newNode(int song_id);
void Insert(int song_id, treeNode* root);
int Search(int song_id, treeNode* root);
treeNode* Delete(int song_id, treeNode* root);

void printInorder(treeNode* node); 
int Treesizecheck(treeNode* node);
int Totalkeysumcheck(treeNode* node);


typedef struct queue {
	struct queueNode *Head;
	struct queueNode *Tail;
	pthread_mutex_t headLock;      
	pthread_mutex_t tailLock;
}queue;

typedef struct queuenode_info {
	int queueID;
	queue* queue_pointer;
	struct queuenode_info* next;
}queuenode_info;

extern queuenode_info *list_head;

typedef struct queueNode {
	int songID;
	struct queueNode *next;
}queueNode;

void enqueue(int song_id, int queue_id);
int dequeue(int queue_id);
void init_queues(int M);
int Queuesizecheck(int M);
int Totalkeysumcheck2(int M);
void printqueues(int M);

typedef struct listNode {
	int songID;
	struct listNode *next;
	pthread_mutex_t lock;
}listNode;

listNode *head;

void init_list();
int LL_Insert(int song_id);
void printlist();
int Listssizecheck();
