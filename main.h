#define ADD '+'
#define SUB '-'
#define POW '^'
#define MUL '*'
#define DIV '/'
#define LEFT_BRACKET '('
#define RIGHT_BRACKET ')'

#define LEFT_ASSOCIATIVE  0
#define RIGHT_ASSOCIATIVE  1

#define ADD_ASSOCIATIVE LEFT_ASSOCIATIVE
#define MUL_ASSOCIATIVE LEFT_ASSOCIATIVE

#define ADD_PRECEDENDCE 0
#define SUB_PRECEDENCE 0
#define MUL_PRECEDENCE 1
#define DIV_PRECEDENCE 1
#define POW_PRECEDENCE 2
#define BRACKET_PRECEDENCE 3

#define MAX_INPUT_LENGTH 100


enum {
    DIGIT,
    OPERAND,
    BRACKET
};

struct type {
    char type;   // ADD, SUB etc.
    char caracter; // Used if not digit
    int val;     // Actual value
};

/* Methods used for setting up the Reverse Polish Notation*/

static int use_operator(int left_value, int right_value, char op);
static int get_priority(char op);
static int get_association(char op);

// Returns a list of 'struct types', also takes a parameter size to return how long it is.
static void tokenizer(char *argv, int *num_tokens);

// Take the tokenized list and return it as a REVERSEN POLISH NOTATION list
static void make_RPN(struct type *list, int num_tokens, int *tokens_rpn);

static int calculate_RPN(struct type *list, int num_tokens);