#include<stdio.h>
#include<stdlib.h>

char** allocate_matrix();
int get_index(char);
int* solve_columns_rows(char**, int*, int, int); 
int* solve_subgrids(char**, int*, int, int);
char** guess(char**, int*, int, int);
void free_everything(char**, int*);

char** allocate_matrix(){
	char** ret = malloc(16 * sizeof(char*));
	for(int i = 0; i < 16; i++){
		ret[i] = malloc(16 * sizeof(char));
	}
	return ret;
}

int get_index(char c){
	if(c == '-'){
		return -1;	
	}else if(c == 'A'){
		return 10;
	}else if(c == 'B'){
		return 11;
	}else if(c == 'C'){
		return 12;
	}else if(c == 'D'){
		return 13;
	}else if(c == 'E'){
		return 14;
	}else if(c == 'F'){
		return 15;
	}else if(c == '0'){
		return 0;
	}else if(c == '1'){
		return 1;
	}else if(c == '2'){
		return 2;
	}else if(c == '3'){
		return 3;
	}else if(c == '4'){
		return 4;
	}else if(c == '5'){
		return 5;
	}else if(c == '6'){
		return 6;
	}else if(c == '7'){
		return 7;
	}else if(c == '8'){
		return 8;
	}else if(c == '9'){
		return 9;
	}
	return 0;
}

int* solve_columns_rows(char** matrix, int* arr, int r, int c){
	for(int i = 0; i < 16; i++){
		int index = get_index(matrix[r][i]);
		if(index != -1){
			arr[index] = 1;
		}
		index = get_index(matrix[i][c]);
		if(index != -1){
			arr[index] = 1;
		}
	}
	return arr;
}

int* solve_subgrids(char** matrix, int* arr, int r, int c){
	int x = (r/4) * 4;
	int y = (c/4) * 4;
	
	for(int i = x; i < (x+4); i++){
		for(int j = y; j < (y+4); j++){
			int index = get_index(matrix[i][j]);
			if(index != -1){
				arr[index] = 1;
			}
		}
	}
	return arr;
}

char** guess(char** matrix, int* arr, int r, int c){
	int index = 0;
	int next_r = -1;
	int next_c = -1;
	while(arr[index] == 1 && index < 16){	
		index++;
	}
	if(index == 16){ //no solutions left
		printf("no-solution\n");
		exit(0);
	}
	matrix[r][c] = arr[index];
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			if(matrix[i][j] == '-'){
				next_r = i;
				next_c = j;
				j = c;
				i = r;
			}
		}
	}
	if(next_r != -1 && next_c != -1){ //there's another unknown
		arr = solve_columns_rows(matrix, arr, next_r, next_c);
		arr = solve_subgrids(matrix, arr, next_r, next_c);
		
		int count = 0;
		int index_solution = -1;
		for(int k = 0; k < 16; k++){			
			if(arr[k] == 0){ //solution(s) found
				index_solution = k;
				count++;
			}
		}
		if(count >= 1){	//possible solutions available			
			if(count == 1){ // ONLY 1 SOL'N - 100% certain						
				if(index_solution == 10){
					matrix[i][j] = 'A';
				}else if(index_solution == 11){
					matrix[i][j] = 'B';
				}else if(index_solution == 12){
					matrix[i][j] = 'C';
				}else if(index_solution == 13){
					matrix[i][j] = 'D';
				}else if(index_solution == 14){
					matrix[i][j] = 'E';
				}else if(index_solution == 15){
					matrix[i][j] = 'F';
				}else if(index_solution < 10){
					char temp = index_solution + '0';
					matrix[i][j] = temp;
				}	
			}else{ //multiple solutions --> guess!
				matrix = guess(matrix, arr); 
			}
	}else{
		
	}
	return matrix;
}

void free_everything(char** matrix, int* arr){
	for(int i = 0; i < 16; i++){
		free(matrix[i]);
	}
	free(arr);

	free(matrix);
}

int main(int argc, char** argv){
	if(argc != 2){
		exit(0);
	}
	
	FILE* fp1 = fopen(argv[1], "r");
	if(fp1 == NULL){
		exit(0);
	}
	
	char c;
	if(fscanf(fp1, "%c\t", &c) == EOF){
		exit(0);
	}
	char** matrix = allocate_matrix();
	int* arr = malloc(16 * sizeof(int));
	
	
	FILE* fp2 = fopen(argv[1], "r");
	if(fp2 == NULL){
		exit(0);
	}
	
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			char temp;
			fscanf(fp2, "%c\t", &temp);
			
			if(temp != '-'){
				for(int k = 0; k < i; k++){	//check col for dups
					if(matrix[k][j] == temp){
						printf("no-solution\n");
						free_everything(matrix, arr);
						exit(0);
					}
				}
				for(int l = 0; l < j; l++){ //check row for dups
					if(matrix[i][l] == temp){ 
						printf("no-solution\n");
						free_everything(matrix, arr);
						exit(0);
					}
				}
				int x = (i/4) * 4; //check subgrid for dups
				int y = (j/4) * 4;
				for(int k = x; k < (x+4); k++){
					for(int l = y; l < (y+4); l++){
						if(matrix[k][l] == temp){
							printf("no-solution\n");
							free_everything(matrix, arr);
							exit(0);
						}
					}
				}
	
			}	
			matrix[i][j] = temp;
		}
	}
	
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			if(matrix[i][j] == '-'){
				for(int i = 0; i < 16; i++){ //resets arr to 0's
					arr[i] = 0;
				}
				arr = solve_columns_rows(matrix, arr, i, j);
				arr = solve_subgrids(matrix, arr, i, j);
				
				int count = 0;
				int index_solution = -1;
				for(int k = 0; k < 16; k++){			
					if(arr[k] == 0){ //solution(s) found
						index_solution = k;
						count++;
					}
				}
				if(count >= 1){	//possible solutions available			
					if(count == 1){ // ONLY 1 SOL'N - 100% certain						
						if(index_solution == 10){
							matrix[i][j] = 'A';
						}else if(index_solution == 11){
							matrix[i][j] = 'B';
						}else if(index_solution == 12){
							matrix[i][j] = 'C';
						}else if(index_solution == 13){
							matrix[i][j] = 'D';
						}else if(index_solution == 14){
							matrix[i][j] = 'E';
						}else if(index_solution == 15){
							matrix[i][j] = 'F';
						}else if(index_solution < 10){
							char temp = index_solution + '0';
							matrix[i][j] = temp;
						}
						i = 0; //restart matrix traversal
						j = -1; 
					
					}else{ //multiple solutions --> guess!
						matrix = guess(matrix, arr); 
					}
				}else if(count == 0){ //no solution
					printf("no-solution\n");
					free_everything(matrix, arr);
					exit(0);
				}
			}
		}
	}
	
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			printf("%c", matrix[i][j]);
			if(j != 15){
				printf("\t");
			}
		}
		printf("\n");
	}
	free_everything(matrix, arr);

	fclose(fp1);
	fclose(fp2);
	return 0;
}
