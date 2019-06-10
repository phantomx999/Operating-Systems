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
	
	// TODO: Implement insertion.
	
	struct node* newN = malloc(sizeof(struct node*));
	newN->val = val;
	newN->left = NULL;
	newN ->right = NULL;
	
	if(tree->root == NULL){
		tree->root = newN;
	}
	else if(val < tree->root->val){
		tree->root = tree->root->left;
		insert(tree, val);
	}
	else if(val >= tree->root->val){
		tree->root = tree->root->right;
		insert(tree, val);
	}	
}

struct btree* populate(int size) {

	// DO NOT CHANGE OR REUSE THIS LINE!
	srand(time(NULL));

	// TODO: Insert 'size' number of random nodes.
	// Obtain random number using rand()
	int count = 0;	
	struct btree* tree = malloc(sizeof(struct btree));
	while(count != size){	
		count++;
		int num = rand();
		insert(tree, num);
	
	}
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

	printf("made it here");

	return 0;
}
