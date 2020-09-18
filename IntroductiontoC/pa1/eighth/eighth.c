#include<stdio.h>
#include<stdlib.h>

typedef struct BSTNode{
	int key;
	struct BSTNode* left;
	struct BSTNode* right;

}BSTNode;

BSTNode* root = NULL;

void insert(int);
void search(int);
void free_nodes();

void insert(int n){
	int height = 1;
	BSTNode* ptr = root;
	BSTNode* prev = NULL;	
	while(ptr != NULL){
		if(ptr->key == n){
			printf("duplicate\n");
			return;
		}
		prev = ptr;		
		if(ptr->key > n){
			ptr = ptr->left;
			height++;
		}else{
			ptr = ptr->right;
			height++;
		}
	}
	
	BSTNode* temp = malloc(sizeof(BSTNode));
	temp->key = n;
	temp->left = NULL;
	temp->right = NULL;

	if(prev == NULL){
		root = temp;
	}else if(prev->key > temp->key){
		prev->left = temp;
	}else{
		prev->right = temp;
	}
	printf("inserted %d\n", height);
}

void search(int n){
	BSTNode* ptr = root;
	int height = 1;	
	while(ptr != NULL){
		if(ptr->key == n){
			printf("present %d\n", height);
			return;
		}else if(n < ptr->key){
			ptr = ptr->left;
			height++;
		}else{
			ptr = ptr->right;
			height++;
		}
	}
	if(ptr == NULL){
		printf("absent\n");
		return;
	}
}

void free_nodes(BSTNode* node){
	if(node == NULL){
		return;
	}
	free_nodes(node->left);
	free_nodes(node->right);
	
	free(node);
}

int main(int argc, char** argv){
	
	if(argc != 2){
		exit(0);
	}
	
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("error\n");
		exit(0);
	}
	
	char c;
	int n = 0;
	if(fscanf(fp, "%c\t%d\n", &c, &n) == EOF){
		exit(0);
	}
	
	//FOR FIRST ELEMENT ONLY
	if(c == 'i'){
		insert(n);
	}else if(c == 's'){
		printf("absent\n");
	}

	while(fscanf(fp, "%c\t%d\n", &c, &n) != EOF){
		if(c == 'i'){
			insert(n);
		}else if(c == 's'){
			search(n);
		}
	}
	
	free_nodes(root);
	
	fclose(fp);
	return 0;
}
