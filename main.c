#include <stdio.h>
#include <ctype.h>
#include <string.h>
 #include <stdlib.h>





/** TODO
 * Implement a --help
 * Choice between int and float
 * How to see if legal input 
 */

 #define ADD '+'
 static int use_operator(int left_value, int right_value, char operator);

 int operator_stack[10];
 int stack_index = 0;


static int is_operator(char op){
    return (op == ADD);
}

int main(int argc, char **argv){
    int result;
    char *token;
    char output[10];
    int output_index = 0;


    if (argc < 1){
        printf("No arguments\n");
        return -1;
    }


    token = argv[1];
    while(*token != '/'){
        if(isdigit((int)*token)){
            printf("digit\n");
            output[output_index++] = *token;
        }
        else if(is_operator(*token)){
                printf("operator\n");
                operator_stack[stack_index++] = *token;
        }
        token++; // Onto the next
    }
    // Pop stack onto queue
    while(stack_index > 0){
        stack_index--;
        output[output_index++] = operator_stack[stack_index];
    }

    

    // Now evaluate expression in Reverse Polish Notation
    char output_stack[10];
    int output_stack_index = 0;
    int return_value;
    for(int i = 0; i < output_index+1; i++){
        char tok = output[i];
        
        if(isdigit((unsigned char) tok)){ // Then push on output stack
            output_stack[output_stack_index] = tok;
            output_stack_index++;
        }
        else{ // If operand pop 2 of stack and evaluate
            int first, second;
            output_stack_index--;
            second = output_stack[output_stack_index] - '0';
            output_stack_index--;
            second = output_stack[output_stack_index] - '0';
            return_value = use_operator(first, second, outp ut[i]);
        }

    }

    printf("value: %d\n", return_value);


    return 1;


}



static int use_operator(int left_value, int right_value, char operator){
    printf("values = %d %d\n", left_value, right_value);
    switch(operator){
        case ADD: return left_value+right_value;
    }
    return 100;
}