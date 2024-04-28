#include"bs_tree.h"

tree_init(struct tree_nd *tree){
	tree = NULL;
}

int
tree_inorder_walkin(struct tree_nd *tree){

int count = 0;

	if(!tree) return 0;

	count += tree_inorder_walkin(tree->l);
	count++; //root node
	count += tree_inorder_walkin(tree->r);

	return count;
}

tree_insert(struct tree_nd *tree, struct tree_nd *nd){

struct tree_nd *subtree;

	nd->p = nd->l = nd->r = NULL;
	subtree = tree;

	while(subtree){
		if(memcmp(&nd->key, &subtree->key, key_size) <= 0){
			if(subtree->l)
				subtree = subtree->l;
			else{
				subtree->l = nd;
				nd->p = subtree;
				return;
			}
		}else{
			if(subtree->r)
				subtree = subtree->r;
			else{
				subtree->r = nd;
				nd->p = subtree;
				return;
			}
		}
	}
	
	tree = nd; //empty tree, insert nd as root
}

struct tree_nd *
tree_search(struct tree_nd *tree, struct tree_nd *nd, int key_size){ 

struct tree_nd *subtree;

	subtree = tree;

	while(subtree){
		if(memcmp(&nd->key, &subtree->key, key_size) == 0)
			return subtree;
		else
		if(memcmp(&nd->key, &subtree->key, key_size) < 0)
			subtree = subtree->l;
		else
			subtree = subtree->r;
	}

	return NULL;
}

/*
struct tree_nd *
tree_predecessor(struct tree_nd *tree, struct tree_nd *nd){
	the maximum of the left subtree
	or
	the nearest anscestor whose right child is also
	an anscestor of nd (or is nd itself)
}*/

struct tree_nd *
tree_successor(struct tree_nd *tree, struct tree_nd *nd){

tree_nd *p = NULL;
tree_nd *x = nd;

	//either the minimum of the right subtree
	x = tree_min(nd->r))

	if(x)
		return x;
	else{
		//the nearest anscestor whose left child is
		//an anscestor of nd (or nd itself)

		while(p = nd->p){
			if(nd == nd->p->l){
				break; //or return p;
			else
				nd = nd->p;
		}

		return p;
	}
}

/*
 if the caller wants to use the deleted nd
 for another purpose, it should copy it to another
 memory location first, since key and data might
 referenced by argument pointer might have been
 overwritten if the successor was spliced out
*/
struct tree_nd *
tree_delete(struct tree_nd *tree, struct tree_nd *nd, int key_size){

struct tree_nd *y; //actual memory location removed (spliced out)
struct tree_nd *x; //pointer to child node that 
			//should have its pointers updated

	//determine which node will be removed (argument, or successor?)
	if(nd->l == NULL || nd->r == NULL)
		y = nd;
	else
		y = tree_successor(tree, nd); //will always return tree_min(nd->r); here

	//x set to y's child (if any)
	if(y->l){
		x = y->l;
	}else{
		x = y->r;
	}

	if(x)
		x->p = y->p;

	//update parent's and child's pointers of spliced out node 
	if(!y->p)
		tree = x; //if y has no parent, x is the new root
	else{
		if(y == y->p->l){
			y->p->l = x;
		}else{
			y->p->r = x;
		}
	}
 
	if(y != nd){ //successor was removed
		memcpy(&nd->key, &y->key, key_size); //copy successor->key to nd->key
							//(everything else in nd remains unchanged)
	}

	return y;
}


struct tree_nd *
tree_min(struct tree_nd *tree){

struct tree_nd *min;

	min = tree;

	while(min->l)
		min = min->l;

	return min;
}

struct tree_nd *
tree_max(struct tree_nd *tree){

struct tree_nd *max;

	max = tree;

	while(max->r)
		max = max->r;

	return max;	
}

//returns node that contains same
//data as "nd", either the same memory location
//or a different one allocated, for further use
//of data by the caller after deletion from tree
void *
tree_del_copy(struct tree_nd *tree , tree_nd *nd, int size){

	tmp = nd;
	tmp2 = malloc(size);

	memcpy(tmp2, nd, size);

	tmp = tree_delete(tree, nd);

	if(tmp == nd){
		free(tmp2);
		return nd;
	}else{
		free(nd);
		return tmp2;
	}
}

