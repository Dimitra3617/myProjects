typedef struct server_node {
	int rank;
	int extra_path;
	struct server_node* next;
}server_node;

typedef struct servers_leader_election {
	int sent;
	int connected;
	int leader_id;
	int counter;
	server_node* header;
	server_node* tail;
}servers_leader_election;

typedef struct storage {
	int file_id;
	int file_version;
	struct storage* next;
}storage;

enum type {upload, retrieve, update};

typedef struct fifo_node {
	int client_id;
	int count;
	enum type request;
	int vers;
	struct fifo_node* next;
}fifo_node;

typedef struct hashtable_node {
	int file_id;
	fifo_node* front;
	fifo_node* back;
}hashtable_node;