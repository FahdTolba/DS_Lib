#ifndef	__LIST_H
#define __LIST_H

#include<string.h>

typedef struct linked_list_s{
        struct node *head;
        struct node *tail;
        struct node *curr; //current node to fetch
        unsigned int count;
        unsigned char lock;
}linked_list_t;

//Node types
#define NDTYPE_GENERIC  0

#define node_hdr		\
	struct node *prev;	\
	struct node *next;	\
	unsigned short type	\
	

#define ND_HDR_SZ sizeof(struct node_hdr)


struct node{
        /*struct node *prev;
        struct node *next;
        unsigned short nd_type;*/
	node_hdr;
        char key_data[512];
};


#define NDKEY_OFFSET(a) &a->key_data

//currently critical sections not defined (should be in a separate header)
#define CS_START(A)	
#define CS_END(A)	

void ls_init(linked_list_t *);

int ls_empty(linked_list_t *);

void ls_add(linked_list_t *, struct node *);

void ls_del(linked_list_t *, struct node *);

struct node *ls_fetch(linked_list_t *);

void ls_reset(linked_list_t *);

struct node *ls_search(linked_list_t *, void *, int);

void ls_free(linked_list_t *, struct node *);

#endif // __LIST_H
