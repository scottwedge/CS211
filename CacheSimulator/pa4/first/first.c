#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct Block{
	size_t tag;
	struct Block* next;
}Block;

int power_2(char*);
void error();
int search(Block**, size_t, int, int, int); 
Block** insert(Block**, size_t, int, int, int);
Block** update(Block**, size_t, int, int, int);
void print(int, int, int, int, int);
void free_blocks(Block**, int);

int power_2(char* str){
	int num = atoi(str);
	if(num && (num & (num-1)) != 0){
		error();
	}
	return num;
}

void error(){
	printf("error\n");
	exit(0);
}

size_t get_tag(size_t address, int S, int A, int B){
	int offset_bits = log(B) / log(2);
	size_t index_bits = log(S) / log(2);
	size_t tag = (address >> (offset_bits + index_bits));	//(address >> offset_bits) % (0x1 << index_bits);
	return tag;
}

size_t get_index(size_t address, int S, int A, int B){
	int offset_bits = log(B) / log(2);
	size_t index = (address >> (offset_bits)) % S;
	return index;
}

int search(Block** cache, size_t address, int S, int A, int B){
	int hit = 0; // 0 = miss, 1 = hit
	size_t tag = get_tag(address, S, A, B);
	size_t index = get_index(address, S, A, B);
	for(Block* ptr = cache[index]; ptr != NULL; ptr = ptr->next){
		if(ptr->tag == tag){
			hit = 1;
		}
	}
	return hit;
}

Block** insert(Block** cache, size_t address, int S, int A, int B){
	size_t tag = get_tag(address, S, A, B);
	size_t index = get_index(address, S, A, B);
	Block* temp = malloc(sizeof(Block));
	temp->tag = tag;
	temp->next = NULL;
	if(cache[index] == NULL){
		cache[index] = temp;
		return cache;
	}
	int num_elements = 1;
	Block* ptr;
	for(ptr = cache[index]; ptr->next != NULL; ptr = ptr->next){
		num_elements++;
	}
	if(num_elements < A){
		ptr->next = temp;
	}else{
		ptr->next = temp;
		cache[index] = cache[index]->next;
	}
	return cache;
}

Block** update(Block** cache, size_t address, int S, int A, int B){
	size_t tag = get_tag(address, S, A, B);
	size_t index = get_index(address, S, A, B);
	
	Block* curr = cache[index];
	Block* prev;
	
	if(cache[index]->tag == tag){ //delete if it is the first
		cache[index] = cache[index]->next;
	}else{
		while(curr->tag != tag){	//deletes if block isnt first
			prev = curr;
			curr = curr->next;
		}
		prev->next = curr->next;
	}
	Block* temp = malloc(sizeof(Block));
	temp->tag = tag;
	temp->next = NULL;
	if(cache[index] == NULL){
		cache[index] = temp;
		return cache;
	}
	Block* ptr = cache[index];
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
	return cache;
}

void print(int n, int reads, int writes, int hits, int misses){
	if(n == 0){
		printf("no-prefetch\n");
	}else{
		printf("with-prefetch\n");
	}
	printf("Memory reads: %d\n", reads);
	printf("Memory writes: %d\n", writes);
	printf("Cache hits: %d\n", hits);
	printf("Cache misses: %d\n", misses);
}

void free_blocks(Block** cache, int S){
	for(int i = 0; i < S; i++){
		Block* temp = cache[i];
		while(temp != NULL){
			Block* temp2 = temp;
			temp = temp->next;
			free(temp2);
		}
	}	
}

int main(int argc, char** argv){	
	if(argc != 7){	//check if valid num of parameters
		error();
	}
	int C = power_2(argv[1]);	//input cache size
	int B = power_2(argv[2]);	//input block size
	
	int cache_policy = 0;	//input cache policy --> fifo = 1, lru = 0	
	if(strcmp(argv[3], "fifo") == 0){ 
		cache_policy = 1;
	}else if(strcmp(argv[3], "lru") == 0){
		cache_policy = 0;
	}else{
		error();
	}
	int A = 0;	//input associativity --> direct = 1, fullassoc = C/B, assoc = n
	int S = 0;	//calculate set number
	if(strcmp(argv[4], "direct") == 0){
		A = 1;
		S = (C / B);
	}else if(strcmp(argv[4], "assoc") == 0){
		A = (C / B);
		S = 1;
	}else{
		char compare[6] = "assoc:";
		for(int i = 0; i < 6; i++){
			if(compare[i] != argv[4][i]){
				error();
			}
		}
		char assoc_n[strlen(argv[4])-6];
		int j = 0;
		for(int i = 6; i < strlen(argv[4]); i++){
			assoc_n[j] = argv[4][i];
			j++;
		}
			A = power_2(assoc_n);		
			S = (C / (A * B));
	}
	int prefetch_size = 0;	//input prefetch data
	if(atoi(argv[5]) >= 0){
		prefetch_size = atoi(argv[5]);
	}else{
		error();
	}

	FILE* fp = fopen(argv[6], "r");
	if(fp == NULL){
		error();	
	}
	
	int reads = 0;
	int writes = 0;
	int hits = 0;
	int misses = 0;
	int reads2 = 0;
	int hits2 = 0;
	int misses2 = 0;
	
	Block** cache = malloc(S*sizeof(Block*));
	Block** cache2 = malloc(S*sizeof(Block*));
	char command;
	size_t address;
	if(cache_policy == 1){
	while(fscanf(fp, "\n%c %zx", &command, &address) > 0){
		if(command == '#'){
			break;
		}
		if(command == 'W'){
			writes++;
		}
		if(search(cache, address, S, A, B) == 1){
			hits++;
		}else{
			misses++;
			reads++;
			cache = insert(cache, address, S, A, B);
		}

		if(search(cache2, address, S, A, B) == 1){
			hits2++;
		}else{
			misses2++;
			reads2++;
			cache2 = insert(cache2, address, S, A, B);
			size_t prefetch_address = address+B;
			for(int i = 0; i < prefetch_size; i++){
				if(search(cache2, prefetch_address, S, A, B) == 0){
					reads2++;
					cache2 = insert(cache2, prefetch_address, S, A, B);
				}
				prefetch_address += B;
			}
		}
	}
	}else if(cache_policy == 0){
		while(fscanf(fp, "\n%c %zx", &command, &address) > 0){
		if(command == '#'){
			break;
		}
		if(command == 'W'){
			writes++;
		}
		if(search(cache, address, S, A, B) == 1){
			hits++;
			cache = update(cache, address, S, A, B);
		}else{
			misses++;
			reads++;
			cache = insert(cache, address, S, A, B);
		}

		if(search(cache2, address, S, A, B) == 1){
			hits2++;
			cache2 = update(cache2, address, S, A, B);
		}else{
			misses2++;
			reads2++;
			cache2 = insert(cache2, address, S, A, B);
			size_t prefetch_address = address+B;
			for(int i = 0; i < prefetch_size; i++){
				if(search(cache2, prefetch_address, S, A, B) == 0){
					reads2++;
					cache2 = insert(cache2, prefetch_address, S, A, B);
				}
				prefetch_address += B;
			}
		}
	}
	}
	print(0, reads, writes, hits, misses);
	print(1, reads2, writes, hits2, misses2);
	free_blocks(cache, S);
	free_blocks(cache2, S);
	free(cache);
	free(cache2);
	fclose(fp);
	return 0;
}
