#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "header.h"
#include <time.h>

#define SIZE 500

char *test_file;
int num_tasks, num_servers, num_clients;

enum message { SERVER, ACK, LEFT_NEIGHBOR, RIGHT_NEIGHBOR, START_LEADER_ELECTION, LEADER_ELECTION_DONE, CLIENT, 
CANDIDATE_ID,CONNECT, UPLOAD, UPLOAD_FAILED, UPLOAD_ACK, UPLOAD_OK, RETRIEVE, RETRIEVE_FAILED, RETRIEVE_ACK, RETRIEVE_OK,
UPDATE, UPDATE_ACK, UPDATE_OK, UPDATE_FAILED, VERSION_CHECK, VERSION_OUTDATED, REQUEST_SHUTDOWN, SHUTDOWN_OK };

void Coordinator() {
	FILE *file;
	file = fopen(test_file,"r");
	char buffer[20];
	int server_rank, left_neigh_rank, right_neigh_rank, i, dummy_int, j=0, leader, client_rank, file_id, 
	client_rank2, file_id2, client_rank3, file_id3, x; 
	MPI_Status status3;

	while( fscanf(file, "%s", buffer) != EOF) {

		if((strcmp(buffer, "SERVER")) == 0) {

			fscanf (file, "%d", &server_rank);
		   	fscanf (file, "%d", &left_neigh_rank);
		   	fscanf (file, "%d", &right_neigh_rank);
		   	printf("SERVER <%d> <%d> <%d>\n", server_rank, left_neigh_rank, right_neigh_rank);

		   	MPI_Send(&dummy_int, 1, MPI_INT, server_rank, SERVER, MPI_COMM_WORLD);
		   	MPI_Recv(&dummy_int, 1, MPI_INT, server_rank, ACK, MPI_COMM_WORLD, 0);

		   	MPI_Send(&left_neigh_rank, 1, MPI_INT, server_rank, LEFT_NEIGHBOR, MPI_COMM_WORLD);
		   	MPI_Recv(&dummy_int, 1, MPI_INT, server_rank, ACK, MPI_COMM_WORLD, 0);

		   	MPI_Send(&right_neigh_rank, 1, MPI_INT, server_rank, RIGHT_NEIGHBOR, MPI_COMM_WORLD);
		   	MPI_Recv(&dummy_int, 1, MPI_INT, server_rank, ACK, MPI_COMM_WORLD, 0);

		}
		else if ((strcmp(buffer, "START_LEADER_ELECTION")) == 0) {

			for(i=0; i<num_tasks; i++)
				MPI_Send(&dummy_int, 1, MPI_INT, i, START_LEADER_ELECTION, MPI_COMM_WORLD);

			MPI_Recv(&leader, 1, MPI_INT, MPI_ANY_SOURCE, LEADER_ELECTION_DONE, MPI_COMM_WORLD, 0);
			
			for(i=1; i<num_tasks; i++) {
				MPI_Send(&leader, 1, MPI_INT, i, CLIENT, MPI_COMM_WORLD);
				MPI_Recv(&dummy_int, 1, MPI_INT, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, 0);
			}
			
		}
		else if ((strcmp(buffer, "UPLOAD")) == 0) {

			fscanf (file, "%d", &client_rank);
		   	fscanf (file, "%d", &file_id);
		   	MPI_Send(&file_id, 1, MPI_INT, client_rank, UPLOAD, MPI_COMM_WORLD);

		}
		else if ((strcmp(buffer, "RETRIEVE")) == 0) {

			fscanf (file, "%d", &client_rank2);
		   	fscanf (file, "%d", &file_id2);
		   	MPI_Send(&file_id2, 1, MPI_INT, client_rank2, RETRIEVE, MPI_COMM_WORLD);

		}	
		else if ((strcmp(buffer, "UPDATE")) == 0) {

			fscanf (file, "%d", &client_rank3);
		   	fscanf (file, "%d", &file_id3);
		   	MPI_Send(&file_id3, 1, MPI_INT, client_rank3, UPDATE, MPI_COMM_WORLD);

		}
	}

	// stelnw se oles alla oses den einai client vlepoun to -1 k to agnooun
	for (i = 1; i < num_tasks; i++) {
		dummy_int = -1;
		MPI_Send(&dummy_int, 1, MPI_INT, i, REQUEST_SHUTDOWN, MPI_COMM_WORLD);
	}

	for (x = 0; x < num_clients; ++x) {
		MPI_Recv(&dummy_int, 1, MPI_INT, MPI_ANY_SOURCE, SHUTDOWN_OK, MPI_COMM_WORLD, 0);
	}

	dummy_int = 0;
	MPI_Send(&dummy_int, 1, MPI_INT, leader, REQUEST_SHUTDOWN, MPI_COMM_WORLD);
	MPI_Recv(&dummy_int, 1, MPI_INT, leader, SHUTDOWN_OK, MPI_COMM_WORLD, 0);
}	

int main(int argc, char *argv[]){

	int rank, tmp[3], i = 0, x;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

	MPI_Status status, status1, status2, status3;
	num_servers = atoi(argv[1]);
	test_file = argv[2];

	num_clients = num_tasks - num_servers - 1;
	int isServer = 0, right_neighbor, left_neighbor, client_active_requests = 0, res[3];
	servers_leader_election  leader_election;
	storage* client_directory = NULL;
	storage* server_directory = NULL;
	hashtable_node hashtable[SIZE];
	server_node* traverse;

	leader_election.leader_id = -1;
	srand ( time(NULL) );


	for (i = 0; i < SIZE; ++i) {
		hashtable[i].file_id = 0;
		hashtable[i].front = NULL;
		hashtable[i].back = NULL;
	}

	if(rank == 0) {
		Coordinator();
	}
	else {
		while(1){

			MPI_Recv(&tmp, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			if(status.MPI_TAG == SERVER) {
				isServer = 1;
				leader_election.sent = 0;
				leader_election.connected = 0;
				leader_election.leader_id = rank;
				leader_election.counter = 0;
				leader_election.header = NULL;
				leader_election.tail = NULL;

				MPI_Send(&rank, 1, MPI_INT, 0, ACK, MPI_COMM_WORLD);
			} 
			else if (status.MPI_TAG == LEFT_NEIGHBOR) {
				left_neighbor = tmp[0];
				MPI_Send(&rank, 1, MPI_INT, 0, ACK, MPI_COMM_WORLD);
			}
			else if (status.MPI_TAG == RIGHT_NEIGHBOR) {
				right_neighbor = tmp[0];
				MPI_Send(&rank, 1, MPI_INT, 0, ACK, MPI_COMM_WORLD);
			}
			else if (status.MPI_TAG == START_LEADER_ELECTION) {
				if(isServer == 1) {
					if (leader_election.sent == 0) {
						MPI_Send(&rank, 1, MPI_INT, left_neighbor, CANDIDATE_ID, MPI_COMM_WORLD);
						leader_election.sent = 1;
					}
				}
			}
			else if (status.MPI_TAG == CANDIDATE_ID) {
				if (leader_election.sent == 0) {
					MPI_Send(&rank, 1, MPI_INT, left_neighbor, CANDIDATE_ID, MPI_COMM_WORLD);
					leader_election.sent = 1;
				}

				if (leader_election.counter < num_servers ) {	

					server_node* new_node = malloc (sizeof(new_node));
					new_node->rank = tmp[0];
					new_node->extra_path = 0;
					new_node->next = NULL;
					leader_election.counter++; 

					if (leader_election.header == NULL) {
						leader_election.header = new_node;
						leader_election.tail = new_node;
					}
					else {
						leader_election.tail->next = new_node;
						leader_election.tail = new_node;
					}
					if (tmp[0] > leader_election.leader_id) leader_election.leader_id = tmp[0];
					if (tmp[0] != rank) MPI_Send(&tmp, 1, MPI_INT, left_neighbor, CANDIDATE_ID, MPI_COMM_WORLD);
				}

				if(leader_election.counter == num_servers) {
					printf("RANK: %d , LEADER_ID: %d, DAKTILIOS: ", rank, leader_election.leader_id );
					traverse = leader_election.header;
					while(traverse != NULL) {
						printf("%d ", traverse->rank);
						traverse = traverse->next;
					}
					printf("\n");

					// egw eimai o arxigoss
					if (leader_election.leader_id == rank) {
						leader_election.connected = 2;
						int k = (num_servers - 3) / 4;
						i=0;
						while(i<k) {
							
							int selected =  (rand() % (num_servers-2 - 2 + 1)) + 2;
							i++;

							traverse = leader_election.header;
							for (x = 1; x < selected; x++) {
								traverse = traverse->next;
							}
							traverse->extra_path = 1;

							printf("~~~> Send CONNECT to server with position: %d His rank: %d\n",selected, traverse->rank );
							MPI_Send(&tmp, 1, MPI_INT, traverse->rank, CONNECT, MPI_COMM_WORLD);
							MPI_Recv(&tmp, 1, MPI_INT, traverse->rank, ACK, MPI_COMM_WORLD, 0);
						}
						MPI_Send(&rank, 1, MPI_INT, 0, LEADER_ELECTION_DONE, MPI_COMM_WORLD);
					}
				}
			}	
			else if (status.MPI_TAG == CONNECT) {
				leader_election.connected = 1;
				printf("~~~> SERVER: %d CONNECTED TO LEADER: %d\n", rank, leader_election.leader_id);
				MPI_Send(&rank, 1, MPI_INT, leader_election.leader_id, ACK, MPI_COMM_WORLD);
			}
			else if (status.MPI_TAG == CLIENT) {
				if (isServer == 0) 
					leader_election.leader_id = tmp[0];
				
				MPI_Send(&rank, 1, MPI_INT, 0, ACK, MPI_COMM_WORLD);
			}
			else if (status.MPI_TAG == UPLOAD) {
				if(isServer == 0) {
					storage* traverse = client_directory;
					while(traverse != NULL) {
						if (tmp[0] == traverse->file_id) 
							break;
						traverse = traverse->next;
					}

					if(traverse != NULL) {
						traverse->file_id = tmp[0];
						traverse->file_version = 1;
					}
					else {
						storage* new_node = malloc (sizeof(storage));
						new_node->file_id = tmp[0];
						new_node->file_version = 1;
						if(client_directory == NULL) {
							new_node->next = NULL;
							client_directory = new_node;
						}	
						else {
							new_node->next = client_directory;
							client_directory = new_node;
						}
					}
					res[0] = tmp[0];
					res[1] = rank;
					client_active_requests++;
					MPI_Send(&res, 2, MPI_INT, leader_election.leader_id, UPLOAD, MPI_COMM_WORLD);
					MPI_Recv(&tmp, 1, MPI_INT, leader_election.leader_id, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);

					if(status1.MPI_TAG == UPLOAD_OK) 
						printf("CLIENT %d UPLOADED %d\n", rank , tmp[0]);
					else if (status1.MPI_TAG == UPLOAD_FAILED) 
						printf("CLIENT %d FAILED TO UPLOAD %d\n", rank , tmp[0]);
					
					client_active_requests--;
				}
				else if (rank == leader_election.leader_id) { //eimai o arxigos
					int found = 0;

					for (i = 0; i < SIZE; ++i) {
						if(hashtable[i].file_id == tmp[0]) {
							found = 1;
							break;
						}
					}

					if(found == 1) 
						MPI_Send(&tmp, 1, MPI_INT, tmp[1] , UPLOAD_FAILED, MPI_COMM_WORLD);
					else {
						int position = tmp[0] % SIZE;
						int receivers = (num_servers-1)/2 + 1;
						hashtable[position].file_id = tmp[0];
						
						fifo_node* new_fifo_node = malloc(sizeof(fifo_node));
						new_fifo_node->client_id = tmp[1]; 
						new_fifo_node->count = receivers;
						new_fifo_node->request = upload;
						new_fifo_node->vers = 0;
						new_fifo_node->next = NULL;
						
						if (hashtable[position].front == NULL) {
							hashtable[position].front = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}
						else {
							hashtable[position].back->next = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}

						i=0;

						while(i<receivers) {
					
							int selected =  (rand() % (num_servers-1 - 1 + 1)) + 1;		
							i++;

							traverse = leader_election.header;
							for (x = 1; x < selected; x++) 
									traverse = traverse->next;

							res[0] = traverse->rank;
							res[1] = tmp[0];
							if (traverse->extra_path == 1) 
								MPI_Send(&res, 2, MPI_INT, traverse->rank, UPLOAD, MPI_COMM_WORLD);
							else {
								server_node* search = traverse;
								search = search->next;
								while(search->rank != rank ) {
									if (search->extra_path == 1) {
										MPI_Send(&res, 2, MPI_INT, search->rank, UPLOAD, MPI_COMM_WORLD);
										break;
									}
									search = search->next;
								}
								if (search->rank == rank) 
									MPI_Send(&res, 2, MPI_INT, left_neighbor, UPLOAD, MPI_COMM_WORLD);
							}
						}
					}
				}
				else if (isServer == 1) {
					if (rank != tmp[0]) {
						res[0] = tmp[0];
						res[1] = tmp[1];
						MPI_Send(&res, 2, MPI_INT, left_neighbor, UPLOAD, MPI_COMM_WORLD);
					}
					else {
						storage* traverse = server_directory;
						while(traverse != NULL) {
							if (tmp[1] == traverse->file_id) 
								break;
							traverse = traverse->next;
						}
						
						if(traverse != NULL) {
							traverse->file_id = tmp[1];
							traverse->file_version = 1;
						}
						else {
							storage* new_node = malloc (sizeof(storage));
							new_node->file_id = tmp[1];
							new_node->file_version = 1;
							if(server_directory == NULL) {
								new_node->next = NULL;
								server_directory = new_node;
							}	
							else {
								new_node->next = server_directory;
								server_directory = new_node;
							}
						}

						tmp[0] = tmp[1];
						if (leader_election.connected == 1) 
							MPI_Send(&tmp, 1, MPI_INT, leader_election.leader_id, UPLOAD_ACK, MPI_COMM_WORLD);
						else 
							MPI_Send(&tmp, 1, MPI_INT, left_neighbor, UPLOAD_ACK, MPI_COMM_WORLD);						
					}
				}
			}
			else if (status.MPI_TAG == UPLOAD_ACK) {
				if (leader_election.leader_id == rank) { // eimai o arxigos
					int position = tmp[0] % SIZE; 
					
					if (hashtable[position].front->count > 1) 
						hashtable[position].front->count -- ;
					else {
						hashtable[position].front->count -- ;
						MPI_Send(&tmp, 1, MPI_INT, hashtable[position].front->client_id, UPLOAD_OK, MPI_COMM_WORLD);
						hashtable[position].front = hashtable[position].front->next;
						if (hashtable[position].front == NULL)
							hashtable[position].back = NULL;
					}
				}
				else {

					if(leader_election.connected == 1)
						MPI_Send(&tmp[0], 1, MPI_INT, leader_election.leader_id, UPLOAD_ACK, MPI_COMM_WORLD);
					else 
						MPI_Send(&tmp[0], 1, MPI_INT, left_neighbor, UPLOAD_ACK, MPI_COMM_WORLD);						
				}
			}
			else if (status.MPI_TAG == RETRIEVE) {
				if (isServer == 0) {
					res[0] = tmp[0]; // file_id
					res[1] = rank;
					client_active_requests++;
					MPI_Send(&res, 2, MPI_INT, leader_election.leader_id, RETRIEVE, MPI_COMM_WORLD);
					MPI_Recv(&tmp, 2, MPI_INT, leader_election.leader_id, MPI_ANY_TAG, MPI_COMM_WORLD, &status2);

					if (status2.MPI_TAG == RETRIEVE_FAILED) 
						printf("CLIENT %d FAILED TO RETRIEVE %d\n", rank, tmp[0]);
					else if (status2.MPI_TAG == RETRIEVE_OK)
						printf("CLIENT %d RETRIEVED VERSION %d OF %d\n", rank, tmp[1], tmp[0]);

					client_active_requests--;
				}
				else if(leader_election.leader_id ==  rank) {
					int position = tmp[0] % SIZE; 
					if (hashtable[position].file_id == 0) 
						MPI_Send(&tmp, 1, MPI_INT, tmp[1], RETRIEVE_FAILED, MPI_COMM_WORLD);
					else {
						int receivers = (num_servers-1)/2 + 1;
					
						fifo_node* new_fifo_node = malloc(sizeof(fifo_node));
						new_fifo_node->client_id = tmp[1]; 
						new_fifo_node->count = receivers;
						new_fifo_node->request = retrieve;
						new_fifo_node->vers = 0;
						new_fifo_node->next = NULL;
						
						if (hashtable[position].front == NULL) {
							hashtable[position].front = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}
						else {
							hashtable[position].back->next = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}

						i=0;
						while(i<receivers) {
							int selected =  (rand() % (num_servers-1 - 1 + 1)) + 1;
							i++;

							traverse = leader_election.header;
							for (x = 1; x < selected; x++) 
									traverse = traverse->next;

							res[0] = traverse->rank;
							res[1] = tmp[0]; //file_id
							if (traverse->extra_path == 1) 
								MPI_Send(&res, 2, MPI_INT, traverse->rank, RETRIEVE, MPI_COMM_WORLD);
							else {
								server_node* search = traverse;
								search = search->next;
								while(search->rank != rank ) {
									if (search->extra_path == 1) {
										MPI_Send(&res, 2, MPI_INT, search->rank, RETRIEVE, MPI_COMM_WORLD);
										break;
									}
									search = search->next;
								}
								if (search->rank == rank) 
									MPI_Send(&res, 2, MPI_INT, left_neighbor, RETRIEVE, MPI_COMM_WORLD);
							}
						}
					}
				}
				else if (isServer == 1) {
					if (rank != tmp[0]) {
						res[0] = tmp[0];
						res[1] = tmp[1]; //file_id
						MPI_Send(&res, 2, MPI_INT, left_neighbor, RETRIEVE, MPI_COMM_WORLD);
					}
					else {
						storage* traverse = server_directory;
						while(traverse != NULL) {
							if (tmp[1] == traverse->file_id) 
								break;
							traverse = traverse->next;
						}
						
						tmp[0] = tmp[1]; //file_id
						if(traverse != NULL) 
							tmp[1] = traverse->file_version;
						else 
							tmp[1] = 0;
							
						if (leader_election.connected == 1)
							MPI_Send(&tmp, 2, MPI_INT, leader_election.leader_id, RETRIEVE_ACK, MPI_COMM_WORLD);
						else
							MPI_Send(&tmp, 2, MPI_INT, left_neighbor, RETRIEVE_ACK, MPI_COMM_WORLD);
					}
				}
			}
			else if (status.MPI_TAG == RETRIEVE_ACK) {
				if (leader_election.leader_id == rank) {
					
					int position = tmp[0] % SIZE; 
							
					if (hashtable[position].front->vers < tmp[1]) 
						hashtable[position].front->vers = tmp[1];

					if (hashtable[tmp[0]].front->count > 1) 
						hashtable[position].front->count -- ;
					else {
						hashtable[position].front->count -- ;
						tmp[1] = hashtable[position].front->vers;
						MPI_Send(&tmp, 2, MPI_INT, hashtable[position].front->client_id, RETRIEVE_OK, MPI_COMM_WORLD);
						hashtable[position].front = hashtable[position].front->next;
						if (hashtable[position].front == NULL)
							hashtable[position].back = NULL;
					}
				}
				else if (leader_election.connected == 1) 
					MPI_Send(&tmp, 2, MPI_INT, leader_election.leader_id, RETRIEVE_ACK, MPI_COMM_WORLD);
				else 
					MPI_Send(&tmp, 2, MPI_INT, left_neighbor, RETRIEVE_ACK, MPI_COMM_WORLD);	
				
			}
			else if (status.MPI_TAG == UPDATE) {
				if (isServer == 0) {
					storage* traverse = client_directory;
					while(traverse != NULL) {
						if (tmp[0] == traverse->file_id) 
							break;
						traverse = traverse->next;
					}

					if (traverse != NULL) 
						res[1] = traverse->file_version;
					else 
						res[1] = 0;

					res[2] = rank;
					res[0] = tmp[0];
					client_active_requests++;
					MPI_Send(&res, 3, MPI_INT, leader_election.leader_id, UPDATE, MPI_COMM_WORLD);	
					MPI_Recv(&tmp, 3, MPI_INT, leader_election.leader_id, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);

					if(status3.MPI_TAG == UPDATE_FAILED) 
						printf("CLIENT %d UPDATE FAILED %d\n", rank , tmp[0]);
					else if (status3.MPI_TAG == VERSION_OUTDATED) 
						printf("CLIENT %d CANNOT UPDATE %d WITHOUT MOST RECENT VERSION\n", rank , tmp[0]);
					else if (status3.MPI_TAG == UPDATE_OK) 
						printf("CLIENT %d UPDATED %d\n", rank , tmp[0]); 
					client_active_requests--;
		
				}
				else if (leader_election.leader_id == rank) {
					
					int position = tmp[0] % SIZE; 
					if (hashtable[position].file_id == 0 || tmp[1] == 0) {
						res[0] = tmp[0];
						MPI_Send(&res, 1, MPI_INT, tmp[2], UPDATE_FAILED, MPI_COMM_WORLD);
					}
					else {
						fifo_node* new_fifo_node = malloc(sizeof(fifo_node));
						new_fifo_node->client_id = tmp[2]; 
						new_fifo_node->count = num_servers/2 + 1;
						new_fifo_node->request = update;
						new_fifo_node->vers = tmp[1];
						new_fifo_node->next = NULL;
						
						if (hashtable[position].front == NULL) {
							hashtable[position].front = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}
						else {
							hashtable[position].back->next = new_fifo_node;
							hashtable[position].back = new_fifo_node;
						}

						tmp[2] = 1;
						MPI_Send(&tmp, 3, MPI_INT, left_neighbor, VERSION_CHECK, MPI_COMM_WORLD);
					}
				}
				else if (isServer == 1) {
					if (rank != tmp[2]) {
						MPI_Send(&tmp, 3, MPI_INT, left_neighbor, UPDATE, MPI_COMM_WORLD);
					}
					else {
						storage* traverse = server_directory;
						while(traverse != NULL) {
							if (tmp[0] == traverse->file_id) 
								break;
							traverse = traverse->next;
						}

						if(traverse != NULL) {
							if (traverse->file_id <= tmp[1]) {
								traverse->file_version = tmp[1] + 1;
							}
						}
						else {
							storage* new_node = malloc (sizeof(storage));
							new_node->file_id = tmp[0];
							new_node->file_version = tmp[1] + 1;
							if(server_directory == NULL) {
								new_node->next = NULL;
								server_directory = new_node;
							}	
							else {
								new_node->next = server_directory;
								server_directory = new_node;
							}
						}
						res[0] = tmp[0];
						if(leader_election.connected == 1)
							MPI_Send(&res, 1, MPI_INT, leader_election.leader_id, UPDATE_ACK, MPI_COMM_WORLD);
						else 
							MPI_Send(&res, 1, MPI_INT, left_neighbor, UPDATE_ACK, MPI_COMM_WORLD);
					}
				}
			}
			else if (status.MPI_TAG == UPDATE_ACK) {	
				if (leader_election.leader_id != rank) {
					if(leader_election.connected == 1)
						MPI_Send(&tmp, 1, MPI_INT, leader_election.leader_id, UPDATE_ACK, MPI_COMM_WORLD);
					else 
						MPI_Send(&tmp, 1, MPI_INT, left_neighbor, UPDATE_ACK, MPI_COMM_WORLD);
				}
				else {
					int position = tmp[0] % SIZE; 
					
					if (hashtable[position].front->count > 1) {
						hashtable[position].front->count -- ;
					}
					else {
						hashtable[position].front->count -- ;
						MPI_Send(&tmp, 1, MPI_INT, hashtable[position].front->client_id, UPDATE_OK, MPI_COMM_WORLD);
						hashtable[position].front = hashtable[position].front->next;
						if (hashtable[position].front == NULL)
							hashtable[position].back = NULL;
					}
				}
			}
			else if (status.MPI_TAG == VERSION_CHECK) {	
				if (leader_election.leader_id == rank) {
					if (tmp[2] == 0) {
						int position = tmp[0] % SIZE;
						MPI_Send(&tmp, 1, MPI_INT, hashtable[position].front->client_id, VERSION_OUTDATED, MPI_COMM_WORLD);	
						hashtable[position].front = hashtable[position].front->next;
						if (hashtable[position].front == NULL)
							hashtable[position].back = NULL;	
					}
					else if (tmp[2] == 1) {
						int k = num_servers/2 + 1;
						i=0;
						while(i<k) {
							
							int selected =  (rand() % (num_servers-1 - 1 + 1)) + 1;
							i++;

							traverse = leader_election.header;
							for (x = 1; x < selected; x++) {
								traverse = traverse->next;
							}
							
							printf("~~~> Send UPDATE to server with position: %d His rank: %d\n",selected, traverse->rank );
							tmp[2] = traverse->rank;
							if (traverse->extra_path == 1) 
								MPI_Send(&tmp, 3, MPI_INT, traverse->rank, UPDATE, MPI_COMM_WORLD);
							else {
								server_node* search = traverse;
								search = search->next;
								while(search->rank != rank ) {
									if (search->extra_path == 1) {
										MPI_Send(&tmp, 3, MPI_INT, traverse->rank, UPDATE, MPI_COMM_WORLD);
										break;
									}
									search = search->next;
								}
								if (search->rank == rank) 
									MPI_Send(&tmp, 3, MPI_INT, left_neighbor, UPDATE, MPI_COMM_WORLD);
							}
						}
					}
				}
				else if (isServer == 1) {
					if (tmp[2] == 1) {

						storage* traverse = server_directory;
						while(traverse != NULL) {
							if (tmp[0] == traverse->file_id) 
								break;
							traverse = traverse->next;
						}
						if (traverse == NULL) {
							MPI_Send(&tmp, 3, MPI_INT, left_neighbor, VERSION_CHECK, MPI_COMM_WORLD);
						}
						else if(traverse->file_version <= tmp[1]) {
							MPI_Send(&tmp, 3, MPI_INT, left_neighbor, VERSION_CHECK, MPI_COMM_WORLD);
						}
						else {
							tmp[2] = 0;
							if(leader_election.connected == 1)
								MPI_Send(&tmp, 3, MPI_INT, leader_election.leader_id, VERSION_CHECK, MPI_COMM_WORLD);
							else 
								MPI_Send(&tmp, 3, MPI_INT, left_neighbor, VERSION_CHECK, MPI_COMM_WORLD);	
						}
					}
					else if (tmp[2] == 0) {

						if(leader_election.connected == 1)
							MPI_Send(&tmp, 3, MPI_INT, leader_election.leader_id, VERSION_CHECK, MPI_COMM_WORLD);
						else 
							MPI_Send(&tmp, 3, MPI_INT, left_neighbor, VERSION_CHECK, MPI_COMM_WORLD);	
					}
				}
				
			}
			else if (status.MPI_TAG == REQUEST_SHUTDOWN) {
				if (isServer == 0) {
					if (client_active_requests == 0) {
						tmp[0] = rank;
						MPI_Send(&tmp, 1, MPI_INT, 0, SHUTDOWN_OK, MPI_COMM_WORLD);	
						break;
					}
					else {
						while(1){
							if(client_active_requests == 0) break;
						}
						MPI_Send(&tmp, 1, MPI_INT, 0, SHUTDOWN_OK, MPI_COMM_WORLD);	
					}
				}
				else if (leader_election.leader_id == rank && tmp[0] != -1) {
					if (tmp[0] == 0) { // o sintonistis m esteile kai to prowthw ston daktilio
						tmp[0] = rank;
						MPI_Send(&tmp, 1, MPI_INT, left_neighbor, REQUEST_SHUTDOWN, MPI_COMM_WORLD);	
					}
					else {	//ean mou xanasteilei kapoios me rank!=0 einai o geitonas m kai ara o kiklos oloklirothike
						MPI_Send(&tmp, 1, MPI_INT, 0, SHUTDOWN_OK, MPI_COMM_WORLD);	
						break;
					}	
				} 
				else if((isServer == 1) && tmp[0] != -1) {
					MPI_Send(&tmp, 1, MPI_INT, left_neighbor, REQUEST_SHUTDOWN, MPI_COMM_WORLD);
					break;
				}
			}
		}
	}
	
	MPI_Finalize();

	return 0;
}
