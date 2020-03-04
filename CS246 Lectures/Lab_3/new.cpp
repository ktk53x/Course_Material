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

struct entry
{
    string name;
    string course_name;
    string grade;

    bool operator < (const entry& b)
    {
        if(this -> name == b.name)
        {
            if(this -> course_name == b.course_name)
            {
                return this -> grade < b.name;
            }
            return this -> course_name > b.course_name;
        }
        return this -> name < b.name;
    }
};

map<string, string> students;
map<string, course> courses;
map<string, map<string, string>> grades;
vector<entry> sorted_entry;

int partition (int low, int high)  
{  
    entry pivot = sorted_entry[high];
    int i = (low - 1); 
    for (int j = low; j <= high - 1; j++)  
    {  
        if (sorted_entry[j] < pivot)  
        {  
            i++;  
            swap(sorted_entry[i], sorted_entry[j]);  
        }  
    }  
    swap(sorted_entry[i + 1], sorted_entry[high]);  
    return (i + 1);  
}  

void quickSort(int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(low, high); 
        quickSort(low, pi - 1);  
        quickSort(pi + 1, high);  
    }  
}

int main()
{
    fstream fin, fout;
    fin.open("students01.csv", ios::in);
    
    while(fin.good())
    {
        string roll_no, name;
        getline(fin, name, ',');
        getline(fin, roll_no, '\n');
        if(!name.empty())
            students[roll_no] = name;
    }
    fin.close();
    
    fin.open("courses01.csv", ios::in);
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
    
    fin.open("grades01.csv", ios::in);
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
                sorted_entry.push_back({students[roll], courses[id].title, g});                
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

    quickSort(0, sorted_entry.size() - 1);

    fout.open("grades-sorted-Q.csv", ios::out);
    for(auto x : sorted_entry)
        fout << x.name << ',' << x.course_name << ',' << x.grade << '\n';
    fout.close();
}
