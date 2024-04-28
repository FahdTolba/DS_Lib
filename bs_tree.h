#ifndef __BS_TREE_H
#define __BS_TREE_H

#define KEY_SIZE	512

struct tree_nd{
	struct tree_nd *p;
	struct tree_nd *l;
	struct tree_nd *r;
	//unsigned int count;
	char key_data[KEY_SIZE];
};

#endif //ndef __BS_TREE_H
