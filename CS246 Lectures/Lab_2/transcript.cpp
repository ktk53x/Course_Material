#include<iostream>
#include<fstream>
#include<map>
#include<iomanip>
#include<vector>


using namespace std;

struct course
{
    int sem;
    int L, T, P, C;
    string title;
};

map<string, string> students;
map<string, course> courses;
map<string, map<string, string>> grades;
map<string, int> gtoi;

int main()
{
    fstream fin, fout;
    fin.open("students.csv", ios::in);
    

    while(fin.good())
    {
        string roll_no, name;
        getline(fin, name, ',');
        getline(fin, roll_no, '\n');
        if(!name.empty())
            students[roll_no] = name;
    }
    fin.close();
    fin.open("courses.csv", ios::in);
    while(fin.good())
    {
        course temp;
        string sem, id, L, T, P, C;
        getline(fin, sem, ',');
        getline(fin, id, ',');
        getline(fin, temp.title, ',');
        getline(fin, L, ',');
        getline(fin, T, ',');
        getline(fin, P, ',');
        getline(fin, C, '\n');
        try
        {
            temp.sem = stoi(sem);
            temp.L = stoi(L);
            temp.T = stoi(T);
            temp.P = stoi(P);
            temp.C = stoi(C);
            if(!id.empty())
                courses[id] = temp;
        }
        catch(const exception& e)
        {
            break;
            std::cerr << e.what() << '\n';
        }
    }
    fin.close();
    fin.open("grades.csv", ios::in);
    while(fin.good())
    {
        string roll, id, g;
        getline(fin, roll, ',');
        getline(fin, id, ',');
        getline(fin, g, '\n');
        try
        {
            if(!roll.empty())
            {
                grades[roll][id] = g;
            }
        }
        catch(const exception& e)
        {
            break;
            std::cerr << e.what() << '\n';
        }
    }
    fin.close();

    vector<string> semester = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
    gtoi["AS"] = 10;
    gtoi["AA"] = 10;
    gtoi["AB"] = 9;
    gtoi["BB"] = 8;
    gtoi["BC"] = 7;
    gtoi["CC"] = 6;
    gtoi["CD"] = 5;
    gtoi["DD"] = 4;
    for(auto student : students)
    {
        fout.open(student.first + ".txt", ios::out);
        fout << "Name:" << student.second << endl;
        fout << "Roll Number:" << student.first << endl;
        vector<double> SPI(8, 0), CPI(8, 0), credits(8, 0);
        for(int i = 0; i < 8; i++)
        {
            fout << "Semester " << semester[i] << endl;
            fout << left << setw(20) << "Course Number\t|" << setw(50) << "Course Name                                     |" << setw(20) << "C |" << '\t' << "Grade" << endl;
            for(auto grade : grades[student.first])
            {
                string id;
                if((grade.first)[5] == 'M')
                {
                    id = grade.first;
                }
                else 
                    id = grade.first.substr(0, 5);
                
                if(courses[id].sem == i + 1)
                {
                    fout << left << setw(20) << id << setw(50) << courses[id].title << '\t' << setw(20) << courses[id].C << '\t' << grade.second << endl;
                    credits[i] += courses[id].C;
                    SPI[i] += ((double(gtoi[grade.second]) / 10) * courses[id].C);
                }
            }
            fout << endl;

        }
        fout << '\t';
        for(int i = 0; i < 8; i++)
            fout << left << setw(4) << semester[i] << '\t';
        fout << endl;
        fout << "SPI" << '\t';
        for(int i = 0; i < 8; i++)
        {
            SPI[i] = (SPI[i] / credits[i]) * 10;
            fout << left << setw(4) << setprecision(3) << SPI[i] << '\t';
        }
        fout << endl;
        fout << "CPI" << '\t' << left << setw(4) << setprecision(3) << SPI[0] << '\t';
        CPI[0] = SPI[0];
        double temp = credits[0];
        for(int i = 1; i < 8; i++)
        {
            CPI[i] = (CPI[i - 1] * temp + SPI[i] * credits[i]) /  (temp + credits[i]);
            temp += credits[i];
            fout << left << setw(4) << setprecision(3) << CPI[i] << '\t';
        }
        fout << endl;
        fout.close();
    }
    return 0;
}