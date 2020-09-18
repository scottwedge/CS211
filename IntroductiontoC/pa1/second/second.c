#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	int data;
	struct node* next;
}node;

int numNodes = 0;
node* head = NULL;
void insert(int);
void delete(int);
void print_list();
void free_list();

//INSERTS NODE INTO LL
void insert(int n){
	node* temp = malloc(sizeof(node));
	temp->data = n;
	if(head == NULL){	//if LL is empty
		temp->next = head;
		head = temp;	
		numNodes++;
		return;
	}else if(head->data > n){ //if number is the least
		temp->next = head;
		head = temp;
		numNodes++;
		return;
	}else{
		for(node* ptr = head; ptr != NULL; ptr = ptr->next){ //check for duplicates
			if(n == ptr->data){
				return;	//if duplicate found, do nothing
			}
		}
		// if node is larger than head and isn't duplicate
		node* prev = NULL;
		node* curr = head;
		while(curr != NULL && curr->data < n){
			prev = curr;
			curr = curr->next;
		}
		prev->next = temp;
		temp->next = curr;
		numNodes++;
	}
}


//DELETES NODE IF PRESENT
void delete(int n){
	if(head == NULL){
		return;
	}
	node* curr = head;
	node* prev;
	if(head->data == n){ //delete head
		head = curr->next;
		free(curr);
		numNodes--;
		return;
	}
	while(curr != NULL && curr->data != n){
		prev = curr;
		curr = curr->next;
	}
	if(curr == NULL){
		return;
	}	
	prev->next = curr->next;
	numNodes--;
	free(curr);
}

//PRINTS OUT THE LL
void print_list(){
	printf("%d\n", numNodes);
	node* temp = head;
	while(temp != NULL){
		printf("%d", temp->data);
		if(temp->next != NULL){
			printf("\t");
		}else{
			printf("\n");
		}
		temp = temp->next;
	}
}

void free_list(){
	node* temp = head;
	while(temp != NULL){
		node* temp2 = temp;
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
		printf("error\n");
		exit(0);
	}
	
	char c;
	int n = 0;
	if(fscanf(fp, "%c\t%d\n", &c, &n) == EOF){
		printf("0\n");
		exit(0);
	}
	if(c == 'i'){ //ONLY FOR FIRST ELEMENT
		insert(n);
	}

	while(fscanf(fp, "%c\t%d\n", &c, &n) != EOF){
		if(c == 'i'){
			insert(n);
		}else if(c == 'd'){
			delete(n);
		}
	}

	print_list();
	free_list();



	fclose(fp);
	return 0;
}
