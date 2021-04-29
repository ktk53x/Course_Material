#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<set>
#include<map>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

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


int PROGADDR;
int LAST;
vector<string> memory((1 << 18), "xx");

map<string,int> ESTAB;

vector<pair<string,int>> get_symbols(string record)
{
	vector<pair<string, int>> symbols;
	int n = record.length();
	for(int i = 1; i < n; i += 12)
	{
		string symbol = record.substr(i, 6);
		int value = stoi(record.substr(i + 6, 6), nullptr, 16);
		symbols.push_back(make_pair(symbol, value));
	}
	return symbols; 
}

void pass_1(string input)
{
	fstream fin;
	
	// open input file
	fin.open(input, ios::in);

	if(!fin.is_open())
	{
		perror(input.c_str());
		exit(1);
	}

	// Take program address as input from the user in hex
	string progaddr;
	cout << "Enter PROGADDR for the program: ";
	cin >> progaddr;
	PROGADDR = stoi(progaddr, nullptr, 16);

	int CSADDR = PROGADDR;
	int CSLTH = 0;
	string record;

	while(fin.good())
	{
		// read record
		getline(fin, record);

		// end of input
		if(record.empty())
			break;

		if(record.front() == 'H')
		{
			// Update CSADDR
			CSADDR = CSADDR + CSLTH;

			// CSECT and CSLTH from the record
			string CSECT = record.substr(1, 6);
			CSLTH = stoi(record.substr(13, 6), nullptr, 16);

			// Enter the CSECT to ESTAB
			if(ESTAB.find(CSECT) == ESTAB.end())
				ESTAB.insert({CSECT, CSADDR});
			else
			{
				perror("duplicate external symbol");
				exit(1);
			}
		}

		if(record.front() == 'D')
		{
			// get the symbols in the record
			for(pair<string,int> symbol : get_symbols(record))
			{
				if(ESTAB.find(symbol.first) == ESTAB.end())
					ESTAB.insert({symbol.first, symbol.second + CSADDR});
				else
				{
					perror("duplicate external symbol");
					exit(1);
				}
			}
		}

	}
	LAST = CSADDR + CSLTH;
}

void pass_2(string input)
{
	fstream fin;
	
	// open input file
	fin.open(input, ios::in);

	if(!fin.is_open())
	{
		perror(input.c_str());
		exit(1);
	}


	int CSADDR = PROGADDR;
	int EXECADDR = PROGADDR;
	int CSLTH = 0;

	string record;
	while(fin.good())
	{
		getline(fin, record);

		// if end of input
		if(record.empty())
			break;

		if(record.front() == 'H')
			CSLTH = stoi(record.substr(13, 6), nullptr, 16);
		
		if(record.front() == 'T')
		{
			// move the record to its appropriate memory location byte by byte
			int STADDR = stoi(record.substr(1, 6), nullptr, 16) + CSADDR;
			int INDEX = 0;
			for(int i = 9; i < (int)record.length(); i += 2)
				memory[STADDR + INDEX++] = record.substr(i, 2); 
		}

		else if(record.front() == 'M')
		{
			string symbol = record.substr(10, 6);
			if(ESTAB.find(symbol) != ESTAB.end())
			{
				// extract address to be modified
				int address = stoi(record.substr(1, 6), nullptr, 16) + CSADDR;
				int length = stoi(record.substr(7, 2), nullptr, 16);

				char halfByte;
				if(length % 2)
					halfByte = memory[address][0];

				string val = "";
				for(int i = 0; i < (length + 1)/ 2; i++)
					val += memory[address + i];
				int value = stoi(val, nullptr, 16);

				cout << "value          = " << val << '\n';
				
				// apply modification
				int modification = ESTAB[symbol];
				char sign = record[9];
				if(sign == '+')
					value += modification;
				else
					value -= modification;

				// apply mask	
				int mask = 0;
				for(int i = 0; i < (length + length % 2) * 4; i++)
				{
					mask |= 1;
					mask = mask << 1;
				}
				mask = mask >> 1;
				value = value & mask;
				
				// write back the modified value
				val = format_number(value, length + (length % 2));
				cout << "modification   = " << format_number(modification, 6) << '\n';
				cout << "modified value = " << val << '\n';
				for(int i = 0; i < length; i += 2)
					memory[address + i / 2] = val.substr(i, 2);

				if(length % 2)
					memory[address][0] = halfByte;

			}
			else
			{
				cout << symbol << '\n';
				perror("undefined symbol");
				exit(1);
			}
		}

		if(record.front() == 'E')
		{
			if(record != "E")
			{
				int FIRST = stoi(record.substr(1, 6), nullptr, 16);
				EXECADDR = CSADDR + FIRST;
			}
			CSADDR = CSLTH + CSADDR;
		}

	}
	
	cout << "Starting execution at: " << format_number(EXECADDR, 4) << '\n';

}

void pretty_print_memory()
{
	fstream fout;
	// open memory file
	fout.open("memory.txt", ios::out);
	int i = (PROGADDR / 16) * 16;
	int n = ((LAST + 16) / 16) * 16;
	while(i < n) 
	{
		fout << format_number(i, 4) << ' ';

		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
				fout << memory[i++];
			fout << ' ';
		}
		fout << '\n';
	}

	// close the memory file
	fout.close();
}

void line_by_line_print_memory()
{
	fstream fout, fin;
	// open line by line file
	fout.open("line_by_line.txt", ios::out);


	// open listing file
	fin.open("listing.txt", ios::in);

	if(!fin.is_open())
	{
		perror("listing.txt");
		exit(1);
	}

	string line;
	vector<string> instructions;
	while(fin.good())
	{
		getline(fin, line);
		if(line.empty())
			break;
		
		string obcode = strip(line.substr(62, 10));
		if(!obcode.empty())
			instructions.push_back(obcode);
	}

	vector<string> answer, address;

	int j = 0, i = 0;
	while(i < (int)(memory.size()))
	{
		if(memory[i] != "xx")
		{
			string temp = "";
			address.push_back(format_number(i, 6));
			int size = instructions[j].length() / 2;
			for(int k = i; k < i + size; k++)
				temp += memory[k];
			answer.push_back(temp);
			j++; 
			i += size;
			
		}
		else
			i++;

	}	
	fout << format_string("address", 10) << format_string("instruction", 10) << '\n';
	for(int i = 0; i < (int)answer.size(); i++)
	{
		fout << format_string(address[i], 10) << format_string(answer[i], 10) << '\n';
	}

	// close the files
	fout.close();
	fin.close();
}
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
	for(auto x : ESTAB)
		cout << x.first << ' ' << format_number(x.second, 4) << '\n';
	pass_2(input);

	pretty_print_memory();
	line_by_line_print_memory();
	return 0;
}
