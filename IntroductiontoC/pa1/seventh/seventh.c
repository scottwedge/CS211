#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv){

	if(argc < 2){	
		exit(0);
	}
	
    int size;
	for(int i = 1; i < argc; i++){
		size = strlen(argv[i]);
		printf("%c", argv[i][size - 1]);
	
	}
	
	printf("\n");

	return 0;
}
