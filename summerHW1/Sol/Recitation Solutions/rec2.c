#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct node {

	int val;
	struct node* left;
	struct node* right;
};

struct btree {

	struct node* root;
};

void insert(struct btree* tree, int val) {
	
	// Find the correct location.
	struct node** n = &(tree->root);
	while (*n != NULL) n = (*n)->val > val ? &((*n)->left) : &((*n)->right);

	// We have found the location where we need to add the new node.
	*n = (struct node*) malloc(sizeof(struct node*));
	(*n)->val = val;
}

struct btree* populate(int size) {

	// DO NOT CHANGE OR REUSE THIS LINE!
	srand(time(NULL));

	// Initialize binary tree.
	struct btree* tree = (struct btree*) malloc(sizeof(struct btree));

	// Add each element to the list.
	for (int i=0; i < size; ++i) insert(tree, rand());

	return tree;
}

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Invalid number of args");
		exit(1);
	}

	int size = atoi(argv[1]);

	if (size < 1) {

		printf("Size must be > 0");
		exit(1);
	}

	// Randomly populate a linked list.
	struct btree* tree = populate(size);

	return 0;
}
