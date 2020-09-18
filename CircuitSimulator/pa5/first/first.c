#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>

typedef struct node{
    char* name;
	int type; //0 = input, 1 = output, 2 = temp, -1 = selector(not really needed), 3 = 1/0 node
    int data;
    struct node* next;
}node;

typedef struct gate{
    char* type;
	int id;
    struct node* inputs;
    struct node* outputs;
    struct node* selectors;
    struct gate* next;
}gate;

int gray_code(int number){
    int ret = number ^ (number >> 1);
    return ret;
}

int and_gate(node* input){
	//printf("AND\n");
	node* input1 = input;
	node* input2 = input->next;
//	printf("%s: %d\n", input1->name, input1->data);
//cd	printf("%s: %d\n", input2->name, input2->data);

    if(input1->data == 1 && input2->data == 1){
		//printf("AND OUTPUT: 1\n");
//		printf("output: 1\n");
        return 1;
    }
	//printf("AND OUTPUT: 0\n");
//	printf("output: 0\n");
    return 0;
}

int or_gate(node* input){
	// printf("\n");
	// for(node* ptr = input; ptr != NULL; ptr = ptr->next){
	// 	printf("%s\t", ptr->name);
	// }
	//printf("\n");
	//printf("OR\n");
	node* input1 = input;
	node* input2 = input->next;
//	printf("%s: %d\n", input1->name, input1->data);
//	printf("%s: %d\n", input2->name, input2->data);

    if(input1->data == 1 || input2->data == 1){
//		printf("output: 1\n");
    	return 1;
	}
//	printf("output: 0\n");
    return 0;
}

int not_gate(node* input){
    if(input->data == 1){
//		printf("output: 0\n");
        return 0;
    }
//	printf("output: 1\n");
	return 1;
}

int xor_gate(node* input){
	node* input1 = input;
	node* input2 = input->next;
//	printf("%s: %d\n", input1->name, input1->data);
//	printf("%s: %d\n", input2->name, input2->data);
    if(input1->data == 1 && input2->data == 0){
//		printf("output: 1\n");
        return 1;
    }else if(input1->data == 0 && input2->data == 1){
//		printf("output: 1\n");
        return 1;
    }
//	printf("output: 0\n");
    return 0;
}

int nand_gate(node* input){
	node* input1 = input;
	node* input2 = input->next;
//printf("%s: %d\n", input1->name, input1->data);
///	printf("%s: %d\n", input2->name, input2->data);
     if(input1->data == 1 && input2->data == 1){
		 //printf("output: 0\n");
        return 0;
    }
	//printf("output: 1\n");
    return 1;
}

int nor_gate(node* input){
	node* input1 = input;
	node* input2 = input->next;
	//	printf("%s: %d\n", input1->name, input1->data);
//	printf("%s: %d\n", input2->name, input2->data);
    if(input1->data == 1 || input2->data == 1){
	//	printf("output: 0\n");
        return 0;
    }
	//printf("output: 1\n");
    return 1;
}

int xnor_gate(node* input){
	node* input1 = input;
	node* input2 = input->next;
	//printf("%s: %d\n", input1->name, input1->data);
	//printf("%s: %d\n", input2->name, input2->data);
	if(input1->data == 1 && input2->data == 0){
		//printf("output: 0\n");
        return 0;
    }else if(input1->data == 0 && input2->data == 1){
		//printf("output: 0\n");
        return 0;
    }
//	printf("output: 1\n");
    return 1;
}

node* decoder_gate(node* input, node* output){
	int count = 0;
	for(node* ptr = input; ptr != NULL; ptr = ptr->next){
		count++;
	}
	int value = 0;
	node* curr = input;
	for(int i = count; i > 0; i--){
		value += (curr->data << (i-1));
		curr = curr->next;
	}

	int inv = 0;
	for(int i = value; i != 0; i = i >> 1){
		inv ^= i;
	}
	count = 0;
	for(node* ptr = output; ptr != NULL; ptr = ptr->next){
		if(count == inv){
			ptr->data = 1;
		}else{
			ptr->data = 0;
		}
		count++;
	}
	return output;
}

int multiplexer_gate(node* input, node* selectors){
	int count = 0;
	for(node* ptr = selectors; ptr != NULL; ptr = ptr->next){
		count++;
	}
	int value = 0;
	node* curr = selectors;
	for(int i = count; i > 0; i--){
		value += (curr->data << (i-1));
		curr = curr->next;
	}
	int inv = 0;
	for(int i = value; i != 0; i = i >> 1){
		inv ^= i;
	}
	node* ptr = input;
	for(int i = 0; i < inv; i++){
		ptr = ptr->next;
	}
	return ptr->data;
}

node* load_inputs(node* variables, int n, int num_inputs){
	int gray = gray_code(n); 
	node* ptr = variables;
	while(ptr->type != 0){
		ptr = ptr->next;
	}
	for(int j = num_inputs; j > 0; j--){
		ptr->data = ((gray >> (j-1)) % 2);
		printf("%d ", ptr->data);
		ptr = ptr->next;
	}
	return variables;
}

node* insert_node(node* linked_list, char* name, int type){
	node* temp = malloc(sizeof(node));
	char* new_name = malloc(1000*sizeof(char));
	new_name = name;
	temp->name = new_name;
	if(type == 3){
		temp->data = atoi(name);
		temp->type = 0;
	}else{
		temp->type = type;
		temp->data = -1;
	}
	temp->next = NULL;

	if(linked_list == NULL){
		linked_list = temp;
	}else{
		node* ptr = linked_list; 
		while(ptr->next != NULL){
	 		ptr = ptr->next;
		}
		ptr->next = temp;
	}
	return linked_list;
}

gate* insert_gate(gate* circuit, char* type, FILE* fp, int id){
	node* inputs = NULL;
	node* outputs = NULL;
	node* selectors = NULL;	
	int n = 0;
	if(strcmp(type, "NOT") == 0){
		char* str1 = malloc(1000*sizeof(char));
		fscanf(fp, "%s", str1);
		inputs = insert_node(inputs, str1, 0);
		char* str2 = malloc(1000*sizeof(char));
		fscanf(fp, "%s", str2);
		outputs = insert_node(outputs, str2, 1);
	}else if((strcmp(type, "AND") == 0) || (strcmp(type, "OR") == 0) || (strcmp(type, "XOR") == 0) || (strcmp(type, "NAND") == 0) || (strcmp(type, "NOR") == 0) || (strcmp(type, "XNOR") == 0)){
		char* str3 = malloc(1000*sizeof(char));
		fscanf(fp, "%s", str3);
		inputs = insert_node(inputs, str3, 0);
		char* str4 = malloc(1000*sizeof(char));		
		fscanf(fp, "%s", str4);
		inputs = insert_node(inputs, str4, 0);
		char* str5 = malloc(1000*sizeof(char));
		fscanf(fp, "%s", str5);
		outputs = insert_node(outputs, str5, 1);
	}else if(strcmp(type, "DECODER") == 0){
		fscanf(fp, "%d", &n);
		for(int i = 0; i < n; i++){
			char* str6 = malloc(1000*sizeof(char));
			fscanf(fp, "%s", str6);
			inputs = insert_node(inputs, str6, 0);
		}
		for(int i = 0; i < (pow(2,n)); i++){
			char* str7 = malloc(1000*sizeof(char));		
			fscanf(fp, "%s", str7);
			outputs = insert_node(outputs, str7, 1);
		}
	}else if(strcmp(type, "MULTIPLEXER") == 0){
		fscanf(fp, "%d", &n);
		for(int i = 0; i < n; i++){
			char* str8 = malloc(1000*sizeof(char));
			fscanf(fp, "%s", str8);
			inputs = insert_node(inputs, str8, 0);
		}
		n = (log2(n));
		for(int i = 0; i < n; i++){
			char* str9 = malloc(1000*sizeof(char));		
			fscanf(fp, "%s", str9);
			selectors = insert_node(selectors, str9, -1);
		}
		char* str10 = malloc(1000*sizeof(char));
		fscanf(fp, "%s", str10);
		outputs = insert_node(outputs, str10, 1);
	}
	gate* temp = malloc(sizeof(gate));
	temp->type = type;
	temp->id = id;
	temp->inputs = inputs;
	temp->outputs = outputs;
	temp->selectors = selectors;
	temp->next = NULL;
	
	if(circuit == NULL){
		circuit = malloc(sizeof(gate));
		circuit = temp;
	}else{
		gate* ptr = circuit; 
		while(ptr->next != NULL){
	 		ptr = ptr->next;
		}
		ptr->next = temp;
	}
	return circuit;	
}

int is_valid(node* variables, node* inputs, node* selectors){
	int found = 0;
	for(node* ptr = inputs; ptr != NULL; ptr = ptr->next){
		//printf("input variable: %s\t", ptr->name);
		for(node* ptr2 = variables; ptr2 != NULL; ptr2 = ptr2->next){
			//printf("variable to compare: %s\n", ptr2->name);
			if(strcmp(ptr->name, ptr2->name) == 0){
				found =  1;
			}
		}

		if(found == 0){
			return 0;
		}else{
			found = 0;
		}	
	}	
	found = 0;
	for(node* ptr = selectors; ptr != NULL; ptr = ptr->next){
		//printf("input variable: %s\t", ptr->name);
		for(node* ptr2 = variables; ptr2 != NULL; ptr2 = ptr2->next){
			//printf("variable to compare: %s\n", ptr2->name);
			if(strcmp(ptr->name, ptr2->name) == 0){
				found =  1;
			}
		}

		if(found == 0){
			return 0;
		}else{
			found = 0;
		}	
	}			

	return 1;
}

gate* update(gate* circuit, gate* node){
	gate* curr = circuit;
	gate* prev;
	if(circuit->id == node->id){ //delete if it is the first
		circuit = circuit->next;
	}else{
		while(curr->id != node->id){	//deletes if not first
			prev = curr;
			curr = curr->next;
		}
		prev->next = curr->next;
	}
	node->next = NULL;

	if(circuit == NULL){
		circuit = node;
		return circuit;
	}
	gate* ptr = circuit;
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = node;
	return circuit;
}

node* update_output(node* variables, node* outputs, int output){
	int found = 0;
	node* curr = outputs;
	while(curr != NULL){
		for(node* ptr = variables; ptr != NULL; ptr = ptr->next){
			if(strcmp(curr->name, ptr->name) == 0){
				ptr->data = output;
				found = 1;
				break;
			}
		}
		if(found == 0){
			variables = insert_node(variables, curr->name, 2);
			variables = update_output(variables, outputs, output);
		}
		curr = curr->next;
		found = 0;
	}	
	return variables;
}

node* update_inputs(node* variables, node* inputs){
	for(node* ptr = inputs; ptr != NULL; ptr = ptr->next){
		//printf("input name to compare: %s\n", ptr->name);
		for(node* curr = variables; curr != NULL; curr = curr->next){
			if(strcmp(ptr->name, curr->name) == 0){
				//printf("same name: %s\n", ptr->name);
				ptr->data = curr->data;
				break;
			}
		}
	}
	return inputs;
}

void solve_circuit(gate* circuit, node* variables, int num_outputs){
	int output;
	gate* prev = NULL;
	gate* ptr = circuit;
	while(ptr != NULL){
	//for(gate* ptr = circuit; ptr != NULL; ptr = ptr->next){
		if(is_valid(variables, ptr->inputs, ptr->selectors) == 0){ //move gate to the end of circuit
			circuit = update(circuit, ptr);
			if(prev != NULL){
				ptr = prev;
			}else{
				ptr = circuit;
			}
		}else{
			if(strcmp(ptr->type, "NOT") == 0){	//execute gate
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = not_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "AND") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = and_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "OR") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = or_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "NOR") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = nor_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "NAND") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = nand_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "XOR") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = xor_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "XNOR") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				output = xnor_gate(ptr->inputs);
				variables = update_output(variables, ptr->outputs, output);
			}else if(strcmp(ptr->type, "DECODER") == 0){
				ptr->inputs = update_inputs(variables, ptr->inputs);
				ptr->outputs = update_inputs(variables, ptr->outputs);
				node* ptr1 = malloc(sizeof(node));
				ptr1 = decoder_gate(ptr->inputs, ptr->outputs);
				while(ptr1 != NULL){
					output = ptr1->data;
					variables = update_output(variables, ptr1, output);
					ptr1 = ptr1->next;
				}
			}else{ //multiplexer
				ptr->inputs = update_inputs(variables, ptr->inputs);
				ptr->selectors = update_inputs(variables, ptr->selectors);
				output = multiplexer_gate(ptr->inputs, ptr->selectors);
				variables = update_output(variables, ptr->outputs, output);
			}
		}
		prev = ptr;
		ptr = ptr->next;
	}

	for(node* ptr = variables; ptr != NULL; ptr = ptr->next){
		if(ptr->type == 1){
			printf("%d ", ptr->data);
			num_outputs--;
			if(num_outputs > 0){
				printf(" ");
			}
		}
	}
}

void free_lists(node* variables, gate* circuits){
	node* temp = variables;
	while(temp != NULL){
		node* temp2 = temp;
		temp = temp->next;
		free(temp2);
	}

	gate* temp1 = circuits;
	while(temp1 != NULL){
		gate* temp3 = temp1;
		temp1 = temp1->next;
		free(temp3);
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
	int num_inputs;
	int num_outputs;
	int count = 0;
    node* variables = NULL;
    gate* circuit = NULL;

	char* str = malloc(1000*sizeof(char));
	while(fscanf(fp, "%s ", str) > 0){
		if(strcmp(str, "INPUTVAR") == 0){
			fscanf(fp, "%d", &num_inputs);
			for(int i = 0; i < num_inputs; i++){
				char* str1 = malloc(1000*sizeof(char));
				fscanf(fp, "%s", str1);
				variables = insert_node(variables, str1, 0);
			}
		}else if(strcmp(str, "OUTPUTVAR") == 0){
			fscanf(fp, "%d", &num_outputs);
			for(int i = 0; i < num_outputs; i++){
				char* str2 = malloc(1000*sizeof(char));
				fscanf(fp, "%s", str2);
				variables = insert_node(variables, str2, 1);
			}
		}else{
			//printf("the type of gate being inserted is: %s\n", str);
			char* str3 = malloc(1000*sizeof(char));
			strcpy(str3, str);
			//str3 = str;
			circuit = insert_gate(circuit, str3, fp, count);
			count++;
		}
	}
	
	variables = insert_node(variables, "0", 3);
	variables = insert_node(variables, "1", 3);
	
	for(int i = 0; i < (pow(2, num_inputs)); i++){
		variables = load_inputs(variables, i, num_inputs);
		solve_circuit(circuit, variables, num_outputs);
		printf("\n");
	}
	
	free_lists(variables, circuit);
	fclose(fp);
    return 0;
}
