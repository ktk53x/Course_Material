ASSIGNMENT-3: LEXICAL ANALYSIS 
Kartikeya Saxena
180101034


TO RUN:
lex ans.l
gcc -lfl lex.yy.c
./a.out < input.pas

___________________________________________________________________

READING THE CODE:(ans.l)
DEFINITIONS:
1. SIZE: number of buckets in the hash table.
2. Each token has a define for its token number.

STRUCTURES:
1. symbol: represents a symbol in the hash table with specifier value and the type of the symbol and next pointer to point to the next entry in the hash table.

SUBROUTINES:
1. void init():  initializes all the buckets of the hash table to NULL.
2. int hash_function(char* specifier): returns a simple hash function which is just the sum of the ascii values of the characters of specifier modulo SIZE. 
3. symbol* search(char* specifier): searches for the specifier in the hash table.
4. symbol* insert(char* specifier, char type): inserts a new symbol into the hash table.
5. void print(): pretty prints the hash table.
6. void* install_id(): installs the specifier and type of id into the hash table and returns the pointer to the entry.
7. void* install_num(): installs the specifier and type of num into the hash table and returns the pointer to the entry.
8. int main(): takes input token by token and prints the lexical scan of the program along with the hash table.

NOTE: 180101034.c is the lex.yy.c file as being compiled on my system, you can run that too.
