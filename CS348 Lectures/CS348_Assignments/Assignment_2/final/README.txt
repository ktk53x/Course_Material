ASSIGNMENT-2: SIC/XE 2 PASS ASSEMBLER AND LINKER LOADER
Kartikeya Saxena
180101034


TO RUN:
g++ -g assembler.cpp
./a.out input.txt

The records for the generated object code can be found in output.txt
The listing for each instruction can be found in listing.txt
The intermediate file is given by intermediate.txt

g++ -g linker_loader.cpp
./a.out output.txt
It will prompt with:
Enter PROGADDR for the program: 
Enter 0 here (or the address you want to load the code at)

The memory visual for the loaded program can be found in memory.txt
The line wise output can be find in line_by_line.txt

___________________________________________________________________

ASSUMPTIONS:
1. The code is limited to the instruction set provided in the question
2. FIELD SIZE is 10 for label and opcode and 30 for the operands. Extra column is provided for '+', '=' and '#'.
3. The program name is assumed to be atmost 6 characters long.
4. Make sure there are no spaces in the expressions in the input file.
___________________________________________________________________

READING THE CODE:(assembler.cpp)
HELPER FUNCTIONS:
1. strip(): removes trailing spaces
2. format_number() and format_string(): for pretty printing the code
3. is_number(): to check if a string is a number or not
4. apply_mask(): so that printing can be done in two's complement properly (only for negative numbers)

STRUCTURES:
1. OPTAB: returns opcode along with instruction length
2. SYMBOL: for storing symbol name along with their CSECT
3. VALUE: to store the address value and type of the SYMBOL
4. map<SYMBOL, VALUE> SYMTAB: symbol table
5. map<SYMBOL, VALUE> LLITTAB: literal table
6. set<string> EXTREF: the set of external references
7. ExpEvaluator: It evaluates the expression consisting of (+, -, *, / ()) and returns the value
8. instruction: this structure stores the label, operation code, operand and i, n, p, b, e, x flags for each instruction.

SUBROUTINES:
1. process_line: takes the line from input/intermediate file and tokenize to create instructions
2. preprocess: stores the register names and values in the SYMTAB
3. main(): it checks if the arguments are provided properly and calls pass_1() and pass_2()
4. pass_1(): Reads the input file line by line and produces the intermediate file with appropriate LOCCTR values for each line. It has been properly commented.
5. pass_2(): Reads the intermediate file line by line and produces output file and listing file. Each subcase is commented.

___________________________________________________________________

READING THE CODE:(linker_loader.cpp)

HELPER FUNCTIONS:
1. strip(): removes trailing spaces
2. format_number() and format_string(): for pretty printing the code
3. is_number(): to check if a string is a number or not

STRUCTURES:
1. vector<string> memory: the simulated memory for the program
2. map<string, int> ESTAB: symbol table for the external references and csect names

SUBROUTINES:
1. vector<pair<string, int>> get_symbols(): tokenize the symbols in the define record
2. main(): it checks if the arguments are provided properly and calls pass_1() and pass_2() and then prints the results
3. pass_1(): Reads the input record file and stores all the external definitions and csect names. It has been properly commented
4. pass_2(): Reads the input record file and loads the text records in the simulated memory and then loads the modifications. It has been properly commented
5. pretty_print_memory(): prints in the format as given in memory.txt
6. line_by_line_print_memory(): prints each instruction line by line

___________________________________________________________________

NOTE: Please don't open the code with visual-studio IDE it might mess up the indentation of my code. Use any standard text-editor like vim, gedit or sublime text. Also I would appreciate if you write some feedback on the overall coding style.
