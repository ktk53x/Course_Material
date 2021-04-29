ASSIGNMENT-4: YACC PARSER 
Kartikeya Saxena
180101034


TO RUN:
chmod +x run.sh // make sure that this file has executable rights
./run.sh
./a.out < input.pas
./a.out < input_error.pas
./a.out < input_error_2.pas

___________________________________________________________________

ERRORS:
I have handled the following semantic errors:
1. Duplicate declaration
2. Missing declaration
3. Missing initialization
4. Type Mismatch
___________________________________________________________________

READING THE CODE:(ans.l)
DEFINITIONS:
1. SIZE: number of buckets in the hash table.
2. NOT_INITIALIZED: default value to be given if not initialized  

STRUCTURES:
1. symbol: represents a symbol in the hash table with specifier string, the type of the symbol, integer and float value associated with it, flags for being initialized or declared and next pointer to point to the next entry in the hash table.
2. node: represent a node of a linked list with specifier strings and next pointer to the next entry of the linked list.

SUBROUTINES:

** Related to Linked List **
1. int search_id(char* specifier): searches the the specifier in the linked list and returns 0 if not found and 1 if found.
2. int add_id(char* specifier): adds a new specifier at the beginning of the linked list, if it is not already present in the list. Returns 0 if not added successfully and 1 if added successfully.
3. void print_id_list(): prints the linked list from head to tail.

** Related to hash table **
1. void init():  initializes all the buckets of the hash table to NULL.
2. int hash_function(char* specifier): returns a simple hash function which is just the sum of the ascii values of the characters of specifier modulo SIZE. 
3. symbol* search(char* specifier): searches for the specifier in the hash table.
4. symbol* insert(char* specifier, char type): inserts a new symbol of the given type into the hash table with values set as NOT_INITIALIZED and all flags set to 0.
5. void print(): pretty prints the hash table.

** Related to communication with yacc file **
1. get_line_number(): returns the number of lines lex has encountered till now.
2. get_int_value(char* specifier):
3. get_real_value(char* specifier):
4. set_int_value(char* specifier, int val):
5. set_float_value(char* specifier, float val):
The above 4 functions are used to get and set values from the hash table
6. is_declared(char* specifier):
7. is_initialized(char* specifier):
The above 2 function are used to check whether the variable has been initialized/declared or not.
8. int get_type(char* specifier): tells the type of variable 0 for integer('I'), 1 for float('F') and 2 for not assigned type yet('V')
9. int set_type(int type): sets the type of all the specifiers currently in the list to the given type, sets declare flags to 1 and resets the list. Returns 0 if any duplicate declaration otherwise 1
10. set_init(): sets the init flag of all variables in the list and resets the list. Returns 0 if any missing declaration otherwise 1.
11. clear_list(): clears the list


___________________________________________________________________

READING THE CODE:(ans.y)
DEFINITIONS:
1. NOT_INIT: the default value if variables are not initialized
2. each token as %token 

STRUCTURES:
1. to use both char* and (float/int) for parsing I used %union which serves the purpose of both by unionizing them. Struct Number is used to represent both real and integer as one single entity i.e. Number.

SUBROUTINES:
1. yerror(const char *str): for printing errors  

%left gives left associativity to the operators and order gives precedence

PRODUCTIONS WITH ACTIONS:
1. program: it prints the hash table after the code has been parsed entirely. This is the starting production.
2. type: sets the type of integer or real to all the ids in the idlist using the set_type() function and prints duplicate declaration if present.
3. idlist: adds id into the idlist using add_id() function and if already present in the list prints duplicate declaration.
4. assign: checks whether variable is declared or not. then if declared checks whether types match or not if they don't match it is not initialized.
5. exp: checks for type mismatch if it is there then the expression is assigned real type.
6. term: checks for type mismatch if it is there then the expression is assigned real type.
7. factor: check for declaration and initialization, if not initialized, give it and integer type.
8. read: it sets the initialized flag of the idlist ids to 1 and clears the idlist
9. write: clears the idlist after done parsing.
10. indexexp:

ASSUMPTIONS: (after error detections)
1. If there is duplicate declaration, then the parser goes with the first declaration.
2. If the variable is not declared it is not declared while assigning it values.
3. If there is type mismatch while assigning then the variable is not initialized.
4. If there is mismatch in expression types then after error, the whole expression is given type real.
5. If a variable is not declared, its expression is given type integer.


Note: I am also printing symbol table at the end of errors.
