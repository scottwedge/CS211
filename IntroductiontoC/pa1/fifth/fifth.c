#include<stdio.h>
#include<stdlib.h>

int** allocate_matrix(int, int);
int check_diagonals(int**, int, int);
int check_horizontals(int**, int, int);
int check_verticals(int**, int, int);
void check_duplicates(int**, int);

//ALLOCATES SPACE FOR MATRIX
int** allocate_matrix(int rows, int cols){
	int** ret = malloc(rows * sizeof(int*));
	for(int i = 0; i < rows; i++){
		ret[i] = malloc(cols * sizeof(int));
	}
	return ret;
}

//CHECKS DIAGONALS --> IF SAME, RETURN 1; IF FALSE RETURN 0
int check_diagonals(int** matrix, int n, int target){	
	int sum = 0;
	for(int i = 0; i < n; i++){
		sum = sum + matrix[n-1-i][i];
	}
	if(sum == target){
		return 1;
	}else{
		return 0;
	}
	
}

//CHECKS HORIZONTONALS --> IF SAME, RETURN 1; IF FALSE RETURN 0
int check_horizontals(int** matrix, int n, int target){
	int sum = 0;	
	for(int r = 0; r < n; r++){
		for(int c = 0; c < n; c++){
			sum = sum + matrix[r][c];
		}
		if(sum == target){
			sum = 0;
		}else{
			return 0;
		}
	}
		
	return 1;

}

//CHECKS VERTICALS --> IF SAME RETURN 1, IF FALSE RETURN 0
int check_verticals(int** matrix, int n, int target){
	int sum = 0;
	for(int c = 0; c < n; c++){
		for(int r = 0; r < n; r++){
			sum = sum + matrix[r][c];
		}
		if(sum == target){
			sum = 0;
		}else{
			return 0;
		}
	}
	
	return 1;
}

//CHECKS FOR DUPLICATES
void check_duplicates(int** matrix, int n){
	int square = (n * n);
	int found = 0; // if found change to 1
	for(int i = 1; i <= square; i++){
		
		for(int j = 0; j < n; j++){
			for(int k = 0; k < n; k++){
				if(i == matrix[j][k]){
					found = 1;	
					break;			
				}
			}
		}
		if(found == 1){
			found = 0;
		}else{
			printf("not-magic"); //there must be one int 1 through n^2
			exit(0);	
		}	
	}
}

int main(int argc, char** argv){
	if(argc != 2 ){
		exit(0);
	}		

	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		exit(0);
	}
	
	int n;
	if(fscanf(fp, "%d\n", &n) == EOF){
		exit(0);
	}
	int** matrix = allocate_matrix(n, n);

//SCANNING IN VALUES FROM FILE INTO MATRIX
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				int temp;				
				fscanf(fp, "%d\t", &temp);
				matrix[i][j] = temp;
			}
		}
//CHECKS FOR DUPLICATE VALUES	
	check_duplicates(matrix, n);

//CHECKS FIRST DIAGONAL
	int sum = 0;	
	for(int i = 0; i < n; i++){
		sum = sum + matrix[i][i];
	}
	int total = sum;

	if(check_diagonals(matrix, n, total) == 0){
		printf("not-magic\n");
		//printf("failed diagonals");
		exit(0);
	}else if(check_horizontals(matrix, n, total) == 0){
		printf("not-magic\n");
		//printf("failed horizontals");
		exit(0);
	}else if(check_verticals(matrix, n, total) == 0){
		printf("not-magic\n");
		//printf("failed verticals");
		exit(0);
	}else{
		printf("magic\n");
	}
	
	//FREES MATRIX
	int x;	
	for(x = 0; x < n; x++){
		free(matrix[x]);
	}
	free(matrix);
	

	fclose(fp);
	return 0;
}
