#include "ReadInput.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Relation.h"

using namespace std;

std::string trim(string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

ReadInput::ReadInput(string file) {
    inputFile.open(file, ios::in);
}

ReadInput::~ReadInput() {
    inputFile.close();
}

Relation<double> ReadInput::read() {
    Relation<double> data;
    vector<double> row;
    string line;

    while (inputFile) {
        //read a line
        getline(inputFile, line);

        //trim left and right
        line = trim(line);

        if (line == "" || line == "\n")
            continue;

        //devide into different words
        istringstream ss(line);

        //convert each word to float and add to row
        string word;
        while (ss >> word) {
            row.push_back(stod(word));
        }
        //add to data
        data.push_back(row);
        row.clear();
    }
    return data;
}
