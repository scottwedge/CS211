#include<stdio.h>
#include<stdlib.h>

typedef struct BSTNode{
	int key;
	struct BSTNode* left;
	struct BSTNode* right;

}BSTNode;

BSTNode* root = NULL;

void insert(int);
BSTNode* delete(int, BSTNode*);
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

BSTNode* delete(int n, BSTNode* current){
	if(current == NULL){ //root is null
		printf("fail\n");
		return NULL;
	}
	if(n > current->key){
		current->right = delete(n, current->right);
		return current;
	}else if(n < current->key){
		current->left = delete(n, current->left);
		return current;
	}else{ //current->key == n
		if(current->left == NULL && current->right == NULL){ //NO CHILDREN -> LEAF NODE
			free(current);
			printf("success\n");
			return NULL;
			
		}else if(current->left != NULL && current->right == NULL){ //LEFT CHILD NODE
			BSTNode* temp = current->left;
						
			free(current);
			printf("success\n");
			return temp;

		}else if(current->left == NULL && current->right != NULL){ //RIGHT CHILD NODE
			BSTNode* temp = current->right;	
			free(current);
			printf("success\n");
			return temp;

		}else{	//TWO CHILDREN
			BSTNode* temp = current->right;
			while(temp->left != NULL){
				temp = temp->left;
			}
			current->key = temp->key;
			current->right = delete(temp->key, current->right);	
	
			return current;
		}	
	}

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
	}else{
		printf("fail\n");
	}

	while(fscanf(fp, "%c\t%d\n", &c, &n) != EOF){
		if(c == 'i'){
			insert(n);
		}else if(c == 's'){
			search(n);
		}else{
			root = delete(n, root);
		}
	}
	
	free_nodes(root);
	
	fclose(fp);
	return 0;
}
