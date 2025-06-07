#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> 
#include <getopt.h>


#include "main.h"




/** TODO
 * FLOATS
 * SPACES
 * Choice between int and float
 * How to see if legal input, give feedback on whats illegal? For example: two operators after each other. 
 * Implement -- ?
 * 
 */



struct type *tokenized_list;
int base = 10; // Default base 
 

 static int get_priority(char op){
    switch(op){
        case ADD: return ADD_PRECEDENDCE;
        case SUB: return SUB_PRECEDENCE;
        case MUL: return MUL_PRECEDENCE;
        case DIV: return DIV_PRECEDENCE;
        case POW: return POW_PRECEDENCE;
    }
    return -1;
 }

 static int get_association(char op){
    switch(op){
        case ADD: return LEFT_ASSOCIATIVE;
        case DIV: return LEFT_ASSOCIATIVE;
        case MUL: return LEFT_ASSOCIATIVE;
        case SUB: return LEFT_ASSOCIATIVE;
    }
    return -1;
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




static void help_page(char **argv){
    printf("\n%s --- HELP PAGE ---\n", argv[0]);
    printf("  -h / --help for help page\n");
    printf("  -x for hexadecimal. Can be used with and without '0x'\n");
    exit(EXIT_SUCCESS);
}




int main(int argc, char **argv){
    
    int opt;
    while((opt = getopt(argc, argv, "hx") ) != -1){
        switch (opt){
            case 'h': help_page(argv);
            case 'x': base = 16;
        }
    }

    if(optind >= argc){
        fprintf(stderr, "Error: No expression given");
    }
    
    int num_tokens = 0;
    int tokens_rpn; // Brackets are removed in RPN
    // Optind is first expression thats not a flag

    if(strlen(argv[optind]) > MAX_INPUT_LENGTH){
        printf("Input is to long. Max length is %d characters\n", MAX_INPUT_LENGTH);
        exit(EXIT_FAILURE);
    }
    
    tokenizer(argv[optind], &num_tokens);
    
    struct type rpn_list[num_tokens]; 
    
    make_RPN(rpn_list, num_tokens, &tokens_rpn);


    free(tokenized_list);
    tokenized_list = NULL;

    calculate_RPN(rpn_list, tokens_rpn);

    return EXIT_SUCCESS;

}


static int use_operator(int left_value, int right_value, char operator){
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
 * Allowing spaces
 */
static void tokenizer(char *argv, int *num_tokens){
    struct type tokenized_intermediate_list[strlen(argv)];

    char *str = argv;
    char *end;
    int left_brackets = 0;
    int right_brackets = 0;

    size_t n = 0;
    // Go through the string, tokenize using 
    while(*str){
        // If its a digit it may be using several digits, so we use strtol() to extract this. 
        // isxdigit() works for both hex and not hex values, is the strtol that interprets it
        if(isxdigit((unsigned char)*str)){
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
        } // Handle case where its an inconsistant use of bracets
        else if(is_left_bracket((unsigned char)*str)){
            struct type token;
            token.type = BRACKET;
            token.caracter = (unsigned char)*str;
            tokenized_intermediate_list[n++] = token;
            str++;
            left_brackets++;
        }
        else if(is_right_bracket((unsigned char)*str)){
            struct type token;
            token.type = BRACKET;
            token.caracter = (unsigned char)*str;
            tokenized_intermediate_list[n++] = token;
            str++;
            right_brackets++;
        }
        else if(isspace((unsigned char)*str)){
            str++;
        }
        else{
            printf("Character not recognised: '%c' \n", (unsigned char)*str);
            exit(EXIT_FAILURE);
        }
    }

    if(left_brackets != right_brackets){
        printf("Inconsistent use of brackets!\n");
        exit(EXIT_FAILURE);
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
    if(base == 16){
        printf("Sum = %#x\n", output_stack[0].val);    
    }
    else if(base == 10){
        printf("Sum = %d\n", output_stack[0].val);
    }
    return 1;



};