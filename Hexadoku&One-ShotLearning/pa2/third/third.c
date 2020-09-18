#include<stdio.h>
#include<stdlib.h>

double** allocate_matrix(int, int);	
double** transpose_matrix(double**, int, int); 
double** invert_matrix(double**, int); 
double** rr_down(double**, int, int, int);
double** rr_up(double**, int, int, int);
double** multiply_matrices(double**, double**, int, int, int, int); 
void free_matrix(double**, int, int);

//	ALLOCATION METHOD
double** allocate_matrix(int r, int c){
	double** ret = malloc(r * sizeof(double*));
	for(int i = 0; i < r; i++){
		ret[i] = malloc(c * sizeof(double));
	}
	return ret;
}
//	TRANSPOSE METHOD
double** transpose_matrix(double** matrix, int r, int c){
	double** ret = allocate_matrix(c, r);
	for(int i = 0; i < c; i++){
		for(int j = 0; j < r; j++){
			ret[i][j] = matrix[j][i];
		}
	}
	return ret;
}
//	MULTIPLICATION METHOD
double** multiply_matrices(double** mtx1, double** mtx2, int r1, int c1, int r2, int c2){
	//if(r2 == c1) necessary?
	double** product = allocate_matrix(r1, c2);
	for(int i = 0; i < r1; i++){
		for(int j = 0; j < c2; j++){
			product[i][j] = 0;
			for(int k = 0; k < r2; k++){
				(*(*(product + i) + j) += (*(*(mtx1 + i) + k)) * (*(*(mtx2 + k) + j)));
			}
		}
	}
	return product;
}
//	INVERSION METHOD
double** invert_matrix(double** matrix, int n){
	int o = 2*n;
	double** rref = allocate_matrix(n, o);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			rref[i][j] = matrix[i][j];
		}
	}
	int row = 0;
	int col = n;
	for(int i = 0; i < n; i++){
		for(int j = n; j < o; j++){
			if(i == row && j == col){
				rref[i][j] = 1.0;
				row++;
				col++;
			}else{
				rref[i][j] = 0.0;
			}
		}
	}

	for(int i = 0; i < n; i++){
		rref = rr_down(rref, i, n, o);
	}
	for(int i = (n-1); i >= 0; i--){
		rref = rr_up(rref, i, n, o);
	}

	double** ret = allocate_matrix(n, n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			ret[i][j] = rref[i][j+n];
		}
	}
	free_matrix(rref, n, o);
	return ret;
}

double** rr_down(double** matrix, int n, int num_rows, int num_cols){
	double scale = 0.0;
	double pivot = matrix[n][n]; 
	
	for(int i = 0; i < num_cols; i++){ 
		matrix[n][i] /= pivot;
	}
		
	for(int j = (n+1); j < num_rows; j++){
		scale = matrix[j][n];
		for(int k = 0; k < num_cols; k++){
			matrix[j][k] = (matrix[j][k] - (scale * matrix[n][k]));
		}
	}
	return matrix;
}

double** rr_up(double** matrix, int n, int num_rows, int num_cols){
	double scale = 0.0;	
	for(int i = (n-1); i >= 0; i--){
		scale = matrix[i][n];
		for(int j = 0; j < num_cols; j++){
			matrix[i][j] = (matrix[i][j] - (scale * matrix[n][j]));
		}	
	}
	return matrix;
}
//FREE METHOD
void free_matrix(double** matrix, int r, int c){
	for(int i = 0; i < r; i++){
		free(matrix[i]);
	}
	free(matrix);
}
//MAIN
int main(int argc, char** argv){
	if(argc != 3){
		exit(0);
	}
	FILE* fp_train = fopen(argv[1], "r");
	if(fp_train == NULL){
		fclose(fp_train);
		exit(0);
	}
	
	int k; //number of attributes
	if(fscanf(fp_train, "%d\n", &k) == EOF){
		fclose(fp_train);
		exit(0);
	}
	int n; //number training examples
	if(fscanf(fp_train, "%d\n", &n) == EOF){
		fclose(fp_train);
		exit(0);
	}
		
	double** X = allocate_matrix(n, (k+1));
	double** Y = allocate_matrix(n, 1);
	double** W = allocate_matrix((k+1), 1);
	
	for(int i = 0; i < n; i++){		
		for(int j = 0; j < (k+1); j++){
			if(j == 0){
				X[i][j] = 1.0;
			}else{		
				double temp1;
				fscanf(fp_train, "%lf,", &temp1);
				X[i][j] = temp1;
			}
	
		}
			double temp2;
			fscanf(fp_train, "%lf\n", &temp2);
			Y[i][0] = temp2;
	}
	double** Xtransposed = transpose_matrix(X, n, (k+1));
	double** product1 = multiply_matrices(Xtransposed, X, (k+1), n, n, (k+1));
	double** inverse = invert_matrix(product1, (k+1));
	double** product2 = multiply_matrices(inverse, Xtransposed, (k+1), (k+1), (k+1), n); 
	W = multiply_matrices(product2, Y, (k+1), n, n, 1);

	FILE* fp_test = fopen(argv[2], "r");
	if(fp_test == NULL){
		fclose(fp_train);
		fclose(fp_test);
		exit(0);
	}
	int m;
	if(fscanf(fp_test, "%d\n", &m) == EOF){
		fclose(fp_train);
		fclose(fp_test);
		exit(0);
	}

	for(int i = 0; i < m; i++){
		double value = W[0][0];
		for(int j = 1; j < (k+1); j++){
			double x;
			fscanf(fp_test, "%lf,", &x);
			value += (W[j][0] * x);
		}
		printf("%0.0lf\n", value);
	}

	free_matrix(X, n, (k+1));
	free_matrix(Y, n, 1);
	free_matrix(W, (k+1), 1);
	free_matrix(Xtransposed, (k+1), n);
	free_matrix(product1, (k+1), (k+1));
	free_matrix(inverse, (k+1), (k+1));
	free_matrix(product2, (k+1), n);
	
	fclose(fp_train);
	fclose(fp_test);
	return 0;
}
