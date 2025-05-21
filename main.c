#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#include "main.h"




/** TODO
 * Change tokeizer so it works with multi digit numbers
 * Implement a --help
 * Choice between int and float
 * How to see if legal input 
 * 
 */



struct type *tokenized_list;
 
 

 static int get_priority(char op){
    switch(op){
        case ADD: return ADD_PRECEDENDCE;
        case SUB: return SUB_PRECEDENCE;
        case MUL: return MUL_PRECEDENCE;
        case DIV: return DIV_PRECEDENCE;
        case POW: return POW_PRECEDENCE;
    }
 }

 static int get_association(char op){
    switch(op){
        case ADD: return LEFT_ASSOCIATIVE;
        case DIV: return LEFT_ASSOCIATIVE;
        case MUL: return LEFT_ASSOCIATIVE;
        case SUB: return LEFT_ASSOCIATIVE;
    }
 }

static int is_operator(char op){
    return (op == ADD  \
    || op == SUB || op == POW || op == MUL || op == DIV);
}

static int is_left_bracket(char op){
    return (op == LEFT_BRACKET);
}

static int is_right_bracket(char op){
    return (op == RIGHT_BRACKET);
}

void print_output(int index, char *output){
    for(int i = 0; i < index; i++){
        printf("queue: %c\n", output[i]);
    }
}

int main(int argc, char **argv){
    // int result;
    // char *token;
    // char output[100];
    // char operator_stack[100];
    // int stack_index = 0;
    // int output_index = 0;
    int num_tokens = 0;
    int tokens_rpn; // Brackets are removed in RPN

    if (argc < 1){
        printf("No arguments\n");
        return -1;
    }

    tokenizer(argv[1], &num_tokens);
    struct type rpn_list[num_tokens]; 

    make_RPN(rpn_list, num_tokens, &tokens_rpn);


    free(tokenized_list);
    tokenized_list = NULL;

    calculate_RPN(rpn_list, tokens_rpn);

}



static int use_operator(int left_value, int right_value, char operator){
    printf("Calculating.  %d %c %d\n", left_value, operator, right_value);

    switch(operator){
        case ADD: return left_value+right_value;
        case SUB: return left_value-right_value;
        case MUL: return left_value*right_value;
        case POW: return (int)pow((double) left_value, (double) right_value);
        case DIV: return left_value/right_value;
    }
    return -1;
}

/**
 * TODO
 * Error checking
 */
static void tokenizer(char *argv, int *num_tokens){
    struct type tokenized_intermediate_list[strlen(argv)];

    char *str = argv;
    char *end;
    int base = 10;
    size_t n = 0;
    // Go through the string, tokenize using 
    while(*str){
        // If its a digit it may be using several digits, so we use strtol() to extract this. 
        if(isdigit((unsigned char)*str)){
            long value = strtol(str, &end, base);
            struct type token;
            token.type = DIGIT;
            token.val = (int)value;
            tokenized_intermediate_list[n++] = token;
            str = end;
        } 
        else if(is_operator((unsigned char)*str)){
            struct type token;
            token.type = OPERAND;
            token.caracter = (unsigned char)*str;
            tokenized_intermediate_list[n++] = token;
            str++;
        }
        else if(is_left_bracket((unsigned char)*str) || is_right_bracket((unsigned char)*str)){
            struct type token;
            token.type = BRACKET;
            token.caracter = (unsigned char)*str;
            tokenized_intermediate_list[n++] = token;
            str++;
        }
    }

    tokenized_list = malloc(sizeof(struct type) * n);
    for(int i = 0; i < n; i++){
        tokenized_list[i] = tokenized_intermediate_list[i];
    }
    *num_tokens = n;


}

static void make_RPN(struct type *output_list, int num_tokens, int *tokens_rpn){

   
    int output_index = 0;
    struct type operator_stack[num_tokens];
    int operator_stack_index = 0;
    struct type *stack_top = operator_stack;


        for(int i = 0; i < num_tokens;  i++){
            struct type token = tokenized_list[i];

            if(tokenized_list[i].type == DIGIT){
                output_list[output_index++] = tokenized_list[i];
            }

            else if(token.type == OPERAND){
                while(operator_stack_index > 0 && stack_top->type==OPERAND){
                    if(((get_association((token.caracter)) == LEFT_ASSOCIATIVE) \
                    && get_priority(token.caracter) <= get_priority(stack_top->caracter))\
                        || ((get_association(token.caracter) == RIGHT_ASSOCIATIVE) \
                        && get_priority(token.caracter) < get_priority(stack_top->caracter)))
                        {
                            output_list[output_index++] = operator_stack[--operator_stack_index];
                            stack_top = &operator_stack[operator_stack_index-1];
                            continue;
                        }
                        break;
                }

                operator_stack[operator_stack_index++] = token;
                stack_top = &operator_stack[operator_stack_index-1];
            }
            else if(is_left_bracket(token.caracter)){
                operator_stack[operator_stack_index++] = token;
                stack_top = &operator_stack[operator_stack_index-1];

            }
            else if(is_right_bracket(token.caracter)){

                while(output_index > 0 &&  !is_left_bracket(stack_top->caracter)){ // While not ' ( '
                    output_list[output_index++] = operator_stack[--operator_stack_index];
                    stack_top = &operator_stack[operator_stack_index-1];
                } // Remove ' ( '
                operator_stack_index--;
                stack_top =  &operator_stack[operator_stack_index-1];
            }
            


        }

    //Pop stack onto queue
    while(operator_stack_index > 0){
        operator_stack_index--;
        output_list[output_index++] = operator_stack[operator_stack_index];
    }

    *tokens_rpn = output_index;

    for(int i = 0; i < output_index; i++){
        if(output_list[i].type == DIGIT){
            printf("output queue %d\n", output_list[i].val);
        }
        else{
            printf("output queue %c\n", output_list[i].caracter);
        }
    }

}

static int calculate_RPN(struct type *list, int num_tokens){

        // // Now evaluate expression in Reverse Polish Notation

    // int output_stack[100];
    struct type output_stack[num_tokens];
    int output_stack_index = 0;

    for(int i = 0; i < num_tokens; i++){
        struct type token = list[i];

        if(token.type == DIGIT){ // Push output on stack
            output_stack[output_stack_index++] = token;
        }
        else if(token.type == OPERAND){ // If operand pop 2 of stack and evaluate
            int first, second, calc;
            second = output_stack[--output_stack_index].val;
            first = output_stack[--output_stack_index].val;

            calc = use_operator(first, second, token.caracter);
            struct type calculated;
            calculated.type = DIGIT;
            calculated.val = calc;

            output_stack[output_stack_index++] = calculated;
        }

    }

    printf("Sum = %d\n", output_stack[0].val);
    return 1;



};