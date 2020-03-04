#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <map>

using namespace std;


int main()
{
    int coursesFd, examFd, duplicateFd, noDuplicateFd;
    int outputFilePermissions = S_IRUSR | S_IWUSR;
    examFd = open("exam-tt-with-duplicates.csv", O_RDONLY);
    coursesFd = open("courses04.csv", O_RDONLY);
    map<string, string> courses;
    map<string, int> f;
    for(int i = 0; i < 354; i++)
    {
        string code, name;
        char code_c[7], name_c[60], temp;
        read(coursesFd, code_c, 6);
        read(coursesFd, &temp, 1);
        read(coursesFd, name_c, 59);
        read(coursesFd, &temp, 1);
        code_c[6] = '\0';
        name_c[59] = '\0';
        code = code_c;
        name = name_c;
        courses[name] = code;
        // cout << code << ' ' << name << '\n';
    }
    for(int i = 0; i < 1062; i++)
    {
        string name, end_date, end_time, mid_date, mid_time;
        char name_c[60], end_date_c[12], end_time_c[12], mid_date_c[12], mid_time_c[12], temp;
        read(examFd, name_c, 59);
        read(examFd, &temp, 1);
        read(examFd, end_date_c, 11);
        read(examFd, &temp, 1);
        read(examFd, end_time_c, 11);
        read(examFd, &temp, 1);
        read(examFd, mid_date_c, 11);
        read(examFd, &temp, 1);
        read(examFd, mid_time_c, 11);
        read(examFd, &temp, 1);
        name_c[59] = '\0';
        end_date_c[11] = '\0';
        end_time_c[11] = '\0';
        mid_date_c[11] = '\0';
        mid_time_c[11] = '\0';
        name = name_c;
        f[courses[name]]++;
        // cout << name << '\n';
    }
    duplicateFd = open("with-duplicates.txt", O_WRONLY | O_CREAT | O_TRUNC, outputFilePermissions);
    noDuplicateFd = open("with-no-duplicates.txt", O_WRONLY | O_CREAT | O_TRUNC, outputFilePermissions);

    write(duplicateFd, "cid   frequency\n", strlen("cid   frequency\n"));
    write(noDuplicateFd, "cid   frequency\n", strlen("cid   frequency\n"));
    for(auto x : f)
    {
        char line[80];
        sprintf(line, "%s %d\n", x.first.c_str(), x.second);
        if(x.second == 1)
            write(noDuplicateFd, line, strlen(line));
        else 
            write(duplicateFd, line, strlen(line));

    }
    close(duplicateFd);
    close(noDuplicateFd);
    close(examFd);
    close(coursesFd);
    return 0;
}