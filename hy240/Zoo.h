struct animal {
	int aid;
	int ecosystem_id;
	struct animal *next;
};

struct ecosystem {
	int eco_id;
	int animal_count;
	struct animal *animals;
};

struct visitor {
	int vid;
	int last_visit;
	int visits;
	struct visitor *next;
};

struct employee_eco {
	int eco_id;
	struct employee_eco *next;
};

struct employee {
	int eid;
	int eco_count;
	struct employee_eco *employee_ecos;
	struct employee *prev;
	struct employee *next;
};

int i, x, missing, wait, first, counter, assign, min, thesi; 

/*The array of the parks ecosystems */
struct ecosystem ecosystems_array [50], *curr;
 
/* global variable, pointer to the beginning of the animals list*/
struct animal *animals_list, *prev, *current, *temp, *p[50]; 	 

/* global variable, pointer to the beginning of the visitors list*/
struct visitor *visitors_list, senti, *sentinal, *ptr, *temp1, *temp2, *gold[5]; 

/* global variable, pointer to the sentinel node of the users list */
struct employee *employees_sentinel, em_senti, *last_insert, *new_empl, *cross, *found, *firstem; 

struct employee_eco *assign_e, *eco, *tmp1, *tmp2;
