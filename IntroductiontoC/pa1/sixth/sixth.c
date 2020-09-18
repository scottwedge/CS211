#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int check_vowel(char);
void pig_latin_c(char*);
void pig_latin_v(char*);

//CHECKS IS GIVEN CHAR IS A VOWEL
int check_vowel(char c){
	int flag = 0; //return true = 1, false = 0
	if(c == 'a' || c == 'e' || c == 'i' || c == 'o'|| c == 'u'){ //check lowercase
		flag = 1;
	}else if(c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'){ //check uppercase
		flag = 1;
	}
	
	return flag;
}

//ARGUMENT WITH CONSONANT
void pig_latin_c(char* word){
	int swap = 0;
	int length = strlen(word);
	for(int i = 0; i < length; i++){
		if(check_vowel(word[i]) == 1){
			swap = i;
			break;
		}
	}
	if(swap != 0){
		for(int i = swap; i < length; i++){
			printf("%c", word[i]);
		}

		for(int i = 0; i < swap; i++){
			printf("%c", word[i]);
		}
		
	}else{ //no vowel found to swap so just print out whole word
		for(int i = 0; i < length; i++){
			printf("%c", word[i]);
		}
	}
	printf("ay");
}


//ARGUMENT WITH VOWEL ADD AY
void pig_latin_v(char* word){
	int length = strlen(word);
	
	for(int i = 0; i < length; i++){
		printf("%c", word[i]);
	}
	printf("yay");
}


int main(int argc, char** argv){
	if(argc < 2){
		exit(0);
	}
	
	for(int i = 1; i < argc; i++){
		char* temp;
		temp = argv[i];
		char letter;
		letter = temp[0];

		if(check_vowel(letter) == 1){	
			pig_latin_v(temp); //is a vowel
		}else{
			pig_latin_c(temp); //is a consonant
		}

		if(i != (argc - 1)){ //if not last argument, print space after
			printf(" ");
		}else{
			printf("\n");
		}
	}
	
	return 0;
}
