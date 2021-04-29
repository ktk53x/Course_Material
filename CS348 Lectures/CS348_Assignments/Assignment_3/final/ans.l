%{
	/* header files */
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>

	/* definitions of manifest constants*/
	#define _PROGRAM	1
	#define _VAR		2
	#define _BEGIN		3
	#define _END		4
	#define _END_DOT	5
	#define _INTEGER	6
	#define _FOR		7
	#define _READ		8
	#define _WRITE		9
	#define _TO		10
	#define _DO		11
	#define _SEMICOLON	12
	#define _COLON		13
	#define _COMMA		14
	#define _ASSIGN		15
	#define _ADD		16
	#define _SUB		17
	#define _MUL		18
	#define _DIV		19
	#define _OPEN_BRACE	20
	#define _CLOSE_BRACE	21
	#define _ID		22
	#define _INT		23

	/* hashtable implementation */
	#define SIZE 20

	typedef struct symbol {
		char* specifier;
		char type;
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
		new_symbol -> type = type;

		// insert it at the beginning of the bucket linked list
		new_symbol -> next = SYMTAB[i];
		SYMTAB[i] = new_symbol;

		return new_symbol;
	}

	void print() {
		// pretty printing of hash table
		printf("-------------------------------------\n");
		printf("SYMTAB\n");
		for(int i = 0; i < SIZE; i++) {
			printf("bucket [%d]: ", i);
			symbol* temp = SYMTAB[i];
			while(temp != NULL) {
				printf("%c%s -> ", temp -> type, temp -> specifier);
				temp = temp -> next;
			}
			printf("NULL\n");
		}
		printf("-------------------------------------\n");
	}

	


	/* declaration of useful functions and variables */
	void* yylval;
	void* install_id();
	void* install_num();
	int line = 0;
%}

delim		[ \t\n]
letter		[A-Za-z]
digit		[0-9]
id		{letter}({letter}|{digit})*
int		{digit}+

%%
\n		{ line++;					}
[ \t]		{						}
PROGRAM		{ return _PROGRAM ;				}
VAR		{ return _VAR ;					}
BEGIN		{ return _BEGIN ;				}
END		{ return _END ;					}
END\.		{ return _END_DOT ;				}
INTEGER		{ return _INTEGER ;				}
FOR		{ return _FOR ;					}
READ		{ return _READ;					}
WRITE		{ return _WRITE ;				}
TO		{ return _TO;					}
DO		{ return _DO;					}
\;		{ return _SEMICOLON;				}
\:		{ return _COLON;				}
\,		{ return _COMMA;				}
\:\=		{ return _ASSIGN;				}
\+		{ return _ADD;					}
\-		{ return _SUB;					}
\*		{ return _MUL;					}
DIV		{ return _DIV;					}
\(		{ return _OPEN_BRACE;				}
\)		{ return _CLOSE_BRACE;				}
{id}		{ yylval = (void*)install_id();  return _ID;	}
{int}		{ yylval = (void*)install_num(); return _INT;	}
%%

void* install_id() {
	symbol* sym = search(yytext);
	if(sym == NULL)
		sym = insert(yytext, '^');
	return sym;
}

void* install_num() {
	symbol* sym = search(yytext);
	if(sym == NULL)
		sym = insert(yytext, '#');
	return sym;
}


int main() {
	init();

	int token;
	int prev_line = 0;

	/* parsing the every token of the input program */
	token = yylex();
	while(token) {

		/* printing the line number */
		if(line == 0) {
			line++;
			printf("%10d", line);
		} else {
			if(line == prev_line)
				printf("%10c", ' ');
			else
				printf("%10d", line);
		}

		/* printing the token id, type and specifier */
		symbol* sym = NULL;
		if(token == 22 || token == 23)
			sym = (symbol *)yylval;
		if(sym != NULL)
			printf("%10d %10c%s\n", token, sym -> type, sym -> specifier);
		else
			printf("%10d \n", token);

		prev_line = line;
		token = yylex();
	}

	print();
	return 0;
}
