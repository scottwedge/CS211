#include<stdio.h>
#include<stdlib.h>

int** allocate_matrix(int, int);
int** multiply(int**, int**, int, int, int, int);
void free_matrix(int**, int, int);
	
//ALLOCATES SPACE FOR MATRIX
int** allocate_matrix(int r, int c){
	int** ret = malloc(r * sizeof(int*));
	int i;
	for(i = 0; i < r; i++){
		ret[i] = malloc(c * sizeof(int));
	}
	return ret;
}


//MULTIPLIES MATRICES & STORES IN NEW MATRIX
int** multiply(int** mtx1, int** mtx2, int r1, int c1, int r2, int c2){
	int** product;
	int i, j, k;
	product = allocate_matrix(r1, c2);
	for(i = 0; i < r1; i++){
		for(j = 0; j < c2; j++){
			product[i][j] = 0;
			for(k = 0; k < r2; k++){
				(*(*(product + i) + j)) += (*(*(mtx1 + i) + k)) * (*(*(mtx2 + k) + j));
			}
		}
	}
	return product;


}

//FREE THE MATRIX
void free_matrix(int** matrix, int r, int c){
	int i;	
	for(i = 0; i < r; i++){
		free(matrix[i]);
	}
	free(matrix);
}

int main(int argc, char** argv){
	
	if(argc != 2){
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		exit(0);
	}
	int r1 = 0;
	int c1 = 0;
	if(fscanf(fp, "%d\t%d\n", &r1, &c1) == EOF){
		exit(0);
	}
	//checks if no matrix
	if(r1 == 0 || c1 == 0){
		printf("bad-matricies");
		exit(0);
	}
	//create first matrix
	int** matrix1;
	int i, j;
	matrix1 = allocate_matrix(r1, c1);
	for(i = 0; i < r1; i++){
		for(j = 0; j < c1; j++){
			int temp;
			fscanf(fp, "%d\t", &temp);
			matrix1[i][j] = temp;
		}
	}
	
	int r2 = 0;
	int c2 = 0;
	fscanf(fp, "%d\t%d", &r2, &c2);
	
	//checks if no matrix
	if(r2 == 0 || c2 == 0){
		printf("bad-matrices");
		exit(0);
	}	

	if(r2 != c1){
		printf("bad-matrices");
		exit(0);
	}

	//create second matrix
	int** matrix2;
	int k, l;
	matrix2 = allocate_matrix(r2, c2);

	for(k = 0; k < r2; k++){
		for(l = 0; l < c2; l++){
			int temp;
			fscanf(fp, "%d\t", &temp);
			matrix2[k][l] = temp;
		}	
	}
	
		
	//to multiply mtx, c1 must = r2	
	int** product;
	product = multiply(matrix1, matrix2, r1, c1, r2, c2);
	
	int m, n;
	for(m = 0; m < r1; m++){
		for(n = 0; n < c2; n++){
			printf("%d\t", product[m][n]);
		}
		printf("\n");
	}
	
	free_matrix(matrix1, r1, c1);
	free_matrix(matrix2, r2, c2);
	free_matrix(product, r1, c2);

	fclose(fp);
	return 0;
}
