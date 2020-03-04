#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// For open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// for read, write and close
#include <unistd.h>

char * grades_file;
char * courses_file;
char * students_file;
char lines[240][80];
char write_data[6000];

int grade_point(char * g)
{
    if(!strcmp(g, "AS"))
        return 10;
    if(!strcmp(g, "AA"))
        return 10;
    if(!strcmp(g, "AB"))
        return 9;
    if(!strcmp(g, "BB"))
        return 8;
    if(!strcmp(g, "BC"))
        return 7;
    if(!strcmp(g, "CC"))
        return 6;
    if(!strcmp(g, "CD"))
        return 5;
    if(!strcmp(g, "DD"))
        return 4;
}

struct student
{
    char * name, * roll_no;
};

struct grade
{
    char * roll_no, * course_id, *g;
};

struct course
{
    char * sem, * course_id, * course_name, * L, * T, * P, * C;
};


struct student students[5000];
struct grade grades[10000];
struct course courses[3000];

int main()
{
    int input_fd, output_fd, read_status, grades_size, courses_size, students_size;
    students_file = (char *)malloc(5000);
    grades_file = (char *)malloc(200000);
    courses_file = (char *)malloc(3000);

    mode_t file_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IXGRP;
    input_fd = open("grades02.csv", O_RDONLY);
    grades_size = read(input_fd, grades_file, 200000);
    close(input_fd);

    input_fd = open("students02.csv",O_RDONLY);
    students_size = read(input_fd, students_file, 5000);
    close(input_fd);
    
    input_fd = open("courses02.csv",O_RDONLY);
    courses_size = read(input_fd, courses_file, 3000);
    close(input_fd);

    printf("grades = %d, students = %d, courses = %d\n", grades_size, students_size, courses_size);
    
    
    char delim[] = ",\n";
    char * token = strtok(students_file, delim);
    int i = 0, j = 0, count = 0;
    while (token != NULL) 
    {
        if(count % 2 == 0)
            students[i].name = token;
        else 
            students[i++].roll_no = token;
        token = strtok(NULL, delim);
        count++;
    }
    students_size = i;

    token = strtok(grades_file, delim);
    i = 0, j = 0, count = 0;
    while (token != NULL) 
    {
        if(count % 3 == 0)
            grades[i].roll_no = token;
        else if(count % 3 == 1)
        {
            grades[i].course_id = token;
            char temp[6];
            if((grades[i].course_id)[5] == ' ')
            {
                for(int t = 0; t < 5; t++)
                {
                    temp[t] = (grades[i].course_id)[t];
                }
                temp[5] ='\0';
                strcpy(grades[i].course_id,temp);  
                printf("%s\n", grades[i].course_id); 
            }
        }
        else
            grades[i++].g = token;
        token = strtok(NULL, delim);
        count++;
    }
    grades_size = i;
    

    token = strtok(courses_file, delim);
    i = 0, j = 0, count = 0;
    while (token != NULL) 
    {
        if(count % 7 == 0)
            courses[i].sem = token;
        else if(count % 7 == 1)
            courses[i].course_id = token;
        else if(count % 7 == 2)
            courses[i].course_name = token;
        else if(count % 7 == 3)
            courses[i].L = token;
        else if(count % 7 == 4)
            courses[i].T = token;
        else if(count % 7 == 5)
            courses[i].P = token;
        else if(count % 7 == 6)
            courses[i++].C = token;
        
        token = strtok(NULL, delim);
        count++;
    }
    courses_size = i;

    char * semester[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
    char * se[] = {"1", "2", "3", "4", "5", "6", "7", "8"};

    
    for(int i = 0; i < students_size; i++)
    {
        int cur_line = 0;
        sprintf(lines[cur_line++], "Name:%s\nRoll Number:%s\n", students[i].name, students[i].roll_no);
        double SPI[8], CPI[8], credits[8];
        for(int t = 0; t < 8; t++)
        {
            SPI[t] = 0.0;
            CPI[t] = 0.0;
            credits[t] = 0.0;
        }
        for(int j = 0; j < 8; j++)
        {
            sprintf(lines[cur_line++], "Semester %s\n", semester[j]);
            sprintf(lines[cur_line++], "Course Number Course Name Grade\n");
            for(int k = 0; k < grades_size; k++)
            {
                if(!strcmp(grades[k].roll_no, students[i].roll_no))
                {
                    for(int l = 0; l < courses_size; l++)
                    {
                        if(!strcmp(courses[l].course_id, grades[k].course_id) && !strcmp(se[j], courses[l].sem))
                        {
                            sprintf(lines[cur_line++],"%s %s %s %s\n", courses[l].course_id, courses[l].course_name,  courses[l].C, grades[k].g);
                            credits[j] = credits[j] + atoi(courses[l].C);
                            SPI[j] = SPI[j] + ((double)grade_point(grades[k].g) / 10.0) * atoi(courses[l].C);
                        }
                    }
                }

            }
        }
        for(int j = 0; j < 8; j++)
        {
            SPI[j]  = (SPI[j] / credits[j]) * 10;
        }
        sprintf(lines[cur_line++], "SEM  : %s %s %s %s %s %s %s %s\n", se[0], se[1], se[2], se[3], se[4], se[5], se[6], se[7]);
        sprintf(lines[cur_line++], "SPI : %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n", SPI[0], SPI[1], SPI[2], SPI[3], SPI[4], SPI[5], SPI[6], SPI[7]);
        CPI[0] = SPI[0];
        double temp = credits[0];
        for(int j = 1; j < 8; j++)
        {
            CPI[j] = (CPI[j - 1] * temp + SPI[j] * credits[j]) /  (temp + credits[j]);
            temp += credits[j];
        }
        sprintf(lines[cur_line++], "CPI : %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n", CPI[0], CPI[1], CPI[2], CPI[3], CPI[4], CPI[5], CPI[6], CPI[7]);
        strcpy(write_data, "");
        for(int k = 0; k < cur_line; k++)
            strcat(write_data, lines[k]);
        char output_file[14];
        strcpy(output_file, students[i].roll_no);
        strcat(output_file, ".txt");
        output_fd = creat(output_file, file_perms);
        close(output_fd);
        output_fd = open(output_file ,O_WRONLY, file_perms);
        write(output_fd, write_data, strlen(write_data));
        printf("%s\n", write_data);
    }
    free(students_file);
    free(courses_file);
    free(grades_file);
    return 0;
}