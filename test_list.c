/*
 * Name:Marcella Petrucci
 * Email: petrucma@oregonstate.edu
 * 
 * This has been adapted from code used and written in CS 261 with Rob Hess. The structs are his, the implementation is mine
 */

#include <stdlib.h>

 /*
  * This structure is used to represent a single link in a singly-linked list.
  * It is not defined in ll.h, so it is not visible to the user.  You should not
  * modify this structure.
  */
struct link {
	void* val;
	struct link* next;
};

/*
 * This structure is used to represent an entire singly-linked list.  Note that
 * we're keeping track of just the head of the list here, for simplicity.
 */
struct list {
	struct link* head;
};

/*
 * This function should allocate and initialize a new, empty linked list and
 * return a pointer to it.
 */
struct list* list_create() {
	struct list* new_list;
	new_list = malloc(sizeof(struct list));/*allocate memory size of struct type list, returns void pointer*/
	new_list->head = NULL;/* empty list, just head and tail */
	return new_list;
}

void list_insert(struct list* list, void* val) {

	struct link* new_movie;/*create pointer to new link*/
	new_movie = malloc(sizeof(struct link));/*allocate memory for new link---returns pointer*/

  /*concept: head points to new node, new next node points to where head's pointer previously pointed*/
	if (new_movie != NULL) {

		new_movie->next = list->head;/*new link becomes head*/
		list->head = new_movie; /*head now points to new lin--new link is already a pointer to a link*/

		new_movie->val = val;//can use as void pointer or just assign parsed info here.
	}

	return;
}