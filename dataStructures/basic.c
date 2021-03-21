#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Zoo.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

int initialize() {
	animals_list = NULL;
	visitors_list = NULL;
	i=0;
	senti.vid=-1;
	senti.last_visit=-1;
	senti.visits=-1;
	senti.next=NULL;
	sentinal=&senti;
	em_senti.eid=-1;
	em_senti.eco_count=-1;
	em_senti.employee_ecos=NULL;
	em_senti.prev=NULL;
	em_senti.next=NULL;
	employees_sentinel=&em_senti;
	last_insert=NULL;
	new_empl=NULL;
	found=NULL;
	cross=NULL;
	firstem=NULL;
	while(i<50) { //arxikopoiw me 0 kai sta 50 ecosystem to plithos twn zwwn kai tous deiktes me NULL
		curr = &ecosystems_array[i];
		curr->animal_count = 0;
		curr->animals = NULL;
		i++;
	}
	return 1;
}

int print_ecos(void) {
	printf("\tECOSYSTEMS: \n");
        i=0;
        while(i<50) {
                printf("\n\t<ecosystem%d: %d>:", i, (ecosystems_array[i].animal_count));
                curr=&ecosystems_array[i];
                temp=curr->animals;
                while(temp!=NULL) {
                        printf(" <%d>", temp->aid);
                        temp=temp->next;
                }
                i=i+1;
        }
        printf("\nDONE\n");

        return 1;
}

int print_visitors(void) {
	temp1=visitors_list;
        printf("\tVisitors = ");
        while(temp1!=sentinal) {
                printf("<%d : %d : %d>, ", (temp1->vid), (temp1->visits), (temp1->last_visit));
                temp1=temp1->next;
        }
        printf("\nDONE\n");

        return 1;
}

int print_employees(void) {
	printf("\tEMPLOYEES:");
        do {
                printf("\n\t<employee%d: %d>:", (cross->eid),(cross->eco_count) );
                eco=cross->employee_ecos;
                for(i=0;i<cross->eco_count;i++) {
                        printf(" <%d>", (eco->eco_id)); //oxi hd!
                        eco=eco->next;
                }
                cross=cross->next;
        } while(cross->eid!=-1);
        cross=cross->next;
        printf("\nDONE\n");

        return 1;
}

int free_world() {
	current=animals_list; //lista zwwn
	while(current!=NULL) { 
		prev=current;
		current=current->next;
		free(prev);
	}		
	
	i=0;             //lista oikosustimatwn
    while(i<50) {
    	curr=&ecosystems_array[i];
        current=curr->animals;
        while(current!=NULL) { //h lista zwwn tou kathe oikosustimatos
        	prev=current;
            current=current->next;
            free(prev);
        }
		i=i+1;
	}

	temp1=visitors_list; //lista episkeptwn
	while(temp1!=sentinal) {
		temp2=temp1;
		temp1=temp1->next;
		free(temp2);
	}

	do {         //lista ergazomenwn
		tmp1=cross->employee_ecos;
		for(i=0;i<cross->eco_count;i++) {
			tmp2=tmp1;
			tmp1=tmp1->next;
			free(tmp2); //ecosystems
			cross->eco_count=cross->eco_count-1;
		}
		found=cross;
		cross=cross->next;
		free(found); //employee
	} while(cross->eid!=-1);

	printf("\nFree: succedeed\n");	
   	return 1;
}

int gold_visitors() {
	ptr=visitors_list;
	for(i=0;i<5;i++) {
		gold[i]=ptr;
		ptr=ptr->next;
	}		
	min=gold[0]->visits;
	thesi=0;
	for(i=1;i<5;i++) {
		if(gold[i]->visits<min) {
			min=gold[i]->visits;
			thesi=i;
		}
	}
	while(ptr->vid!=-1) {
		if(ptr->visits > min) {
			gold[thesi]=ptr;
			min=gold[0]->visits;
	        	thesi=0;
        		for(i=1;i<5;i++) {
                		if(gold[i]->visits<min) {
                        		min=gold[i]->visits;
                        		thesi=i;
                		}
        		}
		}
		ptr=ptr->next;
	}
	printf("Gold Visitors = ");
	for(i=0;i<5;i++) {
		printf("< %d: %d: %d >, ", (gold[i]->vid),(gold[i]->visits), (gold[i]->last_visit)  );
	}
	printf("\nDONE\n");
        return 1;
}

void change_pointers() {
	tmp1=cross->employee_ecos;
    cross->employee_ecos=found->employee_ecos;          
    tmp2=found->employee_ecos->next;       
    cross->employee_ecos->next=tmp1;
    found->employee_ecos=tmp2;      
    cross->eco_count=cross->eco_count+1;
    found->eco_count=found->eco_count-1;

}

int retire(int eid) {
	cross=firstem;
	do {
		if(cross->eid==eid) {
			break;
		}
		cross=cross->next;
	
	} while(cross!=firstem);
	if(cross==firstem) {
		firstem=firstem->next;
		printf("Header changed: %d \n", firstem->eid);
	}
	found=cross;
	i=0;
	while(i<3) {
		cross=cross->prev; //pigaine 3 theseis prin to stoixeio p tha diagrafei
		if(cross->eid!=-1) {
			i++;
		}
	}
	if(found->eco_count/6 == 0) {	//ean o employee exei ligotera apo 6 oikosustimata
		while(found->eco_count!=0) {
			if(cross->eid==-1){ 
				cross=cross->next;
			}	
			change_pointers();
			cross=cross->next;
			if(cross==found) {
				cross=cross->next;
			}
		}
	}
	else {
		assign=0;
		i=found->eco_count/6;
		while(assign<6) {
			if(cross->eid==-1) {
				cross=cross->next;
			}
			counter=0;
			while(counter<i) {
				change_pointers();
				counter++;
			}
			cross=cross->next;
			if(cross==found) {
				cross=cross->next;
			}
			assign++;	
		}
		cross=cross->prev;
		while(found->eco_count!=0) {
				tmp1=cross->employee_ecos;
                                cross->employee_ecos=found->employee_ecos;
                                tmp2=found->employee_ecos->next;
                                cross->employee_ecos->next=tmp1;
                                found->employee_ecos=tmp2;
				cross->eco_count=cross->eco_count+1;
                                found->eco_count=found->eco_count-1;
		}
	}	
	found->prev->next=found->next; //afairese ton employee apo tin lista
	found->next->prev=found->prev; 
	free(found);
	printf("\n\tEMPLOYEES:");
	cross=firstem;
	do {
               	printf("\n\t<employee%d: %d>:", (cross->eid),(cross->eco_count) );
               	eco=cross->employee_ecos;
               	for(i=0;i<cross->eco_count;i++) {
                       	printf(" <%d>", (eco->eco_id)); 
                       	eco=eco->next;
               	}
               	cross=cross->next;
        } while(cross->eid!=-1);   
	cross=cross->next;
	printf("\n\tDONE\n");
        return 1;
}

int assign_eco(int eid, int eco_id) {
	missing=1;
	firstem=cross;
	first=cross->eid;	
	do {
		if(cross->eid==eid) {
			missing=0;
			found=cross;
		}	
		cross=cross->next;
	} while(cross->eid!=first);

	if(missing==1) {
		printf("Did not found!\n");
		return -1;
	}
	else {
		found->eco_count=found->eco_count+1;
		assign_e=(struct employee_eco*)malloc(sizeof(struct employee_eco));
		if(assign_e==NULL) {
			printf("Cannot allocate memory\n");		
			return -1;
		}
		assign_e->eco_id=eco_id;
		if(found->eco_count==1) {
			assign_e->next=NULL;
			found->employee_ecos=assign_e;
		}
		else {
			assign_e->next=found->employee_ecos;
			found->employee_ecos=assign_e;
		}
	}
	printf("\tEMPLOYEES:");
	do {
		printf("\n\t<employee%d: %d>:", (cross->eid),(cross->eco_count) );
		eco=cross->employee_ecos;
		for(i=0;i<cross->eco_count;i++) {
			printf(" <%d>", (eco->eco_id)); //oxi hd!
			eco=eco->next;
		}
		cross=cross->next;
	} while(cross->eid!=-1);
	cross=cross->next;
	printf("\nDONE\n");
        return 1;
}

int hire(int eid) {
	new_empl=(struct employee*)malloc(sizeof(struct employee));
        if(new_empl==NULL) {
        	printf("Cannot allocate memory\n");
                return -1;
        }
        new_empl->eco_count=0;
        new_empl->eid=eid;
        new_empl->employee_ecos=NULL;
	if(last_insert==NULL) {
		new_empl->prev=employees_sentinel;
		new_empl->next=employees_sentinel;
		employees_sentinel->prev=new_empl;
		employees_sentinel->next=new_empl;
		last_insert=new_empl;
	}
	else {
		new_empl->prev=last_insert;
		new_empl->next=employees_sentinel;
		last_insert->next=new_empl;
		employees_sentinel->prev=new_empl;
		last_insert=last_insert->next;
	}
	printf("\tEMPLOYEES: ");
	cross=last_insert->next;
	cross=cross->next;
	while(cross!=employees_sentinel) {
		printf("<%d>, ", cross->eid);
		cross=cross->next;
	}
	cross=cross->next;
	printf("\nDONE\n");
        return 1;
}

int delete_old_visitors(int years_interval) {
	temp1=visitors_list;
	temp2=NULL;
		//tsekarw oti to 1o stoixeio de thelei diagrafi.an thelei thn kanw k tsekarw gia 2o ktl.mexri n vrw 1 p de thelei
		//meta vazw st 1o tmp2 kai sto 2o tmp1 kai vlepw an thelei t tmp1.
		//an to diagrapsw tsekarw to kainourio tmp1 ALLIWS an dn thelei diagrafi proxwraw to tmp1 k t tmp2 ola auta
		//mexri tempp1=frouros tote stop 
	while( (2016-temp1->last_visit) > years_interval ) {
		visitors_list=temp1->next;
		free(temp1);
		temp1=visitors_list;
	}
	wait=0;
	while(temp1!=sentinal) {
		if(wait==0) {
			temp2=temp1;
			temp1=temp1->next;
			if(temp1==sentinal) { break; }
		}
		if( (2016-temp1->last_visit) > years_interval ) {
			temp2->next=temp1->next;
			free(temp1);
			temp1=temp2->next;
			wait=1;
		}
		else { wait=0; }
	}
	printf("\tVisitors = ");
	temp1=visitors_list;
	while(temp1!=sentinal) {
		printf("<%d : %d : %d>, ", (temp1->vid), (temp1->visits), (temp1->last_visit));
		temp1=temp1->next;
	}
	printf("\nDONE\n");
        return 1;
}

int visit(int vid, int year) {
	missing=0;
	if(visitors_list==NULL) {
		ptr = (struct visitor*)malloc(sizeof(struct visitor));
		if(ptr==NULL) {
			printf("Cannot allocate memory\n");
			return -1;
		}
		visitors_list=ptr;
		ptr->vid=vid;
		ptr->last_visit=year;
		ptr->visits=1;
		ptr->next=sentinal;
	}
	else {
		temp1=visitors_list;
		while(temp1->vid!=vid) {
			if(temp1->next!=sentinal) {
				temp1=temp1->next;
			}
			else {
				missing=1;
				break;
			}
		}
		if(missing==0) {
			temp1->last_visit=year;
			temp1->visits=temp1->visits+1;
		}
		else {
			ptr = (struct visitor*)malloc(sizeof(struct visitor));
                	if(ptr==NULL) {
                        	printf("Cannot allocate memory\n");
                        	return -1;
                	}
			temp1->next=ptr;
			ptr->next=sentinal;
			ptr->vid=vid;
			ptr->last_visit=year;
			ptr->visits=1;
		}
	}
	temp1=visitors_list;
	printf("\tVisitors = ");
	while(temp1!=sentinal) {
		printf("<%d : %d : %d>, ", (temp1->vid), (temp1->visits), (temp1->last_visit));
		temp1=temp1->next;
	}
	printf("\nDONE\n");
	return 1;
}


int animal_lives_in_eco(int aid, int eco_id) {
        current = animals_list;
        prev = NULL;
        
	temp = (struct animal*) malloc(sizeof(struct animal));
	if(temp==NULL) {
		printf("Cannot allocate memory\n");
		return -1;
	}
        temp->aid = aid;
        temp->ecosystem_id = eco_id;
        temp->next = NULL;

	while(current!=NULL && current->aid < aid) {
                prev = current;
                current = current->next;
        }
        
	if(prev==NULL) {
		temp->next=current;
		animals_list=temp;
	}	
        else {
		prev->next = temp;
		temp->next = current;
	}

	printf("	Animals = ");
	temp=animals_list;
	while(temp!=NULL) {
		printf("<%d> ", temp->aid);
		temp = temp->next;
	}	
	printf("\nDONE \n");

        return 1;
}

int distribute_animals() {
	temp=animals_list; //me ton deikti temp tha diatrexw thn lista twn zwwn apo thn arxh mexri to telos		
	while(temp!=NULL) {
		animals_list = animals_list->next; //diagrafi tou animal ap thn lista zwwn
		curr = &ecosystems_array[temp->ecosystem_id];
		curr->animal_count = curr->animal_count + 1;

		if(curr->animal_count==1) { //ean einai to 1o zwaki tote vale ton deikti tou ecosystem 
			curr->animals = temp; //na deixnei stin arxi tis listas twn zwwn tou sugkekrimenou oik.
			temp->next = NULL;
			p[temp->ecosystem_id] = temp; /*vale ton deikti p antistoixei sto oik. na deixnei sto animal p molis eisigages*/			 }
		else {
			p[temp->ecosystem_id]->next = temp;
			temp->next = NULL;	
			p[temp->ecosystem_id] = p[temp->ecosystem_id]->next;
		}
		temp = animals_list;
	}
	printf("\tECOSYSTEMS: \n");
	i=0;
	while(i<50) {
		printf("\n\tecosystem%d:", i);
		curr=&ecosystems_array[i];
		temp=curr->animals;
		while(temp!=NULL) {
			printf(" <%d>", temp->aid);
			temp=temp->next;
		}
		i=i+1;
	}
	printf("\nDONE\n");	
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	initialize();

	/* Check command buff arguments */
	if ( argc != 2 ) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Read input file buff-by-buff and handle the events */
	while ( fgets(buff, BUFFER_SIZE, fin) ) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

		/* Comment */
		case '#':
			break;

		/* Animal lives in eco
		 * L <aid> <eco_id> */
		case 'L':
		{
			int aid;
			int eco_id;
			sscanf(buff, "%c %d %d", &event, &aid, &eco_id);
			DPRINT("%c %d %d\n", event, aid, eco_id);


			if ( animal_lives_in_eco(aid, eco_id) ) {
				DPRINT("L %d %d succeeded\n", aid, eco_id);
			} else {
				fprintf(stderr, "L %d %d failed\n", aid, eco_id);
			}

			break;
		}

		/* Distribute animals
		 * D */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( distribute_animals() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Visit
		 * V <vid> <year> */
		case 'V':
		{
			int vid;
			int year;
			sscanf(buff, "%c %d %d", &event, &vid, &year);
			DPRINT("%c %d %d\n", event, vid, year);

			if ( visit(vid, year) ) {
				DPRINT("%c %d %d succeeded\n", event, vid, year);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, vid, year);
			}

			break;
		}

		/* Delete old visitors
		 * O <years_interval> */
		case 'O':
		{
			int years_interval;
			sscanf(buff, "%c %d", &event, &years_interval);
			DPRINT("%c %d\n", event, years_interval);

			if ( delete_old_visitors(years_interval) ) {
				DPRINT("%c %d succeeded\n", event, years_interval);
			} else {
				fprintf(stderr, "%c %d failed\n", event, years_interval);
			}

			break;
		}

		/* Hire employee
		 * H <eid> */
		case 'H':
		{
			int eid;
			sscanf(buff, "%c %d", &event, &eid);
			DPRINT("%c %d\n", event, eid);

			if ( hire(eid) ) {
				DPRINT("%c %d succeeded\n", event, eid);
			} else {
				fprintf(stderr, "%c %d failed\n", event, eid);
			}

			break;
		}

		/* Assign eco to employee
		 * A <eid> <eco_id> */
		case 'A':
		{
			int eid;
			int eco_id;
			sscanf(buff, "%c %d %d", &event, &eid, &eco_id);
			DPRINT("%c %d %d\n", event, eid, eco_id);

			if ( assign_eco(eid, eco_id) ) {
				DPRINT("%c %d %d succeeded\n", event, eid, eco_id);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, eid, eco_id);
			}

			break;
		}

		/* Retire employee
		 * R <eid> */
		case 'R':
		{
			int eid;
			sscanf(buff, "%c %d\n", &event, &eid);
			DPRINT("%c %d\n", event, eid);

			if ( retire(eid) ) {
				DPRINT("%c %d succeeded\n", event, eid);
			} else {
				fprintf(stderr, "%c %d failed\n", event, eid);
			}

			break;
		}

		/* Gold (most frequent) visitors
		 * G */
		case 'G':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( gold_visitors() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print ecosystems
         * X */
		case 'X':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_ecos() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print visitors
		 * Y */
		case 'Y':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_visitors() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print employees
		 * Z */
		case 'Z':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_employees() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}


		/* Empty line */
		case '\n':
			break;

		/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);

			break;
		}
	}
	
	free_world();
	return (EXIT_SUCCESS);
}
