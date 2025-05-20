#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>





/** TODO
 * Implement a --help
 * Choice between int and float
 * How to see if legal input 
 */

#define ADD '+'
#define SUB '-'
#define POW '^'
#define MUL '*'
#define DIV '/'

#define LEFT_ASSOCIATIVE  0
#define RIGHT_ASSOCIATIVE  1

#define ADD_ASSOCIATIVE LEFT_ASSOCIATIVE
#define MUL_ASSOCIATIVE LEFT_ASSOCIATIVE

#define ADD_PRECEDENDCE 0
#define MUL_PRECEDENCE 5





// struct operand
// {   char operand_type;
//     int association;
//     int precedence;
//     /* data */
// };


// Use this;
// static struct operand operators[5] = {
//     {ADD, LEFT_ASSOCIATIVE, 0},
//     {MUL, LEFT_ASSOCIATIVE, 5}
// };

 static int use_operator(int left_value, int right_value, char operator);

 
 

 static int get_priority(char op){
    switch(op){
        case ADD: return ADD_PRECEDENDCE;
        case MUL: return MUL_PRECEDENCE;
    }
 }

 static int get_association(char op){
    switch(op){
        case ADD: return LEFT_ASSOCIATIVE;
        case MUL: return LEFT_ASSOCIATIVE;
    }
 }

static int is_operator(char op){
    return (op == ADD  \
    || op == SUB || op == POW || op == MUL);
}


void print_output(int index, char *output){
    for(int i = 0; i < index; i++){
        printf("queue: %c\n", output[i]);
    }
}

int main(int argc, char **argv){
    int result;
    char *token;
    char output[100];
    char operator_stack[100];
    int stack_index = 0;
    int output_index = 0;
    

    if (argc < 1){
        printf("No arguments\n");
        return -1;
    }

    /*
           {
            If ((token = left associative AND 
                 precedence <= stack top element) OR
            (token = right associative AND 
                 precedence < stack top element))
            {
            Pop stack onto the output queue.  
                Exit while loop.
            }
        }*/

    token = argv[1];
    char *stack_top = operator_stack;
    while(*token != '/'){
        printf("stack top: %c\n", *stack_top);
        if(isdigit((int)*token)){
            output[output_index++] = *token;
        }
        else if(is_operator(*token)){
            while((stack_index > 0) && is_operator(*stack_top)){
                if((get_association(*token) == LEFT_ASSOCIATIVE && (get_priority(*token) <= get_priority(*stack_top))) \
                    || ((get_association(*token)==RIGHT_ASSOCIATIVE) &&  get_priority(*token) < get_priority(*stack_top))
                ){
                    printf("get here \n");
                    output[output_index++] = operator_stack[--stack_index];
                    stack_top = &operator_stack[stack_index-1];
                    continue;
                }
                break;
            }
            
            operator_stack[stack_index++] = *token;
            stack_top = &operator_stack[stack_index-1];
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
   

     for(int i = 0; i < output_index; i++){
        char tok = output[i];
        printf("output queue 2 %c\n", tok);
    }

    int output_stack[100];
    int output_stack_index = 0;
    for(int i = 0; i < output_index; i++){

        char tok = output[i];
        printf("token = %c\n", tok);
        if(isdigit((unsigned char) tok)){ // Then push on output stack
            output_stack[output_stack_index] = tok - '0';
            output_stack_index++;
        }
        else if ((is_operator(tok))){ // If operand pop 2 of stack and evaluate
            int first, second, calc;
            output_stack_index--;
            second = output_stack[output_stack_index];
            output_stack_index--;
            first = output_stack[output_stack_index];

            calc = use_operator(first, second, output[i]);
            printf("calc %d\n", calc);

            output_stack[output_stack_index] = calc;
            output_stack_index++;
        }

    }


    // At this point there should only be one value on the output stack. 
    printf("Sum = %d\n", output_stack[0]);


    return 1;


}



static int use_operator(int left_value, int right_value, char operator){
    printf("Calculating.  %d %c %d\n", right_value, operator, left_value);

    switch(operator){
        case ADD: return left_value+right_value;
        case SUB: return left_value-right_value;
        case MUL: return left_value*right_value;
        case POW: return (int)pow((double) left_value, (double) right_value);
    }
    return -1;
}