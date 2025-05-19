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
#define SUB '-'


 static int use_operator(int left_value, int right_value, char operator);

 int operator_stack[10];
 int stack_index = 0;


static int is_operator(char op){
    return (op == ADD
    || op == SUB);
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
                // If theres an operator on stack we must set it on the output queue, 
                // can fix assosicate stuff later
                printf("operator\n");
                if(stack_index > 0){
                    output[output_index++] = operator_stack[--stack_index];

                }
                operator_stack[stack_index++] = *token;
        }
        token++; // Onto the next
    }
    // Pop stack onto queue
    while(stack_index > 0){
        stack_index--;
        output[output_index++] = operator_stack[stack_index];
    }

    for(int i = 0; i < output_index; i++){
        printf("output queue %c\n", output[i]);
    }

    // Now evaluate expression in Reverse Polish Notation
    int output_stack[10];
    int output_stack_index = 0;
    int return_value;
    for(int i = 0; i < output_index; i++){
        char tok = output[i];
        
        if(isdigit((unsigned char) tok)){ // Then push on output stack
            output_stack[output_stack_index] = tok - '0';
            output_stack_index++;
        }
        else if (is_operator(tok)){ // If operand pop 2 of stack and evaluate
            int first, second, calc;
            output_stack_index--;
            second = output_stack[output_stack_index];
            output_stack_index--;
            first = output_stack[output_stack_index];

            calc = use_operator(first, second, output[i]);
            output_stack[output_stack_index] = calc;
            output_stack_index++;
        }

    }


    // At this point there should only be one value on the output stack. 
    printf("Sum = %d\n", output_stack[0]);


    return 1;


}



static int use_operator(int left_value, int right_value, char operator){
    printf("use_operator. Values: %d %d\n", right_value, left_value);

    switch(operator){
        case ADD: return left_value+right_value;
        case SUB: return left_value-right_value;
    }
    return -1;
}