#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
// Windows doesn't have getline, so we'll implement our own
typedef long long ssize_t;

ssize_t getline_impl(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) return -1;
    
    size_t pos = 0;
    int c;
    
    if (*lineptr == NULL) {
        *n = 128;
        *lineptr = malloc(*n);
        if (!*lineptr) return -1;
    }
    
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (!new_ptr) return -1;
            *lineptr = new_ptr;
            *n = new_size;
        }
        
        (*lineptr)[pos++] = c;
        
        if (c == '\n') break;
    }
    
    if (pos == 0 && c == EOF) return -1;
    
    (*lineptr)[pos] = '\0';
    return pos;
}

#define getline getline_impl
#else
#define _GNU_SOURCE
#include <unistd.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// ANSI color codes for cross-platform compatibility
#define COLOR_RESET   "\033[0m"
#define COLOR_BLUE    "\033[94m"  // Pale blue
#define COLOR_WHITE   "\033[97m"  // Bright white

// Platform-specific color enable/disable
#ifdef _WIN32
#include <windows.h>
static int colors_enabled = 0;

void enable_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
        colors_enabled = 1;
    }
}
#else
static int colors_enabled = 1; // Enable by default on Unix-like systems
void enable_colors() { colors_enabled = 1; }
#endif

// Color printing functions
void print_prompt() {
    if (colors_enabled) {
        printf(COLOR_BLUE "> " COLOR_RESET);
    } else {
        printf("> ");
    }
}

void print_normal(const char* text) {
    if (colors_enabled) {
        printf(COLOR_WHITE "%s" COLOR_RESET, text);
    } else {
        printf("%s", text);
    }
}

// Help command implementation
void show_help(void) {
    print_normal("\n=== RCalc Help ===\n\n");
    
    print_normal("BASIC USAGE:\n");
    print_normal("  > 2 + 3 * 4              # Basic arithmetic\n");
    print_normal("  > (2 + 3) * 4            # Parentheses for grouping\n");
    print_normal("  > 2^3^2                  # Exponentiation (right associative: 2^(3^2))\n\n");
    
    print_normal("VARIABLES:\n");
    print_normal("  > var x = 10;            # Declare variable\n");
    print_normal("  > x = 5;                 # Assign to existing variable\n");
    print_normal("  > y = x * 2 + 1;         # Use variables in expressions\n\n");
    
    print_normal("USER-DEFINED FUNCTIONS:\n");
    print_normal("  > var square(var n) {\n");
    print_normal("  ...   return n * n;\n");
    print_normal("  ... }\n");
    print_normal("  > square(5)              # Call user function\n\n");
    
    print_normal("BUILT-IN FUNCTIONS:\n");
    print_normal("  Trigonometric: sin, cos, tan, asin, acos, atan\n");
    print_normal("  Hyperbolic:    sinh, cosh, tanh, asinh, acosh, atanh\n");
    print_normal("  Logarithmic:   log (ln), log10, log2, exp, exp2\n");
    print_normal("  Other:         sqrt, cbrt, abs, floor, ceil, round\n");
    print_normal("  Conversions:   deg (rad→deg), rad (deg→rad)\n");
    print_normal("  Utility:       min(a,b), max(a,b), hypot(x,y)\n");
    print_normal("  Advanced:      clamp(val,min,max), lerp(a,b,t)\n");
    print_normal("  Programming:   if(condition,true_val,false_val)\n\n");
    
    print_normal("CONSTANTS:\n");
    print_normal("  pi                       # π ≈ 3.14159...\n");
    print_normal("  e                        # Euler's number ≈ 2.71828...\n\n");
    
    print_normal("MULTILINE INPUT:\n");
    print_normal("  Lines ending with {, (, +, -, *, /, ^ continue automatically\n");
    print_normal("  Use ... prompt for continuation lines\n\n");
    
    print_normal("OPERATOR PRECEDENCE (highest to lowest):\n");
    print_normal("  1. Parentheses ()        # Highest precedence\n");
    print_normal("  2. Functions sin(), sqrt(), etc.\n");
    print_normal("  3. Exponentiation ^      # Right associative\n");
    print_normal("  4. Multiplication *, Division /\n");
    print_normal("  5. Addition +, Subtraction -\n");
    print_normal("  6. Comparisons <, >, <=, >=, ==, !=  # Lowest precedence\n\n");
    
    print_normal("COMMANDS:\n");
    print_normal("  help                     # Show this help\n");
    print_normal("  load \"filename.calc\"    # Load and execute a script file\n");
    print_normal("  quit                     # Exit calculator\n\n");
    
    print_normal("SCRIPT FILES:\n");
    print_normal("  You can load script files containing function and variable definitions.\n");
    print_normal("  Use .calc or .rcalc extension. Lines starting with # are comments.\n");
    print_normal("  Command line: rcalc script.calc\n");
    print_normal("  In REPL: load \"script.calc\"\n\n");
    
    print_normal("EXAMPLES:\n");
    print_normal("  > sin(pi/2)              # = 1\n");
    print_normal("  > log(e^2)               # = 2\n");
    print_normal("  > sqrt(16) + 2^3         # = 12\n");
    print_normal("  > deg(pi/4)              # = 45 (degrees)\n");
    print_normal("  > min(5, 10)             # = 5\n");
    print_normal("  > clamp(15, 0, 10)       # = 10\n");
    print_normal("  > if(5 > 3, 100, 200)    # = 100\n");
    print_normal("  > lerp(0, 10, 0.5)       # = 5 (50% between 0 and 10)\n\n");
}

// Variable and function data structures
typedef struct Variable {
    char name[32];
    double value;
    struct Variable *next;
} Variable;

typedef struct Parameter {
    char name[32];
    struct Parameter *next;
} Parameter;

// AST Node types
typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_FUNCTION_CALL
} ASTNodeType;

// AST Node structure
typedef struct ASTNode {
    ASTNodeType type;
    union {
        double number;
        char variable[32];
        struct {
            char op;
            char comparison[3];  // For comparison operators
            struct ASTNode *left;
            struct ASTNode *right;
        } binary;
        struct {
            char op;
            struct ASTNode *operand;
        } unary;
        struct {
            char name[32];
            struct ASTNode **args;
            int arg_count;
        } func_call;
    } data;
} ASTNode;

typedef struct UserFunction {
    char name[32];
    Parameter *params;
    int param_count;
    ASTNode *body; // AST instead of string
    struct UserFunction *next;
} UserFunction;

// Global symbol tables
static Variable *variables = NULL;
static UserFunction *user_functions = NULL;
static int silent_mode = 0;  // For suppressing output during script loading

// Token types for parsing
typedef enum {
    CALC_TOKEN_NUMBER,
    CALC_TOKEN_OPERATOR,
    CALC_TOKEN_COMPARISON,    // <, >, <=, >=, ==, !=
    CALC_TOKEN_FUNCTION,
    CALC_TOKEN_LPAREN,
    CALC_TOKEN_RPAREN,
    CALC_TOKEN_CONSTANT,
    CALC_TOKEN_IDENTIFIER,
    CALC_TOKEN_ASSIGN,        // =
    CALC_TOKEN_SEMICOLON,     // ;
    CALC_TOKEN_COMMA,         // ,
    CALC_TOKEN_LBRACE,        // {
    CALC_TOKEN_RBRACE,        // }
    CALC_TOKEN_VAR,           // var keyword
    CALC_TOKEN_RETURN,        // return keyword
    CALC_TOKEN_LOAD,          // load keyword
    CALC_TOKEN_END
} CalcTokenType;

typedef struct {
    CalcTokenType type;
    double value;
    char op;
    char comparison[3];  // For comparison operators like "<=", ">="
    char name[32];
} Token;

// Global variables for parsing
static const char *expr_pos;
static Token current_token;

// Function prototypes
static void get_next_token(void);
static double parse_expression(void);
static double parse_term(void);
static double parse_factor(void);
static double parse_function(const char *func_name);
static double parse_power(void);
static double parse_statement(void);
static double parse_assignment(void);
static void parse_function_definition(void);
static int is_function(const char *name);
static void skip_whitespace(void);
static Variable* lookup_variable(const char *name);
static UserFunction* lookup_user_function(const char *name);
static void create_variable(const char *name, double value);
static void set_variable_value(const char *name, double value);
static double get_variable_value(const char *name);
static void free_variables(Variable *vars);
static void free_user_functions(void);
static Parameter* create_parameter(const char *name);
static void free_parameters(Parameter *params);
static UserFunction* create_user_function(const char *name, Parameter *params, ASTNode *body);
static double evaluate_user_function(UserFunction *func, double *args, int arg_count);
static int load_script_file(const char *filename);
static void parse_load_command(const char *line);

// AST functions
static ASTNode* create_number_node(double value);
static ASTNode* create_variable_node(const char *name);
static ASTNode* create_binary_op_node(char op, ASTNode *left, ASTNode *right);
static ASTNode* create_unary_op_node(char op, ASTNode *operand);
static ASTNode* create_function_call_node(const char *name, ASTNode **args, int arg_count);
static ASTNode* parse_expression_ast(void);
static ASTNode* parse_comparison_ast(void);
static ASTNode* parse_term_addition_ast(void);
static ASTNode* parse_term_ast(void);
static ASTNode* parse_factor_ast(void);
static ASTNode* parse_power_ast(void);
static double evaluate_ast(ASTNode *node);
static void free_ast(ASTNode *node);

// AST creation functions
static ASTNode* create_number_node(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->data.number = value;
    return node;
}

static ASTNode* create_variable_node(const char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    strcpy(node->data.variable, name);
    return node;
}

static ASTNode* create_binary_op_node(char op, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->data.binary.op = op;
    node->data.binary.comparison[0] = '\0'; // Initialize to empty string
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

static ASTNode* create_comparison_node(const char *comparison, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->data.binary.op = comparison[0]; // Store first character for compatibility
    strcpy(node->data.binary.comparison, comparison);
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

static ASTNode* create_unary_op_node(char op, ASTNode *operand) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_UNARY_OP;
    node->data.unary.op = op;
    node->data.unary.operand = operand;
    return node;
}

static ASTNode* create_function_call_node(const char *name, ASTNode **args, int arg_count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FUNCTION_CALL;
    strcpy(node->data.func_call.name, name);
    node->data.func_call.args = args;
    node->data.func_call.arg_count = arg_count;
    return node;
}

// AST evaluation
static double evaluate_ast(ASTNode *node) {
    if (!node) return NAN;
    
    switch (node->type) {
        case AST_NUMBER:
            return node->data.number;
            
        case AST_VARIABLE:
            return get_variable_value(node->data.variable);
            
        case AST_BINARY_OP: {
            double left = evaluate_ast(node->data.binary.left);
            double right = evaluate_ast(node->data.binary.right);
            
            // Check if it's a comparison operation
            if (node->data.binary.comparison[0] != '\0') {
                if (strcmp(node->data.binary.comparison, "<") == 0) return left < right ? 1.0 : 0.0;
                if (strcmp(node->data.binary.comparison, ">") == 0) return left > right ? 1.0 : 0.0;
                if (strcmp(node->data.binary.comparison, "<=") == 0) return left <= right ? 1.0 : 0.0;
                if (strcmp(node->data.binary.comparison, ">=") == 0) return left >= right ? 1.0 : 0.0;
                if (strcmp(node->data.binary.comparison, "==") == 0) return fabs(left - right) < 1e-10 ? 1.0 : 0.0;
                if (strcmp(node->data.binary.comparison, "!=") == 0) return fabs(left - right) >= 1e-10 ? 1.0 : 0.0;
            }
            
            // Regular arithmetic operations
            switch (node->data.binary.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': 
                    if (right == 0.0) {
                        fprintf(stderr, "Error: Division by zero\n");
                        return NAN;
                    }
                    return left / right;
                case '^': return pow(left, right);
                default: return NAN;
            }
        }
        
        case AST_UNARY_OP: {
            double operand = evaluate_ast(node->data.unary.operand);
            switch (node->data.unary.op) {
                case '+': return operand;
                case '-': return -operand;
                default: return NAN;
            }
        }
        
        case AST_FUNCTION_CALL: {
            // Handle built-in functions
            if (is_function(node->data.func_call.name)) {
                // Multi-argument built-in functions
                if (strcmp(node->data.func_call.name, "pow") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: pow() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return pow(arg1, arg2);
                }
                
                if (strcmp(node->data.func_call.name, "atan2") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: atan2() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return atan2(arg1, arg2);
                }
                
                if (strcmp(node->data.func_call.name, "fmod") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: fmod() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return fmod(arg1, arg2);
                }
                
                if (strcmp(node->data.func_call.name, "min") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: min() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return (arg1 < arg2) ? arg1 : arg2;
                }
                
                if (strcmp(node->data.func_call.name, "max") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: max() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return (arg1 > arg2) ? arg1 : arg2;
                }
                
                if (strcmp(node->data.func_call.name, "hypot") == 0) {
                    if (node->data.func_call.arg_count != 2) {
                        fprintf(stderr, "Error: hypot() requires 2 arguments\n");
                        return NAN;
                    }
                    double arg1 = evaluate_ast(node->data.func_call.args[0]);
                    double arg2 = evaluate_ast(node->data.func_call.args[1]);
                    return hypot(arg1, arg2);
                }
                
                // Three-argument functions
                if (strcmp(node->data.func_call.name, "clamp") == 0) {
                    if (node->data.func_call.arg_count != 3) {
                        fprintf(stderr, "Error: clamp() requires 3 arguments (value, min, max)\n");
                        return NAN;
                    }
                    double value = evaluate_ast(node->data.func_call.args[0]);
                    double min_val = evaluate_ast(node->data.func_call.args[1]);
                    double max_val = evaluate_ast(node->data.func_call.args[2]);
                    if (value < min_val) return min_val;
                    if (value > max_val) return max_val;
                    return value;
                }
                
                if (strcmp(node->data.func_call.name, "lerp") == 0) {
                    if (node->data.func_call.arg_count != 3) {
                        fprintf(stderr, "Error: lerp() requires 3 arguments (a, b, t)\n");
                        return NAN;
                    }
                    double a = evaluate_ast(node->data.func_call.args[0]);
                    double b = evaluate_ast(node->data.func_call.args[1]);
                    double t = evaluate_ast(node->data.func_call.args[2]);
                    return a + t * (b - a);
                }
                
                if (strcmp(node->data.func_call.name, "if") == 0) {
                    if (node->data.func_call.arg_count != 3) {
                        fprintf(stderr, "Error: if() requires 3 arguments (condition, true_value, false_value)\n");
                        return NAN;
                    }
                    double condition = evaluate_ast(node->data.func_call.args[0]);
                    if (condition != 0.0) {
                        return evaluate_ast(node->data.func_call.args[1]);
                    } else {
                        return evaluate_ast(node->data.func_call.args[2]);
                    }
                }
                
                // Single-argument built-in functions
                if (node->data.func_call.arg_count != 1) {
                    fprintf(stderr, "Error: Built-in function '%s' expects 1 argument\n", 
                            node->data.func_call.name);
                    return NAN;
                }
                double arg = evaluate_ast(node->data.func_call.args[0]);
                
                if (strcmp(node->data.func_call.name, "sin") == 0) return sin(arg);
                if (strcmp(node->data.func_call.name, "cos") == 0) return cos(arg);
                if (strcmp(node->data.func_call.name, "tan") == 0) return tan(arg);
                if (strcmp(node->data.func_call.name, "sqrt") == 0) return sqrt(arg);
                if (strcmp(node->data.func_call.name, "log") == 0) return log(arg);
                if (strcmp(node->data.func_call.name, "ln") == 0) return log(arg);
                if (strcmp(node->data.func_call.name, "exp") == 0) return exp(arg);
                if (strcmp(node->data.func_call.name, "abs") == 0) return fabs(arg);
                if (strcmp(node->data.func_call.name, "fabs") == 0) return fabs(arg);
                if (strcmp(node->data.func_call.name, "floor") == 0) return floor(arg);
                if (strcmp(node->data.func_call.name, "ceil") == 0) return ceil(arg);
                if (strcmp(node->data.func_call.name, "round") == 0) return round(arg);
                if (strcmp(node->data.func_call.name, "sqrt") == 0) return sqrt(arg);
                if (strcmp(node->data.func_call.name, "cbrt") == 0) return cbrt(arg);
                if (strcmp(node->data.func_call.name, "deg") == 0) return arg * 180.0 / M_PI;
                if (strcmp(node->data.func_call.name, "rad") == 0) return arg * M_PI / 180.0;
                // Add other single-argument functions as needed
            }
            
            // Handle user-defined functions
            UserFunction *func = lookup_user_function(node->data.func_call.name);
            if (func) {
                if (node->data.func_call.arg_count != func->param_count) {
                    fprintf(stderr, "Error: Function '%s' expects %d arguments, got %d\n",
                            node->data.func_call.name, func->param_count, node->data.func_call.arg_count);
                    return NAN;
                }
                
                double args[10]; // Max 10 args
                for (int i = 0; i < node->data.func_call.arg_count; i++) {
                    args[i] = evaluate_ast(node->data.func_call.args[i]);
                }
                
                return evaluate_user_function(func, args, node->data.func_call.arg_count);
            }
            
            fprintf(stderr, "Error: Unknown function '%s'\n", node->data.func_call.name);
            return NAN;
        }
        
        default:
            return NAN;
    }
}

static void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_BINARY_OP:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case AST_UNARY_OP:
            free_ast(node->data.unary.operand);
            break;
        case AST_FUNCTION_CALL:
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                free_ast(node->data.func_call.args[i]);
            }
            free(node->data.func_call.args);
            break;
        default:
            break;
    }
    free(node);
}

// AST parsing functions
static ASTNode* parse_factor_ast(void) {
    if (current_token.type == CALC_TOKEN_NUMBER) {
        double value = current_token.value;
        get_next_token();
        return create_number_node(value);
    }
    
    if (current_token.type == CALC_TOKEN_CONSTANT) {
        double value = current_token.value;
        get_next_token();
        return create_number_node(value);
    }
    
    if (current_token.type == CALC_TOKEN_IDENTIFIER || current_token.type == CALC_TOKEN_FUNCTION) {
        char name[32];
        strcpy(name, current_token.name);
        get_next_token();
        
        if (current_token.type == CALC_TOKEN_LPAREN) {
            // Function call
            get_next_token(); // consume '('
            
            ASTNode **args = NULL;
            int arg_count = 0;
            
            if (current_token.type != CALC_TOKEN_RPAREN) {
                args = malloc(10 * sizeof(ASTNode*)); // Max 10 args
                while (1) {
                    if (arg_count >= 10) {
                        fprintf(stderr, "Error: Too many arguments\n");
                        return NULL;
                    }
                    ASTNode *arg_node = parse_expression_ast();
                    if (!arg_node) {
                        fprintf(stderr, "Error: Failed to parse argument %d in function '%s'\n", arg_count + 1, name);
                        if (args) {
                            for (int i = 0; i < arg_count; i++) {
                                free_ast(args[i]);
                            }
                            free(args);
                        }
                        return NULL;
                    }
                    args[arg_count++] = arg_node;
                    
                    if (current_token.type == CALC_TOKEN_COMMA) {
                        get_next_token(); // consume comma and continue
                    } else {
                        break; // no more arguments
                    }
                }
            }
            
            if (current_token.type != CALC_TOKEN_RPAREN) {
                fprintf(stderr, "Error: Expected ')' in function call '%s'\n", name);
                if (args) {
                    for (int i = 0; i < arg_count; i++) {
                        free_ast(args[i]);
                    }
                    free(args);
                }
                return NULL;
            }
            get_next_token(); // consume ')'
            
            return create_function_call_node(name, args, arg_count);
        } else {
            // Variable
            return create_variable_node(name);
        }
    }
    
    if (current_token.type == CALC_TOKEN_LPAREN) {
        get_next_token(); // consume '('
        ASTNode *node = parse_expression_ast();
        if (current_token.type != CALC_TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            free_ast(node);
            return NULL;
        }
        get_next_token(); // consume ')'
        return node;
    }
    
    if (current_token.type == CALC_TOKEN_OPERATOR && (current_token.op == '-' || current_token.op == '+')) {
        char op = current_token.op;
        get_next_token();
        ASTNode *operand = parse_factor_ast();
        return create_unary_op_node(op, operand);
    }
    
    fprintf(stderr, "Error: Unexpected token in AST parsing\n");
    return NULL;
}

static ASTNode* parse_power_ast(void) {
    ASTNode *left = parse_factor_ast();
    
    if (current_token.type == CALC_TOKEN_OPERATOR && current_token.op == '^') {
        get_next_token();
        ASTNode *right = parse_power_ast(); // Right associative
        return create_binary_op_node('^', left, right);
    }
    
    return left;
}

static ASTNode* parse_term_ast(void) {
    ASTNode *left = parse_power_ast();
    
    while (current_token.type == CALC_TOKEN_OPERATOR && 
           (current_token.op == '*' || current_token.op == '/')) {
        char op = current_token.op;
        get_next_token();
        ASTNode *right = parse_power_ast();
        left = create_binary_op_node(op, left, right);
    }
    
    return left;
}

static ASTNode* parse_expression_ast(void) {
    return parse_comparison_ast();
}

static ASTNode* parse_comparison_ast(void) {
    ASTNode *left = parse_term_addition_ast();
    
    while (current_token.type == CALC_TOKEN_COMPARISON) {
        char comparison[3];
        strcpy(comparison, current_token.comparison);
        get_next_token();
        ASTNode *right = parse_term_addition_ast();
        
        left = create_comparison_node(comparison, left, right);
    }
    
    return left;
}

static ASTNode* parse_term_addition_ast(void) {
    ASTNode *left = parse_term_ast();
    
    while (current_token.type == CALC_TOKEN_OPERATOR && 
           (current_token.op == '+' || current_token.op == '-')) {
        char op = current_token.op;
        get_next_token();
        ASTNode *right = parse_term_ast();
        left = create_binary_op_node(op, left, right);
    }
    
    return left;
}

// Variable management functions
static Variable* lookup_variable(const char *name) {
    Variable *var = variables;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            return var;
        }
        var = var->next;
    }
    return NULL;
}

static void create_variable(const char *name, double value) {
    Variable *var = malloc(sizeof(Variable));
    strcpy(var->name, name);
    var->value = value;
    var->next = variables;
    variables = var;
}

static void set_variable_value(const char *name, double value) {
    Variable *var = lookup_variable(name);
    if (var) {
        var->value = value;
    } else {
        create_variable(name, value);
    }
}

static double get_variable_value(const char *name) {
    Variable *var = lookup_variable(name);
    if (var) return var->value;
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    return NAN;
}

static void free_variables(Variable *vars) {
    while (vars) {
        Variable *next = vars->next;
        free(vars);
        vars = next;
    }
}

// User function management
static UserFunction* lookup_user_function(const char *name) {
    UserFunction *func = user_functions;
    while (func) {
        if (strcmp(func->name, name) == 0) {
            return func;
        }
        func = func->next;
    }
    return NULL;
}

static Parameter* create_parameter(const char *name) {
    Parameter *param = malloc(sizeof(Parameter));
    strcpy(param->name, name);
    param->next = NULL;
    return param;
}

static void free_parameters(Parameter *params) {
    while (params) {
        Parameter *next = params->next;
        free(params);
        params = next;
    }
}

static UserFunction* create_user_function(const char *name, Parameter *params, ASTNode *body) {
    // Remove existing function with same name
    UserFunction **current = &user_functions;
    while (*current) {
        if (strcmp((*current)->name, name) == 0) {
            UserFunction *to_remove = *current;
            *current = (*current)->next;
            free_ast(to_remove->body);
            free_parameters(to_remove->params);
            free(to_remove);
            break;
        }
        current = &(*current)->next;
    }
    
    // Create new function
    UserFunction *func = malloc(sizeof(UserFunction));
    strcpy(func->name, name);
    func->params = params;
    func->body = body;
    func->next = user_functions;
    
    // Count parameters
    func->param_count = 0;
    Parameter *p = params;
    while (p) {
        func->param_count++;
        p = p->next;
    }
    
    user_functions = func;
    return func;
}

static void free_user_functions(void) {
    while (user_functions) {
        UserFunction *next = user_functions->next;
        free_ast(user_functions->body);
        free_parameters(user_functions->params);
        free(user_functions);
        user_functions = next;
    }
}

// Skip whitespace characters
static void skip_whitespace(void) {
    while (*expr_pos && isspace(*expr_pos)) {
        expr_pos++;
    }
}

// Check if a string is a known function
static int is_function(const char *name) {
    const char *functions[] = {
        "sin", "cos", "tan", "asin", "acos", "atan",
        "sinh", "cosh", "tanh", "asinh", "acosh", "atanh",
        "log", "ln", "log10", "log2", "exp", "exp2", "exp10",
        "sqrt", "cbrt", "abs", "floor", "ceil", "round",
        "deg", "rad", "pow", "fmod", "fabs", "atan2",
        "min", "max", "clamp", "lerp", "hypot", "if", NULL
    };
    
    for (int i = 0; functions[i]; i++) {
        if (strcmp(name, functions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Get the next token from the expression
static void get_next_token(void) {
    skip_whitespace();
    
    if (*expr_pos == '\0') {
        current_token.type = CALC_TOKEN_END;
        return;
    }
    
    // Numbers (including decimals)
    if (isdigit(*expr_pos) || *expr_pos == '.') {
        char *endptr;
        current_token.value = strtod(expr_pos, &endptr);
        current_token.type = CALC_TOKEN_NUMBER;
        expr_pos = endptr;
        return;
    }
    
    // Comparison operators (must come before assignment)
    if (*expr_pos == '<') {
        if (*(expr_pos + 1) == '=') {
            current_token.type = CALC_TOKEN_COMPARISON;
            strcpy(current_token.comparison, "<=");
            expr_pos += 2;
            return;
        } else {
            current_token.type = CALC_TOKEN_COMPARISON;
            strcpy(current_token.comparison, "<");
            expr_pos++;
            return;
        }
    }
    
    if (*expr_pos == '>') {
        if (*(expr_pos + 1) == '=') {
            current_token.type = CALC_TOKEN_COMPARISON;
            strcpy(current_token.comparison, ">=");
            expr_pos += 2;
            return;
        } else {
            current_token.type = CALC_TOKEN_COMPARISON;
            strcpy(current_token.comparison, ">");
            expr_pos++;
            return;
        }
    }
    
    if (*expr_pos == '=' && *(expr_pos + 1) == '=') {
        current_token.type = CALC_TOKEN_COMPARISON;
        strcpy(current_token.comparison, "==");
        expr_pos += 2;
        return;
    }
    
    if (*expr_pos == '!' && *(expr_pos + 1) == '=') {
        current_token.type = CALC_TOKEN_COMPARISON;
        strcpy(current_token.comparison, "!=");
        expr_pos += 2;
        return;
    }
    
    // Assignment operator
    if (*expr_pos == '=') {
        current_token.type = CALC_TOKEN_ASSIGN;
        expr_pos++;
        return;
    }
    
    // Semicolon
    if (*expr_pos == ';') {
        current_token.type = CALC_TOKEN_SEMICOLON;
        expr_pos++;
        return;
    }
    
    // Comma
    if (*expr_pos == ',') {
        current_token.type = CALC_TOKEN_COMMA;
        expr_pos++;
        return;
    }
    
    // Braces
    if (*expr_pos == '{') {
        current_token.type = CALC_TOKEN_LBRACE;
        expr_pos++;
        return;
    }
    
    if (*expr_pos == '}') {
        current_token.type = CALC_TOKEN_RBRACE;
        expr_pos++;
        return;
    }
    
    // Operators
    if (strchr("+-*/^", *expr_pos)) {
        current_token.type = CALC_TOKEN_OPERATOR;
        current_token.op = *expr_pos;
        expr_pos++;
        return;
    }
    
    // Parentheses
    if (*expr_pos == '(') {
        current_token.type = CALC_TOKEN_LPAREN;
        expr_pos++;
        return;
    }
    
    if (*expr_pos == ')') {
        current_token.type = CALC_TOKEN_RPAREN;
        expr_pos++;
        return;
    }
    
    // Functions, keywords, constants, and identifiers
    if (isalpha(*expr_pos) || *expr_pos == '_') {
        int i = 0;
        while ((isalnum(*expr_pos) || *expr_pos == '_') && i < 31) {
            current_token.name[i++] = *expr_pos++;
        }
        current_token.name[i] = '\0';
        
        // Check for keywords
        if (strcmp(current_token.name, "var") == 0) {
            current_token.type = CALC_TOKEN_VAR;
        } else if (strcmp(current_token.name, "return") == 0) {
            current_token.type = CALC_TOKEN_RETURN;
        } else if (strcmp(current_token.name, "load") == 0) {
            current_token.type = CALC_TOKEN_LOAD;
        } else if (strcmp(current_token.name, "pi") == 0) {
            current_token.type = CALC_TOKEN_CONSTANT;
            current_token.value = M_PI;
        } else if (strcmp(current_token.name, "e") == 0) {
            current_token.type = CALC_TOKEN_CONSTANT;
            current_token.value = M_E;
        } else if (is_function(current_token.name)) {
            current_token.type = CALC_TOKEN_FUNCTION;
        } else if (lookup_user_function(current_token.name)) {
            current_token.type = CALC_TOKEN_FUNCTION;
        } else {
            current_token.type = CALC_TOKEN_IDENTIFIER;
        }
        return;
    }
    
    // Unknown character
    fprintf(stderr, "Error: Unknown character '%c'\n", *expr_pos);
    current_token.type = CALC_TOKEN_END;
}

// Parse function calls (both built-in and user-defined)
static double parse_function(const char *func_name) {
    if (current_token.type != CALC_TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after function '%s'\n", func_name);
        return NAN;
    }
    
    get_next_token(); // consume '('
    
    // Check if it's a user-defined function
    UserFunction *user_func = lookup_user_function(func_name);
    if (user_func) {
        double args[10]; // Maximum 10 parameters
        int arg_count = 0;
        
        // Parse arguments
        if (current_token.type != CALC_TOKEN_RPAREN) {
            do {
                if (arg_count >= 10) {
                    fprintf(stderr, "Error: Too many arguments (max 10)\n");
                    return NAN;
                }
                args[arg_count++] = parse_expression();
                if (current_token.type == CALC_TOKEN_COMMA) {
                    get_next_token(); // consume ','
                }
            } while (current_token.type == CALC_TOKEN_COMMA);
        }
        
        if (current_token.type != CALC_TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')' after function arguments\n");
            return NAN;
        }
        
        get_next_token(); // consume ')'
        
        if (arg_count != user_func->param_count) {
            fprintf(stderr, "Error: Function '%s' expects %d arguments, got %d\n", 
                    func_name, user_func->param_count, arg_count);
            return NAN;
        }
        
        return evaluate_user_function(user_func, args, arg_count);
    }
    
    // Built-in function - parse arguments
    double args[10]; // Maximum 10 arguments
    int arg_count = 0;
    
    if (current_token.type != CALC_TOKEN_RPAREN) {
        do {
            if (arg_count >= 10) {
                fprintf(stderr, "Error: Too many arguments (max 10)\n");
                return NAN;
            }
            args[arg_count++] = parse_expression();
            if (current_token.type == CALC_TOKEN_COMMA) {
                get_next_token(); // consume ','
            }
        } while (current_token.type == CALC_TOKEN_COMMA);
    }
    
    if (current_token.type != CALC_TOKEN_RPAREN) {
        fprintf(stderr, "Error: Expected ')' after function arguments\n");
        return NAN;
    }
    
    get_next_token(); // consume ')'
    
    // Apply the built-in function
    if (strcmp(func_name, "pow") == 0) {
        if (arg_count != 2) {
            fprintf(stderr, "Error: pow() requires 2 arguments\n");
            return NAN;
        }
        return pow(args[0], args[1]);
    }
    
    if (strcmp(func_name, "atan2") == 0) {
        if (arg_count != 2) {
            fprintf(stderr, "Error: atan2() requires 2 arguments\n");
            return NAN;
        }
        return atan2(args[0], args[1]);
    }
    
    if (strcmp(func_name, "fmod") == 0) {
        if (arg_count != 2) {
            fprintf(stderr, "Error: fmod() requires 2 arguments\n");
            return NAN;
        }
        return fmod(args[0], args[1]);
    }
    
    // Single argument functions
    if (arg_count != 1) {
        fprintf(stderr, "Error: Function '%s' requires 1 argument\n", func_name);
        return NAN;
    }
    
    double arg = args[0];
    
    if (strcmp(func_name, "sin") == 0) return sin(arg);
    if (strcmp(func_name, "cos") == 0) return cos(arg);
    if (strcmp(func_name, "tan") == 0) return tan(arg);
    if (strcmp(func_name, "asin") == 0) return asin(arg);
    if (strcmp(func_name, "acos") == 0) return acos(arg);
    if (strcmp(func_name, "atan") == 0) return atan(arg);
    if (strcmp(func_name, "sinh") == 0) return sinh(arg);
    if (strcmp(func_name, "cosh") == 0) return cosh(arg);
    if (strcmp(func_name, "tanh") == 0) return tanh(arg);
    if (strcmp(func_name, "asinh") == 0) return asinh(arg);
    if (strcmp(func_name, "acosh") == 0) return acosh(arg);
    if (strcmp(func_name, "atanh") == 0) return atanh(arg);
    if (strcmp(func_name, "log") == 0) return log(arg);
    if (strcmp(func_name, "ln") == 0) return log(arg);
    if (strcmp(func_name, "log10") == 0) return log10(arg);
    if (strcmp(func_name, "log2") == 0) return log2(arg);
    if (strcmp(func_name, "exp") == 0) return exp(arg);
    if (strcmp(func_name, "exp2") == 0) return exp2(arg);
    if (strcmp(func_name, "exp10") == 0) return pow(10, arg);
    if (strcmp(func_name, "sqrt") == 0) return sqrt(arg);
    if (strcmp(func_name, "cbrt") == 0) return cbrt(arg);
    if (strcmp(func_name, "abs") == 0) return fabs(arg);
    if (strcmp(func_name, "fabs") == 0) return fabs(arg);
    if (strcmp(func_name, "floor") == 0) return floor(arg);
    if (strcmp(func_name, "ceil") == 0) return ceil(arg);
    if (strcmp(func_name, "round") == 0) return round(arg);
    if (strcmp(func_name, "deg") == 0) return arg * 180.0 / M_PI;
    if (strcmp(func_name, "rad") == 0) return arg * M_PI / 180.0;
    
    fprintf(stderr, "Error: Unknown function '%s'\n", func_name);
    return NAN;
}

// Evaluate user-defined function with local scope
static double evaluate_user_function(UserFunction *func, double *args, int arg_count) {
    // Create local scope by saving current variables
    Variable *saved_vars = variables;
    variables = NULL;
    
    // Bind parameters to arguments
    Parameter *param = func->params;
    for (int i = 0; i < arg_count && param; i++, param = param->next) {
        create_variable(param->name, args[i]);
    }
    
    // Evaluate the AST body
    double result = evaluate_ast(func->body);
    
    // Restore global scope
    free_variables(variables);
    variables = saved_vars;
    
    return result;
}

// Parse function definition
static void parse_function_definition(void) {
    // Already consumed 'var'
    if (current_token.type != CALC_TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected function name\n");
        return;
    }
    
    char func_name[32];
    strcpy(func_name, current_token.name);
    get_next_token(); // consume function name
    
    if (current_token.type != CALC_TOKEN_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after function name\n");
        return;
    }
    get_next_token(); // consume '('
    
    // Parse parameter list
    Parameter *params = NULL;
    Parameter *last_param = NULL;
    
    while (current_token.type != CALC_TOKEN_RPAREN) {
        if (current_token.type != CALC_TOKEN_VAR) {
            fprintf(stderr, "Error: Expected parameter type 'var'\n");
            return;
        }
        get_next_token(); // consume 'var'
        
        if (current_token.type != CALC_TOKEN_IDENTIFIER) {
            fprintf(stderr, "Error: Expected parameter name\n");
            return;
        }
        
        Parameter *param = create_parameter(current_token.name);
        if (params == NULL) {
            params = param;
            last_param = param;
        } else {
            last_param->next = param;
            last_param = param;
        }
        
        get_next_token(); // consume parameter name
        
        if (current_token.type == CALC_TOKEN_COMMA) {
            get_next_token(); // consume ','
        }
    }
    
    get_next_token(); // consume ')'
    
    if (current_token.type != CALC_TOKEN_LBRACE) {
        fprintf(stderr, "Error: Expected '{' to start function body\n");
        return;
    }
    get_next_token(); // consume '{'
    
    // Parse the function body as AST
    if (current_token.type != CALC_TOKEN_RETURN) {
        fprintf(stderr, "Error: Expected 'return' statement in function body\n");
        free_parameters(params);
        return;
    }
    get_next_token(); // consume 'return'
    
    // Parse the return expression as AST
    ASTNode *body = parse_expression_ast();
    if (!body) {
        fprintf(stderr, "Error: Failed to parse return expression\n");
        fprintf(stderr, "Debug: Current token type: %d\n", current_token.type);
        if (current_token.type == CALC_TOKEN_IDENTIFIER || current_token.type == CALC_TOKEN_FUNCTION) {
            fprintf(stderr, "Debug: Current token name: '%s'\n", current_token.name);
        }
        free_parameters(params);
        return;
    }
    
    if (current_token.type != CALC_TOKEN_SEMICOLON) {
        fprintf(stderr, "Error: Expected ';' after return expression\n");
        free_parameters(params);
        free_ast(body);
        return;
    }
    get_next_token(); // consume ';'
    
    if (current_token.type != CALC_TOKEN_RBRACE) {
        fprintf(stderr, "Error: Expected '}' to end function body\n");
        free_parameters(params);
        free_ast(body);
        return;
    }
    get_next_token(); // consume '}'
    
    // Create the function with AST body
    create_user_function(func_name, params, body);
    
    if (!silent_mode) {
        printf("Function '%s' defined\n", func_name);
    }
}

// Parse factors (numbers, constants, functions, parentheses, variables)
static double parse_factor(void) {
    double result;
    
    if (current_token.type == CALC_TOKEN_NUMBER) {
        result = current_token.value;
        get_next_token();
        return result;
    }
    
    if (current_token.type == CALC_TOKEN_CONSTANT) {
        result = current_token.value;
        get_next_token();
        return result;
    }
    
    if (current_token.type == CALC_TOKEN_FUNCTION) {
        char func_name[32];
        strcpy(func_name, current_token.name);
        get_next_token();
        return parse_function(func_name);
    }
    
    if (current_token.type == CALC_TOKEN_IDENTIFIER) {
        char name[32];
        strcpy(name, current_token.name);
        
        // Look ahead to see if this is a function call
        const char *saved_pos = expr_pos;
        Token saved_token = current_token;
        get_next_token();
        
        if (current_token.type == CALC_TOKEN_LPAREN && lookup_user_function(name)) {
            // It's a user-defined function call
            expr_pos = saved_pos;
            current_token = saved_token;
            get_next_token(); // consume function name
            return parse_function(name);
        } else {
            // It's a variable, restore state
            expr_pos = saved_pos;
            current_token = saved_token;
            get_next_token(); // consume variable name
            return get_variable_value(name);
        }
    }
    
    if (current_token.type == CALC_TOKEN_LPAREN) {
        get_next_token(); // consume '('
        result = parse_expression();
        if (current_token.type != CALC_TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            return NAN;
        }
        get_next_token(); // consume ')'
        return result;
    }
    
    if (current_token.type == CALC_TOKEN_OPERATOR && current_token.op == '-') {
        get_next_token(); // consume unary '-'
        return -parse_factor();
    }
    
    if (current_token.type == CALC_TOKEN_OPERATOR && current_token.op == '+') {
        get_next_token(); // consume unary '+'
        return parse_factor();
    }
    
    fprintf(stderr, "Error: Unexpected token\n");
    return NAN;
}

// Parse assignment or variable declaration
static double parse_assignment(void) {
    char var_name[32];
    strcpy(var_name, current_token.name);
    get_next_token(); // consume variable name
    
    if (current_token.type != CALC_TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' in assignment\n");
        return NAN;
    }
    get_next_token(); // consume '='
    
    double value = parse_expression();
    set_variable_value(var_name, value);
    
    return value;
}

// Parse statements (assignments, declarations, expressions)
static double parse_statement(void) {
    if (current_token.type == CALC_TOKEN_VAR) {
        get_next_token(); // consume 'var'
        
        if (current_token.type == CALC_TOKEN_IDENTIFIER) {
            // Check if this is a function definition
            const char *saved_pos = expr_pos;
            Token saved_token = current_token;
            
            char name[32];
            strcpy(name, current_token.name);
            get_next_token();
            
            if (current_token.type == CALC_TOKEN_LPAREN) {
                // It's a function definition
                expr_pos = saved_pos;
                current_token = saved_token;
                parse_function_definition();
                return NAN; // Functions don't return values to REPL
            } else {
                // It's a variable declaration
                expr_pos = saved_pos;
                current_token = saved_token;
                
                if (current_token.type != CALC_TOKEN_IDENTIFIER) {
                    fprintf(stderr, "Error: Expected variable name\n");
                    return NAN;
                }
                
                double value = parse_assignment();
                if (!silent_mode) {
                    printf("Variable '%s' = %.10g\n", name, value);
                }
                return value;
            }
        }
    }
    
    if (current_token.type == CALC_TOKEN_IDENTIFIER) {
        // Could be assignment, function call, or expression
        const char *saved_pos = expr_pos;
        Token saved_token = current_token;
        
        char name[32];
        strcpy(name, current_token.name);
        get_next_token();
        
        if (current_token.type == CALC_TOKEN_ASSIGN) {
            // It's an assignment
            expr_pos = saved_pos;
            current_token = saved_token;
            double value = parse_assignment();
            if (!silent_mode) {
                printf("Variable '%s' = %.10g\n", name, value);
            }
            return value;
        } else if (current_token.type == CALC_TOKEN_LPAREN && lookup_user_function(name)) {
            // It's a user-defined function call, restore and parse with AST
            expr_pos = saved_pos;
            current_token = saved_token;
            ASTNode *ast = parse_expression_ast();
            if (!ast) {
                fprintf(stderr, "Error: Failed to parse expression\n");
                return NAN;
            }
            double result = evaluate_ast(ast);
            free_ast(ast);
            return result;
        } else {
            // It's an expression, restore state and parse with AST
            expr_pos = saved_pos;
            current_token = saved_token;
            ASTNode *ast = parse_expression_ast();
            if (!ast) {
                fprintf(stderr, "Error: Failed to parse expression\n");
                return NAN;
            }
            double result = evaluate_ast(ast);
            free_ast(ast);
            return result;
        }
    }
    
    // Regular expression - use AST parser
    ASTNode *ast = parse_expression_ast();
    if (!ast) {
        fprintf(stderr, "Error: Failed to parse expression\n");
        return NAN;
    }
    double result = evaluate_ast(ast);
    free_ast(ast);
    return result;
}

// Parse power operations (right associative)
static double parse_power(void) {
    double result = parse_factor();
    
    if (current_token.type == CALC_TOKEN_OPERATOR && current_token.op == '^') {
        get_next_token(); // consume '^'
        double exponent = parse_power(); // right associative
        result = pow(result, exponent);
    }
    
    return result;
}

// Parse terms (multiplication and division)
static double parse_term(void) {
    double result = parse_power();
    
    while (current_token.type == CALC_TOKEN_OPERATOR && 
           (current_token.op == '*' || current_token.op == '/')) {
        char op = current_token.op;
        get_next_token();
        double right = parse_power();
        
        if (op == '*') {
            result *= right;
        } else {
            if (right == 0.0) {
                fprintf(stderr, "Error: Division by zero\n");
                return NAN;
            }
            result /= right;
        }
    }
    
    return result;
}

// Parse expressions (addition and subtraction)
static double parse_expression(void) {
    double result = parse_term();
    
    while (current_token.type == CALC_TOKEN_OPERATOR && 
           (current_token.op == '+' || current_token.op == '-')) {
        char op = current_token.op;
        get_next_token();
        double right = parse_term();
        
        if (op == '+') {
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

double compute_expression(const char *expression)
{
    if(expression == NULL || strlen(expression) == 0) 
    {
        fprintf(stderr, "Error: Empty expression\n");
        return NAN;
    }

    // Initialize parser
    expr_pos = expression;
    get_next_token();
    
    // Parse as statement (handles declarations, assignments, expressions)
    double result = parse_statement();
    
    // Check if we consumed the entire expression
    if (current_token.type != CALC_TOKEN_END && current_token.type != CALC_TOKEN_SEMICOLON) {
        fprintf(stderr, "Error: Unexpected characters at end of expression\n");
        return NAN;
    }
    
    return result;
}

// Load and execute a script file
static int load_script_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    char *line = NULL;
    size_t line_capacity = 0;
    char *accumulated = malloc(4096);
    size_t accumulated_capacity = 4096;
    size_t accumulated_length = 0;
    
    if (!accumulated) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(fp);
        return -1;
    }
    
    accumulated[0] = '\0';
    
    int line_num = 0;
    int func_count = 0;
    int var_count = 0;
    int brace_count = 0;
    int paren_count = 0;
    int in_multiline = 0;
    int saved_func_count = 0;
    int saved_var_count = 0;
    
    // Enable silent mode for script loading
    silent_mode = 1;
    
    // Count existing functions and variables
    UserFunction *uf = user_functions;
    while (uf) {
        saved_func_count++;
        uf = uf->next;
    }
    Variable *v = variables;
    while (v) {
        saved_var_count++;
        v = v->next;
    }
    
    while (getline(&line, &line_capacity, fp) != -1) {
        line_num++;
        
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        // Skip empty lines and comments
        char *trimmed = line;
        while (*trimmed && isspace(*trimmed)) trimmed++;
        if (*trimmed == '\0' || *trimmed == '#') {
            continue;
        }
        
        // Calculate space needed
        size_t space_needed = accumulated_length + (accumulated_length > 0 ? 1 : 0) + len + 1;
        
        // Grow buffer if needed
        if (space_needed > accumulated_capacity) {
            size_t new_capacity = accumulated_capacity * 2;
            while (new_capacity < space_needed) {
                new_capacity *= 2;
            }
            char *new_accumulated = realloc(accumulated, new_capacity);
            if (!new_accumulated) {
                fprintf(stderr, "Error: Memory allocation failed at line %d\n", line_num);
                free(accumulated);
                free(line);
                fclose(fp);
                return -1;
            }
            accumulated = new_accumulated;
            accumulated_capacity = new_capacity;
        }
        
        // Append line
        if (accumulated_length > 0) {
            strcat(accumulated, " ");
            accumulated_length++;
        }
        strcat(accumulated, trimmed);
        accumulated_length += strlen(trimmed);
        
        // Count braces and parentheses
        brace_count = 0;
        paren_count = 0;
        int in_string = 0;
        
        for (const char *p = accumulated; *p; p++) {
            if (*p == '"') {
                in_string = !in_string;
            } else if (!in_string) {
                if (*p == '{') brace_count++;
                else if (*p == '}') brace_count--;
                else if (*p == '(') paren_count++;
                else if (*p == ')') paren_count--;
            }
        }
        
        // Check if we need more input
        int needs_more = 0;
        if (brace_count > 0 || paren_count > 0) {
            needs_more = 1;
        }
        
        if (accumulated_length > 0) {
            char last_char = accumulated[accumulated_length - 1];
            if (last_char == '{' || last_char == ',' || last_char == '(' || 
                last_char == '+' || last_char == '-' || last_char == '*' || 
                last_char == '/' || last_char == '^') {
                needs_more = 1;
            }
        }
        
        if (needs_more) {
            in_multiline = 1;
            continue;
        }
        
        // We have a complete statement
        in_multiline = 0;
        
        // Track if this is a function or variable definition
        int is_func_def = (strstr(accumulated, "var ") == accumulated && 
                          strchr(accumulated, '(') != NULL && 
                          strchr(accumulated, '{') != NULL);
        int is_var_def = (strstr(accumulated, "var ") == accumulated && !is_func_def);
        
        // Execute the statement
        double result = compute_expression(accumulated);
        
        // Only report errors for non-definition statements
        // Function and variable definitions may return NAN but that's OK
        if (isnan(result) && !is_func_def && !is_var_def) {
            fprintf(stderr, "Error in %s:%d: Failed to evaluate statement\n", 
                    filename, line_num);
        }
        
        if (is_func_def) func_count++;
        else if (is_var_def) var_count++;
        
        // Clear accumulated buffer
        accumulated[0] = '\0';
        accumulated_length = 0;
    }
    
    // Check for incomplete statement
    if (accumulated_length > 0) {
        fprintf(stderr, "Warning: Incomplete statement at end of %s\n", filename);
    }
    
    free(accumulated);
    free(line);
    fclose(fp);
    
    // Calculate actual new counts
    int new_func_count = 0;
    uf = user_functions;
    while (uf) {
        new_func_count++;
        uf = uf->next;
    }
    int new_var_count = 0;
    v = variables;
    while (v) {
        new_var_count++;
        v = v->next;
    }
    
    func_count = new_func_count - saved_func_count;
    var_count = new_var_count - saved_var_count;
    
    // Restore normal mode
    silent_mode = 0;
    
    if (func_count > 0 || var_count > 0) {
        printf("Loaded ");
        if (func_count > 0) {
            printf("%d function%s", func_count, func_count == 1 ? "" : "s");
            if (var_count > 0) printf(", ");
        }
        if (var_count > 0) {
            printf("%d variable%s", var_count, var_count == 1 ? "" : "s");
        }
        printf(" from %s\n", filename);
    } else {
        printf("Loaded %s\n", filename);
    }
    
    return 0;
}

// Parse and execute load command
static void parse_load_command(const char *line) {
    // Skip "load" and whitespace
    const char *p = line + 4;
    while (*p && isspace(*p)) p++;
    
    if (*p == '\0') {
        fprintf(stderr, "Error: load command requires a filename\n");
        fprintf(stderr, "Usage: load \"filename.calc\" or load filename.calc\n");
        return;
    }
    
    // Extract filename (with or without quotes)
    char filename[256];
    int i = 0;
    
    if (*p == '"') {
        // Quoted filename
        p++;
        while (*p && *p != '"' && i < 255) {
            filename[i++] = *p++;
        }
    } else {
        // Unquoted filename
        while (*p && !isspace(*p) && i < 255) {
            filename[i++] = *p++;
        }
    }
    
    filename[i] = '\0';
    
    if (filename[0] == '\0') {
        fprintf(stderr, "Error: Empty filename\n");
        return;
    }
    
    load_script_file(filename);
}

int main(int argc, char *argv[])
{
    char *input = NULL;        // Dynamic buffer for accumulated input
    char *line = NULL;         // Dynamic buffer for single line
    size_t input_capacity = 0;
    size_t input_length = 0;
    size_t line_capacity = 0;
    double result;
    int brace_count = 0;
    int paren_count = 0;
    int in_multiline = 0;
    
    // Enable colors
    enable_colors();
    
    // Check for command-line script file
    int loaded_scripts = 0;
    if (argc > 1) {
        // Load script file(s) from command line
        for (int i = 1; i < argc; i++) {
            if (load_script_file(argv[i]) == 0) {
                loaded_scripts++;
            } else {
                fprintf(stderr, "Failed to load %s\n", argv[i]);
            }
        }
        printf("\n");
    }
    
    print_normal("RCalc - Mathematical Expression Calculator with Variables and Functions\n");
    print_normal("Type 'help' for help\n");
    print_normal("Type 'quit' to exit\n\n");
    
    // Initialize input buffer
    input_capacity = 1024;
    input = malloc(input_capacity);
    if (!input) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    input[0] = '\0';
    input_length = 0;
    
    // Initialize line buffer
    line_capacity = 1024;
    line = malloc(line_capacity);
    if (!line) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(input);
        return 1;
    }
    
    while (1) {
        if (in_multiline) {
            print_normal("... ");  // Continuation prompt
        } else {
            print_prompt();
        }
        fflush(stdout);
        
        // Read input line with dynamic allocation
        ssize_t chars_read = getline(&line, &line_capacity, stdin);
        if (chars_read == -1) {
            print_normal("\nGoodbye!\n");
            break;
        }
        
        // Remove trailing newline
        if (chars_read > 0 && line[chars_read - 1] == '\n') {
            line[chars_read - 1] = '\0';
            chars_read--;
        }
        
        // Handle quit command on any line
        if (strcmp(line, "quit") == 0) {
            print_normal("Goodbye!\n");
            break;
        }
        
        // Handle help command on any line
        if (strcmp(line, "help") == 0) {
            show_help();
            // Clear any accumulated input and reset multiline state
            input[0] = '\0';
            input_length = 0;
            in_multiline = 0;
            brace_count = 0;
            paren_count = 0;
            continue;
        }
        
        // Handle load command
        if (strncmp(line, "load", 4) == 0 && (line[4] == '\0' || isspace(line[4]))) {
            parse_load_command(line);
            // Clear any accumulated input and reset multiline state
            input[0] = '\0';
            input_length = 0;
            in_multiline = 0;
            brace_count = 0;
            paren_count = 0;
            continue;
        }
        
        // If we're starting fresh and line is empty, skip
        if (!in_multiline && chars_read == 0) {
            continue;
        }
        
        // Calculate required space for appending this line
        size_t line_len = strlen(line);
        size_t space_needed = input_length + (input_length > 0 ? 1 : 0) + line_len + 1;
        
        // Grow input buffer if needed
        if (space_needed > input_capacity) {
            size_t new_capacity = input_capacity;
            while (new_capacity < space_needed) {
                new_capacity *= 2;
            }
            
            char *new_input = realloc(input, new_capacity);
            if (!new_input) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                break;
            }
            input = new_input;
            input_capacity = new_capacity;
        }
        
        // Append current line to input buffer
        if (input_length > 0) {
            strcat(input, " ");  // Add space between lines
            input_length++;
        }
        strcat(input, line);
        input_length += line_len;
        
        // Count braces and parentheses to determine if we need more input
        brace_count = 0;
        paren_count = 0;
        int in_string = 0;
        
        for (const char *p = input; *p; p++) {
            if (*p == '"') {
                in_string = !in_string;
            } else if (!in_string) {
                if (*p == '{') brace_count++;
                else if (*p == '}') brace_count--;
                else if (*p == '(') paren_count++;
                else if (*p == ')') paren_count--;
            }
        }
        
        // Check if we need more input
        int needs_more = 0;
        
        // If we have unmatched braces or parentheses
        if (brace_count > 0 || paren_count > 0) {
            needs_more = 1;
        }
        
        // If line ends with certain tokens that suggest continuation
        if (input_length > 0) {
            char last_char = input[input_length - 1];
            if (last_char == '{' || last_char == ',' || last_char == '(' || last_char == '+' || 
                last_char == '-' || last_char == '*' || last_char == '/' || last_char == '^') {
                needs_more = 1;
            }
        }
        
        // Check for incomplete function definition (has "double" and identifier but no complete body)
        if (!needs_more && strstr(input, "double ") == input) {
            // Simple heuristic: if it starts with "double" but doesn't have both { and }
            int has_open_brace = strchr(input, '{') != NULL;
            int has_close_brace = strchr(input, '}') != NULL;
            if (has_open_brace && !has_close_brace) {
                needs_more = 1;
            }
        }
        
        if (needs_more) {
            in_multiline = 1;
            continue;
        }
        
        // We have a complete statement, evaluate it
        in_multiline = 0;
        
        result = compute_expression(input);
        if (!isnan(result)) {
            printf("= %.10g\n", result);
        }
        printf("\n");
        
        // Clear input buffer for next statement
        input[0] = '\0';
        input_length = 0;
    }
    
    // Cleanup
    free(input);
    free(line);
    free_variables(variables);
    free_user_functions();
    
    return 0;
}