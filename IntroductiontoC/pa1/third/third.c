#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
	int data;
	struct Node* next;
}Node;

Node** insert(Node**, int);
void search(Node**, int);
void free_nodes(Node*);

Node** insert(Node** hash_table, int n){	
	int isNeg = 0;	
	int bucket = 0;

	if(n < 0){
	 isNeg = 1;	
	}
	Node* temp = malloc(sizeof(Node));
	temp->data = n;
	temp->next = NULL;

	if(isNeg == 0){	
		bucket = n % 1000;
	}else{
		int n1000 = n;
		while(n1000 < 0){
			 n1000 = n1000 + 1000;
		}
		bucket = n1000 % 1000;
	}
	Node* ptr;
	for(ptr = hash_table[bucket]; ptr != NULL; ptr = ptr->next){
		if(ptr->data == n){
			printf("duplicate\n");
			return hash_table;
		}
	}
	
	temp->next = hash_table[bucket]; //add node to front of bucket LL
	hash_table[bucket] = temp;
	printf("inserted\n");

	return hash_table;

}

void search(Node** hash_table, int n){
	int isNeg = 0;
	int bucket = 0;
	if(n < 0){
		isNeg = 1;
	}
	if(isNeg == 0){
		bucket = n % 1000;
	}else{
		int n1000 = n;
		while(n1000 < 0){
			n1000 = n1000 + 1000;
		}
		bucket = n1000 % 1000;
	}
	
	Node* ptr;
	for(ptr = hash_table[bucket]; ptr != NULL; ptr = ptr->next){
		if(ptr->data == n){
			printf("present\n");
			return;
		}
	}

	printf("absent\n");
}


void free_nodes(Node* front){
	Node* temp = front;
	while(temp != NULL){
		Node* temp2 = temp;
		temp = temp->next;
		free(temp2);
	}
}

int main(int argc, char** argv){
	
	if(argc != 2){
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		exit(0);	
	}
	char c;
	int n = 0;
	if(fscanf(fp, "%c\t%d\n", &c, &n) == EOF){
		exit(0);
	}

	Node** hash_table = malloc(1000*sizeof(Node*)); //makes arr of linked lists
	
	//first element
	if(c == 'i'){
		hash_table = insert(hash_table, n);
	}else{
		printf("absent\n");
	}

	while(fscanf(fp, "%c\t%d\n", &c, &n) != EOF){
		if(c == 'i'){
			hash_table = insert(hash_table, n);
		}else{
			search(hash_table, n);
		}
	}
	
	free(hash_table);

	return 0;
}
