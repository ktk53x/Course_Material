#ifndef READINPUT_H
#define READINPUT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Relation.h"

using namespace std;

class ReadInput
{
	private:
		ifstream inputFile;

	public:
		// open the file
		ReadInput(string file);

		// close the file
		~ReadInput();

		//read input
		Relation<double> read();
};

#endif
