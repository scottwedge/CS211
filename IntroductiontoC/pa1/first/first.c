#include<stdio.h>
#include<stdlib.h>

int isPrime(int);

//CHECKS IF NUMBER IS PRIME
int isPrime(int n){ // 0 = false, 1 = true		
	if(n <= 1){
		return 0;
	}	
	for(int i = 2; i < n; i++){
		if(n % i == 0){
			return 0;
		}
	}

	return 1;
		
}

int main(int argc, char** argv){
	if(argc != 2){
		exit(0);
	}

	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		exit(0);
	}
	
	int num = 0;
	if(fscanf(fp, "%d\n", &num) == EOF){
		exit(0);
	}
	
	int arr[num];
	for(int i = 0; i < num; i++){
		int temp;		
		fscanf(fp, "%d\n", &temp);
		arr[i] = temp;
	}

	int temp = 0;
	for(int i = 0; i < num; i++){
		int flag = 1;
		temp = arr[i];
		if(temp <= 1){ //AUTOMATICALLY NOT PRIME
			printf("no\n"); 
		}else{
			while(temp > 0){
				if(isPrime(temp) == 0){
					flag = 0;
				}
				temp = temp / 10;
			}
			if(flag == 0){
				printf("no\n");
			}else{
				printf("yes\n");
			}
		}
	}

	fclose(fp);
	return 0;
}
