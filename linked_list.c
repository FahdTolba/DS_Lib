#include"linked_list.h"

void
ls_init(linked_list_t *ls) {

	CS_START(&sig); //might be useful for using in an OS Kernel
        ls->head = NULL;
        ls->tail = NULL;
	ls->curr = NULL;
	ls->count = 0;
	CS_END(&sig);
}

int
ls_empty(linked_list_t *ls) {
	if(!ls //list uninitialized
	|| ls->head == NULL)
		return 1;
	else
		return 0;
}

void
ls_add(linked_list_t *ls, struct node *curr_node) {

//the head stays the same, and the tail points to the last added node
//this holds for both following cases
//the next pointer points the node added after the current node

	CS_START(&sig);

	if(ls_empty(ls)) {
		ls->head = curr_node;
		ls->tail = curr_node;
		ls->curr = curr_node;
		curr_node->next = NULL;
		curr_node->prev = NULL;
	}else {
		curr_node->prev = ls->tail;
		curr_node->next = NULL;
		ls->tail->next = curr_node;
		ls->tail = curr_node;
	}

	ls->count++;

	CS_END(&sig);
}

//used to avoid infinite loops
//when traversing a list using ls_fetch(),
//and using ls_del() then adding the same node
//back into the list, if ls_add() is used,
//then would be added from list's tail which
//causes the infinite loop
void
ls_insert(linked_list_t *ls, struct node *nd) {

	CS_START(&sig);
	if(ls_empty(ls)) {
		ls_add(ls, nd);
	}else {
		ls->head->prev = nd;
		nd->next = ls->head;
		nd->prev = NULL;
		ls->head = nd;
		ls->count++;
	}
	CS_START(&sig);
}


void
ls_del(linked_list_t *ls, struct node *curr_node) {

	CS_START(&sig);

	if(ls_empty(ls)) return;

	if(curr_node == ls->head) {
		if(curr_node == ls->tail) {
			ls->curr = ls->tail = ls->head = NULL;
		}else {
			ls->head = ls->head->next;
			ls->head->prev = NULL;
		}
	}else
	if(curr_node == ls->tail) {
		ls->tail = ls->tail->prev;
		ls->tail->next = NULL;
	}else {
		curr_node->next->prev = curr_node->prev;
		curr_node->prev->next = curr_node->next;
	}

	if(ls->curr == curr_node)
		ls->curr = ls->curr->next;

	curr_node->prev = curr_node->next = NULL; //might not always be necessary
							//but it's better to avoid
							//another code from using a node
							//to corrupt a list it no longer belongs to
	ls->count--;

	CS_END(&sig);
}

struct node *
ls_fetch(linked_list_t *ls){

struct node *temp;

	CS_START(&sig);
	temp = ls->curr;

	if(ls->curr == NULL)
		ls->curr = ls->head;
	else
		ls->curr = ls->curr->next;

	CS_END(&sig);
	return temp;
}

void //functionality included in ls_fetch()
ls_reset(linked_list_t *ls) {
	
	CS_START(&sig);
	ls->curr = ls->head;
	CS_END(&sig);
}


struct node *
ls_search(linked_list_t *ls, void *key, int keysize) {

struct node *curr_nd;

	CS_START(&sig);

	if(ls_empty(ls)) {
		CS_END(&sig);
		return NULL;
	}else {
		curr_nd = ls->head;
		while(curr_nd) {
			if( memcmp(NDKEY_OFFSET(curr_nd), key, keysize) == 0 ) {
				CS_END(&sig);
				return curr_nd;
			}
			curr_nd = curr_nd->next;
		}
		CS_END(&sig);
		return NULL;
	}
}

void ls_mv_all(linked_list_t *dst, linked_list_t *src){

	CS_START(&sig);
	dst->head = src->head;
	dst->tail = src->tail;
	dst->curr = src->curr;
	dst->count = src->count;
	ls_init(src);
	CS_END(&sig);
}

//merge both ls1 and ls2 into ls1
void 
ls_merge(linked_list_t *ls1, linked_list_t *ls2) {//printf("ls_merge()\n");

	CS_START(&sig);
	if(ls_empty(ls2))
		goto out;

	if(ls_empty(ls1))
		ls_mv_all(ls1, ls2);
	else{
		ls1->tail->next = ls2->head;
		ls2->head->prev = ls1->tail;
		ls1->tail = ls2->tail;
		ls1->count += ls2->count;
	}

out:
	ls_init(ls2);
	CS_END(&sig);
}

//split a list of length size out of "ls" and place it in "seg"
void
ls_split(linked_list_t *ls, linked_list_t *seg, int size){

struct node *nd,*tmp;

	if(size < 1) return;

	if(size >= ls->count || ls->count == 1){
		ls_merge(seg, ls);
		return;
	}

	ls_reset(ls);

	for(int i = 0; i < size; i++) 
		nd = ls_fetch(ls);

	tmp = nd->next; //save for swapping

	ls_init(seg);
	seg->head = seg->curr = ls->head;
	seg->tail = nd;
	seg->tail->next = NULL;
	seg->count = size;

	ls->count -= size;
	if(ls->count){
		ls->head = ls->curr = tmp;
		ls->head->prev = NULL;
	}
}


//remove all nodes from a list
void
ls_del_all(linked_list_t *ls) {

struct node *nd;

	CS_START(&sig);
        if(ls_empty(ls))
		goto out;

	ls_reset(ls);
	while(nd = ls_fetch(ls)){
        	ls_del(ls,nd);
	}

out:
	CS_START(&arg);
}
