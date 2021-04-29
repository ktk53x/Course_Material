#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<set>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include<stack>

using namespace std;

#define FIELD_LENGTH 10
#define OPERAND_LENGTH 30

// helper functions
string strip(string s)
{
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	return s;
}

string format_number(int num, int width, char pad = '0')
{
	stringstream temp;
	temp <<  std::hex << std::uppercase << std::setfill(pad) << std::setw(width) << num;
	return temp.str();
}

string format_string(string name, int width, char pad = ' ')
{
	stringstream temp;
	temp << std::left << std::setfill(pad) << std::setw(width) << name;
	return temp.str();
}

bool is_number(string s)
{
	auto it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void apply_mask(int& value, int bits)
{
	if(value < 0)
	{
		int mask = 0;
		for(int i = 0; i < bits; i++)
		{
			mask |= 1;
			mask = mask << 1;
		}
		mask = mask >> 1;
		value = value & mask;
	}
}
//<-------------------------------------------------------------->

// Operation Table (format, opcode)
pair<int, int> OPTAB(string opcode)
{
	if(opcode == "LDA")	return make_pair(3,0x00);
	if(opcode == "LDX")	return make_pair(3,0x04);
	if(opcode == "LDL")	return make_pair(3,0x08);
	if(opcode == "LDB")	return make_pair(3,0x68);
	if(opcode == "LDT")	return make_pair(3,0x74);
	if(opcode == "STA")	return make_pair(3,0x0C);
	if(opcode == "STX")	return make_pair(3,0x10);
	if(opcode == "STL")	return make_pair(3,0x14);
	if(opcode == "LDCH")	return make_pair(3,0x50);
	if(opcode == "STCH")	return make_pair(3,0x54);
	if(opcode == "ADD")	return make_pair(3,0x18);
	if(opcode == "SUB")	return make_pair(3,0x1C);
	if(opcode == "MUL")	return make_pair(3,0x20);
	if(opcode == "DIV")	return make_pair(3,0x24);
	if(opcode == "COMP")	return make_pair(3,0x28);
	if(opcode == "COMPR")	return make_pair(2,0xA0);
	if(opcode == "CLEAR")	return make_pair(2,0xB4);
	if(opcode == "J")	return make_pair(3,0x3C);
	if(opcode == "JLT")	return make_pair(3,0x38);
	if(opcode == "JEQ")	return make_pair(3,0x30);
	if(opcode == "JGT")	return make_pair(3,0x34);
	if(opcode == "JSUB")	return make_pair(3,0x48);
	if(opcode == "RSUB")	return make_pair(3,0x4C);
	if(opcode == "TIX")	return make_pair(3,0x2C);
	if(opcode == "TIXR")	return make_pair(2,0xB8);
	if(opcode == "TD")	return make_pair(3,0xE0);
	if(opcode == "RD")	return make_pair(3,0xD8);
	if(opcode == "WD")	return make_pair(3,0xDC);
	
	// not found
	return make_pair(-1, -1);
}

struct SYMBOL
{
	string CSECT;
	string name;
	SYMBOL(string CSECT, string name) 
	{
		this -> CSECT = CSECT;
		this -> name = name;
	}
	bool operator<(const SYMBOL& s) const
	{
		return make_pair(this -> CSECT, this -> name) < make_pair(s.CSECT, s.name);
	}
};

struct VALUE
{
	int value;
	// type 0: absolute, 1: relative
	bool type;
	int length;
	VALUE(int value = 0, int length = 3, bool type = 1)
	{
		this -> value = value;
		this -> length = length;
		this -> type = type;
	}
};

// Symbol Table SYMTAB[(csect,name)] = address
map<SYMBOL, VALUE> SYMTAB;

// Literal Pool LITTAB[(csect,name)] = address
map<SYMBOL, VALUE> LITTAB;

set<string> EXTREF;

// Some preprocessed constants in SYMTAB
void preprocess(string CSECT)
{
	// other mnemonics
	SYMTAB.insert({SYMBOL(CSECT,"A" ),VALUE(0, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"X" ),VALUE(1, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"L" ),VALUE(2, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"PC"),VALUE(8, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"SW"),VALUE(9, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"B" ),VALUE(3, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"S" ),VALUE(4, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"T" ),VALUE(5, 3, 0)});
	SYMTAB.insert({SYMBOL(CSECT,"F" ),VALUE(6, 3, 0)});
}

struct ExpEvaluator
{
	string CSECT;
	ExpEvaluator(string CSECT)
	{
		this -> CSECT = CSECT;
	}
	int precedence(string op)
	{
		if(op == "+" || op == "-")
			return 1;
		if(op == "*" || op == "/")
			return 2;
		return 0;
	}
	pair<int, int> apply_operator(pair<int, int> a, pair<int, int> b, string op)
	{
		if(op == "+") return make_pair(a.first + b.first, a.second + b.second);
		if(op == "-") return make_pair(a.first - b.first, a.second - b.second);
		if(op == "*") return make_pair(a.first * b.first, a.second * b.second);
		if(op == "/") return make_pair(a.first / b.first, a.second / b.second);
		return make_pair(0, 0);
	}

	vector<string> tokenize(string exp)
	{
		int n = exp.length();
		vector<string> tokens;
		int i = 0;
		while(i < n)
		{
			if(exp[i] == '*' || exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == ')' || exp[i] == '(')
			{
				tokens.push_back(exp.substr(i, 1));
				i++;
			}
			else
			{
				int j = i;
				while(j < n && !(exp[j] == '*' || exp[j] == '+' || exp[j] == '-' || exp[j] == '/' || exp[j] == '(' || exp[j] == ')'))
					j++;
				tokens.push_back(exp.substr(i, j - i));
				i = j;
			}
		}
		return tokens;
	}

	pair<int, int> evaluate(string exp)
	{
		vector<string> tokens = tokenize(exp);

		stack<pair<int, int>> operands;
		stack<int> type;
		stack<string> operators;

		int n = tokens.size();
		for(int i = 0; i < n; i++)
		{
			if(tokens[i] == "(")
				operators.push("(");
			else if(is_number(tokens[i]))
			{
				operands.push(make_pair(stoi(tokens[i]), stoi(tokens[i])));
			}
			else if(tokens[i] == ")")
			{
				while(!operators.empty() && operators.top() != "(")
				{
					pair<int, int> operand_2 = operands.top();
					operands.pop();

					pair<int, int> operand_1 = operands.top();
					operands.pop();

					string op = operators.top();
					operators.pop();

					operands.push(apply_operator(operand_1, operand_2, op));
				}
				if(!operators.empty())
					operators.pop();
			}
			else if(tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/")
			{
				while(!operators.empty() && precedence(operators.top()) >= precedence(tokens[i]))
				{
					pair<int, int> operand_2 = operands.top();
					operands.pop();

					pair<int, int> operand_1 = operands.top();
					operands.pop();

					string op = operators.top();
					operators.pop();

					operands.push(apply_operator(operand_1, operand_2, op));
				}

				operators.push(tokens[i]);
			}	
			else
			{
				if(SYMTAB.find(SYMBOL(CSECT, tokens[i])) != SYMTAB.end())
				{
					int value = SYMTAB[SYMBOL(CSECT, tokens[i])].value;
					int type = SYMTAB[SYMBOL(CSECT, tokens[i])].type;
					operands.push(make_pair(type, value));
				}
				else
				{
					if(EXTREF.find(tokens[i]) != EXTREF.end())
					{
					}
					operands.push(make_pair(0, 0));
				}
			}	
		}
		while(!operators.empty())
		{
			pair<int, int> operand_2 = operands.top();
			operands.pop();

			pair<int, int> operand_1 = operands.top();
			operands.pop();

			string op = operators.top();
			operators.pop();

			operands.push(apply_operator(operand_1, operand_2, op));
		}
		return operands.top();
	}
};

// instruction class
struct instruction
{
	string label;
	string opcode;
	string operands;
	int length;
	bool i, n, p, b, e, x;
	bool literal;

	// default constructor
	instruction(string label = "", string opcode = "", string operands = "", int length = 3, bool i = 1, bool n = 1, bool p = 1, bool b = 0, int e = 0, int literal = 0, int x = 0)
	{
		this -> label = label;
		this -> opcode = opcode;
		this -> operands = operands;
		this -> length = length;
		this -> i = i;
		this -> n = n;
		this -> p = p;
		this -> b = b;
		this -> e = e;
		this -> x = x;
		this -> literal = literal;
	}

	// parse multiple operands
	vector<string> get_operands()
	{
		vector<string> m_operands;
		int i = 0;
		int l =  operands.length();
		while(i < l)
		{
			int j = i + 1;
			while(j < l && operands[j] != ',')
				j++;
			m_operands.push_back(operands.substr(i, j - i));
			i = j + 1;
		}
		return m_operands;
	}

	// work in the flags for opcode
	void format_opcode(int& op)
	{
		if(length == 3 || length == 4)
		{
			op |= i;
			op |= (n << 1);
		}
	}

	// work in the flags for operands
	void format_operand(int& operand)
	{
		if(length == 3 || length == 4)
		{
			// apply mask for negative operand
			int mask = 0;
			for(int i = 0; i < ((length == 3) ? 12 : 20); i++)
			{
				mask |= 1;
				mask = mask << 1;
			}
			mask = mask >> 1;
			operand = operand & mask;


			if(!e)
			{
				operand |= (p << 13);
				operand |= (b << 14);
				operand |= (x << 15);
			}
			else
			{
				operand |= (e << 20);
				operand |= (p << 21);
				operand |= (b << 22);
				operand |= (x << 23);
			}
		}
	}

	// whether an instruction is comment or not
	bool is_comment()
	{
		return (label == ".");
	}

	// whether an instruction has operands or not
	bool has_operands()
	{
		return (operands != "");
	}

	// whether an instruction has multiple operands or not
	bool has_multiple_operands()
	{
		return (operands.find(",") != string::npos);
	}


	// whether an instruction has label or not
	bool has_label()
	{
		return (label != "");
	}

	/* Debugging */
	void print()
	{
		cout << "-----------------------------------\n";
		cout << "label = " << label << '\n';
		cout << "opcode = " << opcode << '\n';
		cout << "operand = " << operands << '\n';
		cout << "length = " << length << '\n';
		cout << "i n b p e x = " << i << n << b << p << e << x << '\n';
		cout << "-----------------------------------\n";
	}

};

// takes a line from source code/intermediate code and tokenize the line
int process_line(string line, instruction& instr, bool input = true)
{
	string LOCCTR = "";

	int offset = (input) ? 0 : FIELD_LENGTH;

	// label
	instr.label = strip(line.substr(offset, FIELD_LENGTH));

	// extended format instruction
	if(line[offset + FIELD_LENGTH] == '+')
	{
		instr.e = 1;
		instr.p = 0;
		instr.b = 0;
		instr.n = 1;
		instr.i = 1;
	}

	// opcode
	instr.opcode = strip(line.substr(offset + FIELD_LENGTH + 1, FIELD_LENGTH));

	// operands
	instr.operands = strip(line.substr(offset + 2 * FIELD_LENGTH + 2, OPERAND_LENGTH));


	// operand types
	if(line[offset + 2 * FIELD_LENGTH + 1] != ' ')
	{
		char x = line[offset + 2 * FIELD_LENGTH + 1];
		// immediate
		if(x == '#')
		{
			instr.i = 1;
			instr.n = 0;
			if(is_number(instr.operands))
			{
				instr.p = 0;
				instr.b = 0;
			}
		}
		// indirect
		else if(x == '@')
		{
			instr.i = 0;
			instr.n = 1;
		}
		else if(x == '=')
		{
			instr.literal = true;
		}
	}


	// if reading intermediate file
	if(!input)
	{
		// location counter
		LOCCTR = strip(line.substr(0, FIELD_LENGTH));
		if(!LOCCTR.empty())
			return stoi(LOCCTR, nullptr, 16);
	}
	return -1;
}

void pass_1(string input)
{
	fstream fin, fout;
	
	// open input file
	fin.open(input, ios::in);

	if(!fin.is_open())
	{
		perror(input.c_str());
		exit(1);
	}

	// open intermediate file
	fout.open("intermediate.txt", ios::out);

	// some useful variables' initialisation
	int LOCCTR = 0, STADDR = 0, LENGTH;
	string CSECT = "";

	// reading input file
	while(fin.good())
	{
		string line;
		getline(fin, line);

		// end of file stop reading
		if(line.empty())
			break;

		// processing line into tokens
		instruction instr;
		process_line(line, instr);
		
		if(!instr.is_comment())
		{
			if(instr.opcode == "START" || instr.opcode == "CSECT")
			{
				// enter the previous CSECT in the SYMTAB
				if(!CSECT.empty())
				{
					LENGTH = LOCCTR - STADDR;
					SYMTAB.insert({SYMBOL(CSECT, CSECT), VALUE(STADDR, LENGTH)});
				}

				// initialize LOCCTR ,STADDR and CSECT
				if(instr.has_operands())
					STADDR = stoi(instr.operands, NULL, 16);
				else
					STADDR = 0;

				LOCCTR = STADDR;
				CSECT = instr.label;

				// initialize some symbols for the csection
				preprocess(CSECT);

				// write to intermediate file
				fout << format_string(format_number(LOCCTR, 4), FIELD_LENGTH) << line << '\n';
			}
			else if(instr.opcode == "END" || instr.opcode == "LTORG")
			{
				// write to intermediate file
				fout << format_string("", FIELD_LENGTH) << line << '\n';

				// Update LITTAB
				for(auto& x : LITTAB)
				{
					if(x.second.value == -1)
					{
						// write to intermediate file
						fout << format_string(format_number(LOCCTR, 4), FIELD_LENGTH) << format_string("*", FIELD_LENGTH) << "=" << format_string(x.first.name, FIELD_LENGTH) << " " << format_string("", OPERAND_LENGTH) << '\n';

						// Update literal address
						x.second.value = LOCCTR;

						// Update LOCCTR according to the literal type
						if(x.first.name.front() == 'C')
							LOCCTR += (x.first.name.length() - 3);
						else if(x.first.name.front() == 'X')
							LOCCTR += ((x.first.name.length() - 3)/2);
						else
							LOCCTR += 3;
					}
				}

				// enter the last CSECT in the SYMTAB
				if(instr.opcode == "END")
				{
					LENGTH = LOCCTR - STADDR;
					SYMTAB.insert({SYMBOL(CSECT, CSECT), VALUE(STADDR, LENGTH)});
				}
			}
			else if(instr.opcode == "BASE")
			{
				// write to intermediate file
				fout << format_string("", FIELD_LENGTH) << line << '\n';
			}
			else if(instr.opcode == "EQU")
			{
				if(instr.has_label())
				{
					if(SYMTAB.find(SYMBOL(CSECT, instr.label)) == SYMTAB.end())
					{
						if(instr.operands == "*")
						{
							SYMTAB.insert({SYMBOL(CSECT, instr.label), VALUE(LOCCTR)});

							// write to intermediate file
							fout << format_string(format_number(LOCCTR, 4), FIELD_LENGTH) << line << '\n';
						}
						else if(is_number(instr.operands)) // assumption that number is in decimal format only
						{
							SYMTAB.insert({SYMBOL(CSECT, instr.label), VALUE(stoi(instr.operands), 3, 0)});

							// write to intermediate file
							fout << format_string(format_number(stoi(instr.operands), 4), FIELD_LENGTH) << line << '\n';
						}
						else
						{
							// Evaluate the expression
							ExpEvaluator e(CSECT);
							int value = e.evaluate(instr.operands).second;
							int type = e.evaluate(instr.operands).first;
							if(type != 0 && type != -1 && type != 1)
							{
								perror("Invalid Expression");
								exit(1);
							}
							apply_mask(value, 16);


							// write to intermediate file
							fout << format_string(format_number(value, 4), FIELD_LENGTH) << line << '\n';
						}
					}
					else
					{
						perror("duplicate symbol");
						exit(1);
					}

				}
			}
			else if(instr.opcode == "EXTDEF" || instr.opcode == "EXTREF")
			{
				// write to intermediate file
				fout << format_string("", FIELD_LENGTH) << line << '\n';
			}
			else
			{
				// write to intermediate file
				fout << format_string(format_number(LOCCTR, 4), FIELD_LENGTH) << line << '\n';


				// Update LITTAB
				if(instr.literal)
				{
					if(LITTAB.find(SYMBOL(CSECT, instr.operands)) == LITTAB.end())	
					{
						LITTAB.insert({SYMBOL(CSECT, instr.operands), VALUE(-1)});
					}
				}

				// Update SYMTAB
				if(instr.has_label())
				{
					if(SYMTAB.find(SYMBOL(CSECT, instr.label)) == SYMTAB.end())
						SYMTAB.insert({SYMBOL(CSECT, instr.label), VALUE(LOCCTR)});
					else
					{
						perror("duplicate symbol");
						exit(1);
					}
				}

				// Update LOCCTR
				if(OPTAB(instr.opcode) != make_pair(-1, -1))
				{
					pair<int, int> format = OPTAB(instr.opcode);
					// extended instruction format
					if(instr.e)
						format.first++;

					instr.length = format.first;	
					LOCCTR += instr.length;
				}
				else if(instr.opcode == "WORD")
					LOCCTR += 3;
				else if(instr.opcode == "RESW")
					LOCCTR += 3 * stoi(instr.operands);
				else if(instr.opcode == "RESB")
					LOCCTR += stoi(instr.operands);
				else if(instr.opcode == "BYTE")
				{
					if(instr.operands.front() == 'C')
						LOCCTR += (instr.operands.length() - 3);
					if(instr.operands.front() == 'X')
						LOCCTR += ((instr.operands.length() - 3)/2);
				}	
				else
				{
					perror("Invalid Operation Code\n");
					exit(1);
				}	
			}
		}
		else
		{
			// write to intermediate file 
			fout << line << '\n';
		}
	}

	// closing files
	fin.close();
	fout.close();
}

void pass_2()
{
	fstream fin, fout, fout2;

	// open intermediate file
	fin.open("intermediate.txt", ios::in);

	if(!fin.is_open())
	{
		perror("intermediate.txt");
		exit(1);
	}

	// open output and listing file
	fout.open("output.txt", ios::out);
	fout2.open("listing.txt", ios::out);

	// useful variables' initialisation
	int LOCCTR = 0, BASE = 0, PC = 0, START = 0;
	string CSECT = "", PROGNAME = "", text = "";


	// for record storing
	map<string, vector<string>> text_list, modification_list;
	map<string, string> header_list, end_list, define_list, refer_list;
	vector<string> CSECTS;

	// reading intermediate file
	while(fin.good())
	{
		string line;
		getline(fin, line);

		// end of file
		if(line.empty())
			break;

		// processing line into tokens
		instruction instr;
		LOCCTR = process_line(line, instr, false); 

		if(!instr.is_comment())
		{
			if(instr.opcode == "START")
			{
				// write listing for the instruction
				fout2 << line << '\n';

				// csection name
				CSECT = instr.label;
				PROGNAME = CSECT;
				CSECTS.push_back(CSECT);


				// starting address for the csection
				int STADDR = 0;
				if(instr.has_operands())
					STADDR = stoi(instr.operands, NULL, 16);
				else
					STADDR = 0;

			
				// length of the csection
				int LENGTH = 0;
				if(SYMTAB.find(SYMBOL(CSECT, CSECT)) != SYMTAB.end())
					LENGTH = SYMTAB[SYMBOL(CSECT, CSECT)].length;

				// write the output machine code
				stringstream header;
				header << "H";
				header << format_string(CSECT, 6);
				header << format_number(STADDR, 6);
				header << format_number(LENGTH, 6);
				header_list.insert({CSECT, header.str()});
			}
			else if(instr.opcode == "END")
			{
				// write listing for the instruction
				fout2 << line << '\n';

				// write the output machine code
				stringstream end; 
				int first = SYMTAB[SYMBOL(CSECT, instr.operands)].value;
				end << "E";
				end_list.insert({CSECT, end.str()});

				end << format_number(first, 6);
				end_list[PROGNAME] =  end.str();
			}
			else if(instr.opcode == "CSECT")
			{
				// write listing for the instruction
				fout2 << line << '\n';

				// write the left over output machine code of the previous CSECT
				if(text.length())
				{
					int LENGTH = (text.length()) / 2;
					stringstream text_record;
					text_record << "T";
					text_record << format_number(START, 6);
					text_record << format_number(LENGTH, 2);
					text_record << text;

					text_list[CSECT].push_back(text_record.str());

					text = "";
					START = 0;
				}

				stringstream end;
				end << "E";
				end_list.insert({CSECT, end.str()});


				// starting address, length of the new CSECT
				int STADDR = 0;
				CSECT = instr.label;
				CSECTS.push_back(CSECT);

				int LENGTH = 0; 
				if(SYMTAB.find(SYMBOL(CSECT, CSECT)) != SYMTAB.end())
					LENGTH = SYMTAB[SYMBOL(CSECT, CSECT)].length;

				// write the output machine code
				stringstream header;
				header << "H";
				header << format_string(CSECT, 6);
				header << format_number(STADDR, 6);
				header << format_number(LENGTH, 6);
				header_list.insert({CSECT, header.str()});
			}
			else if(instr.opcode == "EXTDEF" || instr.opcode == "EXTREF")
			{
				// write listing for the instruction
				fout2 << line << '\n';

				// write the output machine code
				vector<string> operands = instr.get_operands();
				if(instr.opcode == "EXTDEF")
				{
					stringstream def;
					def << "D";
					for(string operand: operands)
					{
						def << format_string(operand, 6);
						int address = 0;
						if(SYMTAB.find(SYMBOL(CSECT, operand)) != SYMTAB.end())
							address = SYMTAB[SYMBOL(CSECT, operand)].value;
						def << format_number(address, 6); 
					}
					define_list.insert({CSECT, def.str()});
				}
				else
				{
					// clear EXTREF list and update according to new CSECT
					EXTREF.clear();
					for(auto x : operands)
						EXTREF.insert(x);

					
					stringstream refer;
					refer << "R";
					for(string operand : operands)
						refer << format_string(operand, 6);
					refer_list.insert({CSECT, refer.str()});
				}
			}
			else if(instr.opcode == "BASE")
			{
				// write listing for the instruction
				fout2 << line << '\n';

				// set BASE register location for base relative addressing
				if(SYMTAB.find(SYMBOL(CSECT, instr.operands)) != SYMTAB.end())
					BASE = SYMTAB[SYMBOL(CSECT, instr.operands)].value;
			}
			else if(instr.opcode == "LTORG")
			{
				// write listing for the instruction
				fout2 << line << '\n';
			}
			else if(instr.opcode == "EQU")
			{
				// write listing for the instruction
				fout2 << line << '\n';
			}
			else
			{
				string obcode = "";
				

				if(OPTAB(instr.opcode) != make_pair(-1, -1))
				{
					// generate op
					pair<int, int> format = OPTAB(instr.opcode);

					// extended operation instruction
					if(instr.e)
						format.first++;
					instr.length = format.first;

					int op = format.second;
					instr.format_opcode(op);
					
					// update program counter
					PC = LOCCTR + instr.length;
					
					// generate operandcode
					int operandcode = 0;
					if(instr.has_operands())
					{
						// format operands
						if(instr.has_multiple_operands())
						{
							vector<string> m_operands = instr.get_operands();
							// register-register instructions
							if(instr.length == 2)
							{
								for(string op : m_operands)
								{
									if(SYMTAB.find(SYMBOL(CSECT, op)) != SYMTAB.end())
									{
										int x = SYMTAB[SYMBOL(CSECT, op)].value;
										operandcode = operandcode << 4;
										operandcode += x;
									}
									else
									{
										perror("register not found");
										exit(1);
									}
								}	
							}
							// instructions of type BUFFER,X
							else
							{
								int x = 0;
								if(SYMTAB.find(SYMBOL(CSECT, m_operands.front())) != SYMTAB.end())
								{
									x = SYMTAB[SYMBOL(CSECT, m_operands.front())].value;
									operandcode += x;
									if(instr.p)
									{
										// decide between PC relative and base relative
										if(operandcode - PC >= -2048 && operandcode - PC <= 2047)
											operandcode -= PC;
										else if(operandcode - BASE >= 0 && operandcode - BASE <= 4095)
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											perror("can't fit PC or BASE");
											exit(1);
										}

									}

									if(instr.e)
									{
										stringstream modification;
										modification << "M";
										modification << format_number(LOCCTR + 1, 6);
										modification << "05+";
										modification << format_string(CSECT, 6); 
										modification_list[CSECT].push_back(modification.str());
									}

									
								}
								else
								{
									operandcode += x;
									if(EXTREF.find(m_operands.front()) != EXTREF.end())
									{
										stringstream modification;
										modification << "M";
										modification << format_number(LOCCTR + 1, 6);
										modification << "05+";
										modification << format_string(m_operands.front(), 6); 
										modification_list[CSECT].push_back(modification.str());
									}
								}
								instr.x = 1;
							}
						}
						else
						{
							// if instruction has literal value then get its address
							if(instr.literal)
							{
								if(LITTAB.find(SYMBOL(CSECT, instr.operands)) != LITTAB.end())
								{
									operandcode = LITTAB[SYMBOL(CSECT, instr.operands)].value;
									// decide between PC relative and BASE relative
									if(instr.p)
									{
										if(operandcode - PC >= -2048 && operandcode - PC <= 2047)
											operandcode -= PC;
										else if(operandcode - BASE >= 0 && operandcode - BASE <= 4095)
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											perror("can't fit PC or BASE");
											exit(1);
										}

									}
								}
								else
								{
									// symbol not found in literal table -> that should not happen 
									perror("my error");
									exit(1);
								}
							}	
							// if instruction has operand symbol then get its address
							else if(SYMTAB.find(SYMBOL(CSECT, instr.operands)) != SYMTAB.end())
							{
								operandcode = SYMTAB[SYMBOL(CSECT, instr.operands)].value;
								if(instr.length == 2)
								{
									operandcode = operandcode << 4;
								}
								else
								{
									if(instr.p)
									{
										// decide between PC relative and BASE relative
										if(operandcode - PC >= -2048 && operandcode - PC <= 2047)
											operandcode -= PC;
										else if(operandcode - BASE >= 0 && operandcode - BASE <= 4095)
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											perror("can't fit PC or BASE");
											exit(1);
										}
									}
									
									if(instr.e)
									{
										stringstream modification;
										modification << "M";
										modification << format_number(LOCCTR + 1, 6);
										modification << "05+";
										modification << format_string(CSECT, 6); 
										modification_list[CSECT].push_back(modification.str());
									}
								}
								
							}
							// if operand is immediate or external reference
							else
							{
								if(EXTREF.find(instr.operands) != EXTREF.end())
								{
									stringstream modification;
									modification << "M";
									modification << format_number(LOCCTR + 1, 6);
									modification << "05+";
									modification << format_string(instr.operands, 6); 
									modification_list[CSECT].push_back(modification.str());
								}
								if(instr.i)
								{
									if(is_number(instr.operands))
										operandcode = stoi(instr.operands);
									else
										;
								}

								
							}
						}

						instr.format_operand(operandcode);	
					}
					
					// writing listing for the instruction
					obcode = format_number(op, 2) + format_number(operandcode, 2 * (instr.length - 1));
					fout2 << line << format_string(obcode, 10) << '\n';	

				}
				else
				{
					// process constants
					if(instr.label == "*")
					{
						string constant = instr.opcode.substr(2, instr.opcode.length() - 3);
						if(instr.opcode.front() == 'X')
						{
							int x = stoi(constant, nullptr, 16);
							obcode = format_number(x, constant.length());
						}
						else if(instr.opcode.front() == 'C')
						{
							for(char ch : constant)
							{
								int x = ch;
								obcode += format_number(x, 2, false);
							}
						}
						else
						{
							int word = stoi(instr.operands);
							obcode = format_number(word, 6, false);
						}
					}

					if(instr.opcode == "WORD")
					{
						if(is_number(instr.operands))
						{
							int word = stoi(instr.operands);
							obcode = format_number(word, 6);
						}
						else
						{
							ExpEvaluator e(CSECT);
							int word = e.evaluate(instr.operands).second; 
							apply_mask(word, 24);
							obcode = format_number(word, 6);

							vector<string> m_operands = e.tokenize(instr.operands);
							for(int i = 0; i < (int)(m_operands.size()); i++)
							{
								if(EXTREF.find(m_operands[i]) != EXTREF.end())
								{
									stringstream modification;
									modification << "M";
									modification << format_number(LOCCTR, 6);
									modification << "06";
									if(i > 0 && m_operands[i-1] == "-")
										modification << "-";
									else
										modification << "+";
									modification << format_string(m_operands[i], 6); 
									modification_list[CSECT].push_back(modification.str());
								}
							}
						}
					}

					if(instr.opcode == "BYTE")
					{
						string constant = instr.operands.substr(2, instr.operands.length() - 3);
						if(instr.operands.front() == 'X')
						{
							int x = stoi(constant, nullptr, 16);
							obcode = format_number(x, constant.length());
						}

						
						if(instr.operands.front() == 'C')
						{
							for(char ch : constant)
							{
								int x = ch;
								obcode += format_number(x, 2, false);
							}
						}
					}

					// writing listing for the instruction
					fout2 << line << format_string(obcode, 10) << '\n';	

				}

				// write the output machine code
				if(text.length() == 0)
					START = LOCCTR;

				if(text.length() + obcode.length() <= 60 && instr.opcode != "RESW" && instr.opcode != "RESB")
					text += obcode;
				else
				{
					if(text.length())
					{
						int LENGTH = (text.length()) / 2;
						stringstream text_record;
						text_record << "T";
						text_record << format_number(START, 6);
						text_record << format_number(LENGTH, 2);
						text_record << text;

						text_list[CSECT].push_back(text_record.str());

						text = obcode;
						START = LOCCTR;
					}
				}
			}
		}
		else
			fout2 << line << '\n';
	}

	if(text.length())
	{
		int LENGTH = (text.length()) / 2;
		stringstream text_record;
		text_record << "T";
		text_record << format_number(START, 6);
		text_record << format_number(LENGTH, 2);
		text_record << text;

		text_list[CSECT].push_back(text_record.str());

		text = "";
		START = 0;
	}


	for(string CSECT: CSECTS)
	{
		fout << header_list[CSECT] << '\n';
		if(!define_list[CSECT].empty())
			fout << define_list[CSECT] << '\n';
		if(!refer_list[CSECT].empty())
			fout << refer_list[CSECT] << '\n';
		for(string text_record: text_list[CSECT])
			fout << text_record << '\n';
		for(string modification_record: modification_list[CSECT])
			fout << modification_record << '\n';
		fout << end_list[CSECT] << '\n';
		
	}
	
	// closing files
	fin.close();
	fout.close();
	fout2.close();
}

// Main function
int main(int argc, char **argv)
{
	// check that the input is provided in the proper format
	if(argc != 2)
	{
		cout << "Usage: ./a.out {source file}\n";
		return 0;
	}

	// run the  2-pass assembler
	string input = argv[1];
	pass_1(input);
	pass_2();

	return 0;
}

