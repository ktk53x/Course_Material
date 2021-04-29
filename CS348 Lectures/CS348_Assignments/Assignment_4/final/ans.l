%{
	/* header files */
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "y.tab.h"
	#define NOT_INITIALIZED -10000000

	/* declaration of useful functions and variables */
	char* install_id();
	int install_int();
	float install_real();
	int line = 0;

	/* linked list implementation */
	typedef struct node {
		char* specifier;
		struct node* next;
	} node;

	node* head = NULL;

	int search_id(char* specifier) {
		// search the specifier in the linked list
		node* temp = head;
		while(temp != NULL && strcmp(specifier, temp -> specifier)) {
			temp = temp -> next; 
		}
		// 0 if not found, 1 if found
		return (temp != NULL);
	}

	int add_id(char* specifier) {
		// add a new id to the beggining of the list and return success / failure
		node* temp = (node *)malloc(sizeof(node));
		temp -> specifier = (char *)strdup(specifier);
		temp -> next = NULL;

		// if the id is already present then don't add and return failure
		if(search_id(specifier)) {
			return 0;
		}

		if(head) {
			temp -> next = head;
			head = temp;
		}
		else
			head = temp;

		// 0 for failure 1 for success
		return 1;
	}


	void print_id_list() {
		// print the linked list
		node* temp = head;
		while(temp != NULL) {
			printf("%s -> ", temp -> specifier);
			temp = temp -> next;
		}
		printf("NULL\n");
	}


	/* hashtable implementation */
	#define SIZE 20

	typedef struct symbol {
		char* specifier;
		float real;
		int integer;
		char type;
		int initialized;
		int declared;
		struct symbol* next;
	} symbol;

	symbol* SYMTAB[SIZE];

	int hash_function(char* specifier) {
		int len = strlen(specifier);
		// simple hash function
		int hash = 0;
		for(int i = 0; i < len; i++)
			hash += (int) specifier[i];
		return hash % SIZE;
	}

	void init() {
		// initialize all bucket to null
		for(int i = 0; i < SIZE; i++)
			SYMTAB[i] = NULL;
	}

	symbol* search(char* specifier) {
		// compute the hash function for bucket index
		int i = hash_function(specifier);

		// search the linked list associated with the bucket
		symbol* temp = SYMTAB[i];
		while(temp != NULL && strcmp(temp -> specifier, specifier))
			temp = temp -> next;

		return temp;
	}

	symbol* insert(char* specifier, char type) {
		// compute the hash function for bucket index
		int i = hash_function(specifier);

		// create the new symbol
		symbol* new_symbol = (symbol *)malloc(sizeof(symbol));
		new_symbol -> specifier = (char *)strdup(specifier);
		new_symbol -> real = NOT_INITIALIZED;
		new_symbol -> integer = NOT_INITIALIZED;
		new_symbol -> type = type;
		new_symbol -> initialized = 0;
		new_symbol -> declared = 0;

		// insert it at the beginning of the bucket linked list
		new_symbol -> next = SYMTAB[i];
		SYMTAB[i] = new_symbol;

		return new_symbol;
	}

	void print_hash_table() {
		// pretty printing of hash table
		printf("-------------------------------------------------------------------------------------\n");
		printf("SYMTAB\n");
		for(int i = 0; i < SIZE; i++) {
			printf("bucket [%d]: ", i);
			symbol* temp = SYMTAB[i];
			while(temp != NULL) {
				printf("<%s %c : ", temp -> specifier, temp -> type);
				printf("{ ");
				printf("declared: %d, initialized: %d ", temp -> declared, temp -> initialized);
				printf("}> -> ");
				temp = temp -> next;
			}
			printf("NULL\n");
		}
		printf("-------------------------------------------------------------------------------------\n");
	}

	
	int get_line_number() {
		// returns the current line number to the parser
		return line;
	}

	int get_int_value(char* specifier) {
		// search the symbol and get the value of the symbol
		symbol* sym = search(specifier);
		if(sym != NULL)
			return sym -> integer; 
	}

	float get_real_value(char* specifier) {
		// search the symbol and get the value of the symbol
		symbol* sym = search(specifier);
		if(sym != NULL)
			return sym -> real; 

	}

	void set_int_value(char* specifier, int val) {
		// search the symbol and set its value to val
		symbol* sym = search(specifier);
		if(sym != NULL) {
			sym -> integer = val;
			sym -> type = 'I';
			sym -> initialized = 1;
		}
	}

	void set_real_value(char* specifier, float val) {
		// search the symbol and set its value to val
		symbol* sym = search(specifier);
		if(sym != NULL) {
			sym -> real = val;
			sym -> type = 'F';
			sym -> initialized = 1;
		}
	}

	int is_declared(char* specifier) {
		// check if the specifier is declared or not
		symbol* sym = search(specifier);
		if(sym != NULL)
			return (sym -> declared);
	}

	int is_initialized(char* specifier) {
		// check if the specifier is initialized or not
		symbol* sym = search(specifier);
		if(sym != NULL) {
			return (sym -> initialized);
		}
		
	}

	int get_type(char* specifier) {
		// search the symbol and get its type
		symbol* sym = search(specifier);
		if(sym != NULL) {
			if(sym -> type == 'I')
				return 0;
			if(sym -> type == 'F')
				return 1;
			if(sym -> type == 'V')
				return 2;
		}
	}

	int set_type(int type) {
		// once the declaration list is complete the set the declaration of the variables and set their type
		node* temp = head;
		int flag = 1;
		while(temp != NULL) {
			symbol* sym = search(temp -> specifier);
			if(sym -> declared)
				flag = 0; // duplicate error
			else {
				if(type == 0)
					sym -> type = 'I';
				if(type == 1)
					sym -> type = 'F';
				sym -> declared = 1;
			}
			temp = temp -> next;
		}
		head = NULL;
		return flag;
	}

	int set_init() {
		// while reading we need to initialize all the variables in the id list
		node* temp = head;
		int flag = 1;
		while(temp != NULL) {
			symbol* sym = search(temp -> specifier);
			if(sym -> declared == 0) // missing declaration
				flag = 0;
			else 
				sym -> initialized = 1;
			temp = temp -> next;
		}
		head = NULL;
		return flag;
	}

	void clear_list() {
		head = NULL;
	}

%}

delim		[ \t\n]
letter		[A-Za-z]
digit		[0-9]
id		{letter}({letter}|{digit})*
int		{digit}+
real  		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
\n		{ line++;									}
[ \t]		{										}
PROGRAM		{ return _PROGRAM ;								}
VAR		{ return _VAR ;									}
BEGIN		{ return _BEGIN ;								}
END		{ return _END ;									}
END\.		{ return _END_DOT ;								}
INTEGER		{ return _INTEGER ;								}
REAL		{ return _REAL ;								}
FOR		{ return _FOR ;									}
READ		{ return _READ;									}
WRITE		{ return _WRITE ;								}
TO		{ return _TO;									}
DO		{ return _DO;									}
\;		{ return _SEMICOLON;								}
\:		{ return _COLON;								}
\,		{ return _COMMA;								}
\:\=		{ return _ASSIGN;								}
\+		{ return _ADD;									}
\-		{ return _SUB;									}
\*		{ return _MUL;									}
DIV		{ return _DIV;									}
\(		{ return _OPEN_BRACE;								}
\)		{ return _CLOSE_BRACE;								}
{id}		{ yylval.ID = install_id();  return _ID;					}
{int}		{ yylval.NUMBER.integer = install_int(); yylval.NUMBER.type = 0; return _INT;	}
{real}		{ yylval.NUMBER.real = install_real(); yylval.NUMBER.type = 1; return _FLOAT;	}
.		{ printf("error: unexpected character syntax error, line number: %d\n", line + 1);	}
%%

char* install_id() {
	symbol* sym = search(yytext);
	if(sym == NULL)
		sym = insert(yytext, 'V');
	return strdup(yytext);
}

int install_int() {
	return atoi(yytext);
}

float install_real() {
	return atof(yytext);
}



