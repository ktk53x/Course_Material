#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int gradePoint(char * grade)
{
    if(!strcmp(grade, "AS")) return 10;
    if(!strcmp(grade, "AA")) return 10;
    if(!strcmp(grade, "AB")) return 9;
    if(!strcmp(grade, "BB")) return 8;
    if(!strcmp(grade, "BC")) return 7;
    if(!strcmp(grade, "CC")) return 6;
    if(!strcmp(grade, "CD")) return 5;
    if(!strcmp(grade, "DD")) return 4;
}

int main()
{
    int studentsFd, gradesFd, coursesFd, outputFd, studentsRead, gradesRead, coursesRead;
    int read_count = 0, write_count = 0, lseek_count = 0;
    int outputFilePermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IXGRP;
    studentsFd = open("students03.csv", O_RDONLY);
    coursesFd = open("courses03.csv", O_RDONLY);

    char * studentsData, * coursesData;
    studentsData = (char *)malloc(4096);
    coursesData = (char *)malloc(8192);
    read(studentsFd, studentsData, 4096);read_count++;
    read(coursesFd, coursesData, 8192);read_count++;

    int ptr = 0;
    for(int i = 0; i < 99; i++)
    {
        double credits[8], SPI[8], CPI[8], cr[8];
        for(int j = 0; j < 8; j++)
        {
            credits[j] = 0.0;
            SPI[j] = 0.0;
            CPI[j] = 0.0;
            cr[j] = 0.0;
        }

        char name[31], roll[10], filename[14];
        for(int j = 0; j < 30; j++)
        {
            name[j] = studentsData[ptr++];
        }
        name[30] = '\0';
        ptr++;
        for(int j = 0; j < 9; j++)
            roll[j] = studentsData[ptr++];
        roll[9] = '\0';
        ptr++;


        strcpy(filename, roll);
        strcat(filename, ".txt");

        outputFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, outputFilePermissions);
        write(outputFd, "Name: ", strlen("Name: "));write_count++;
        write(outputFd, name, strlen(name));write_count++;
        write(outputFd, "\nRoll Number: ", strlen("\nRoll Number: "));write_count++;
        write(outputFd, roll, strlen(roll));write_count++;

        gradesFd = open("grades03.csv", O_RDONLY);
        char *gradesLine;
        gradesLine = (char *)malloc(20);

        lseek(gradesFd, i * 20, SEEK_SET);lseek_count++;
        int coursesptr = 0;
        for(int j = 0; j < 63; j++)
        {
            char course[7], grade[3];
            int gradeptr = 0;
            read(gradesFd, gradesLine, 20);read_count++;
            for(int k = 0; k < 10; k++)
                gradeptr++;
            for(int k = 0; k < 6; k++)
                course[k] = gradesLine[gradeptr++];
            gradeptr++;
            for(int k = 0; k < 2; k++)
                grade[k] = gradesLine[gradeptr++];
            course[6] = '\0';
            grade[2] = '\0';


            char sem, courseName[49], C[3];
            sem = coursesData[coursesptr++];
            for(int k = 0; k < 8; k++)
                coursesptr++;
            for(int k = 0; k < 48; k++)
                courseName[k] = coursesData[coursesptr++];
            courseName[48] = '\0';
            for(int k = 0; k < 8; k++)
                coursesptr++;
            for(int k = 0; k < 2; k++)
                C[k] = coursesData[coursesptr++];
            coursesptr++;


            if(j == 0)
            {
                write(outputFd, "\nSemester I\n", strlen("\nSemester I\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 8)
            {
                write(outputFd, "\nSemester II\n", strlen("\nSemester II\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 17)
            {
                write(outputFd, "\nSemester III\n", strlen("\nSemester III\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 26)
            {
                write(outputFd, "\nSemester IV\n", strlen("\nSemester IV\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 35)
            {
                write(outputFd, "\nSemester V\n", strlen("\nSemester V\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 44)
            {
                write(outputFd, "\nSemester VI\n", strlen("\nSemester VI\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j  == 52)
            {
                write(outputFd, "\nSemester VII\n", strlen("\nSemester VII\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 58)
            {
                write(outputFd, "\nSemester VIII\n", strlen("\nSemester VIII\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            write(outputFd, "\n", 1);write_count++;
            write(outputFd, course, strlen(course));write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, courseName, strlen(courseName));write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, C, 2);write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, grade, strlen(grade));write_count++;
            lseek(gradesFd, 157 * 20, SEEK_CUR);lseek_count++;

            credits[sem - '1'] = credits[sem - '1'] + (stoi(C));
            SPI[sem - '1'] = SPI[sem - '1'] + (double)(stoi(C)) * (gradePoint(grade)) / 10;

        }
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "Semester I II III IV V VI VII VIII", strlen("Semester I II III IV V VI VII VIII"));write_count++;
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "SPI  ", strlen("SPI  "));write_count++;
        for(int j = 0; j < 8; j++)
        {
            float x = (SPI[j] / credits[j]) * 10;
            char s[6];
            sprintf(s, "%.2f ", x);
            write(outputFd, s, strlen(s));write_count++;
        }
        CPI[0] = SPI[0];
        cr[0] = credits[0];
        for(int j = 1; j < 8; j++)
        {
            CPI[j] = CPI[j-1] + SPI[j];
            cr[j]  = cr[j-1] + credits[j];
        }
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "CPI  ", strlen("CPI  "));write_count++;

        
        for(int j = 0; j < 8; j++)
        {
            float x = (CPI[j] / cr[j]) * 10;
            char s[6];
            sprintf(s, "%.2f ", x);
            write(outputFd, s, strlen(s));write_count++;
        }
        free(gradesLine);
        close(gradesFd);
        close(outputFd);
    }
    close(studentsFd);
    free(studentsData);
    
    
    
    
    
    studentsData = (char *)malloc(4096);
    studentsFd = open("students03.csv", O_RDONLY);
    lseek(studentsFd, 41 * 99, SEEK_CUR);lseek_count++;
    read(studentsFd, studentsData, 4096);read_count++;
    ptr = 0;
    for(int i = 0; i < 62; i++)
    {
        double credits[8], SPI[8], CPI[8], cr[8];
        for(int j = 0; j < 8; j++)
        {
            credits[j] = 0.0;
            SPI[j] = 0.0;
            CPI[j] = 0.0;
            cr[j] = 0.0;
        }

        char name[31], roll[10], filename[14];
        for(int j = 0; j < 30; j++)
        {
            name[j] = studentsData[ptr++];
        }
        name[30] = '\0';
        ptr++;
        for(int j = 0; j < 9; j++)
            roll[j] = studentsData[ptr++];
        roll[9] = '\0';
        ptr++;

        strcpy(filename, roll);
        strcat(filename, ".txt");

        outputFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, outputFilePermissions);
        write(outputFd, "Name: ", strlen("Name: "));write_count++;
        write(outputFd, name, strlen(name));write_count++;
        write(outputFd, "\nRoll Number: ", strlen("\nRoll Number: "));write_count++;
        write(outputFd, roll, strlen(roll));write_count++;

        gradesFd = open("grades03.csv", O_RDONLY);
        char *gradesLine;
        gradesLine = (char *)malloc(20);

        lseek(gradesFd, (99 + i) * 20, SEEK_SET);lseek_count++;
        int coursesptr = 0;
        for(int j = 0; j < 63; j++)
        {
            char course[7], grade[3];
            int gradeptr = 0;
            read(gradesFd, gradesLine, 20);read_count++;
            for(int k = 0; k < 10; k++)
                gradeptr++;
            for(int k = 0; k < 6; k++)
                course[k] = gradesLine[gradeptr++];
            gradeptr++;
            for(int k = 0; k < 2; k++)
                grade[k] = gradesLine[gradeptr++];
            course[6] = '\0';
            grade[2] = '\0';


            char sem, courseName[49], C[3];
            sem = coursesData[coursesptr++];
            for(int k = 0; k < 8; k++)
                coursesptr++;
            for(int k = 0; k < 48; k++)
                courseName[k] = coursesData[coursesptr++];
            courseName[48] = '\0';
            for(int k = 0; k < 8; k++)
                coursesptr++;
            for(int k = 0; k < 2; k++)
                C[k] = coursesData[coursesptr++];
            coursesptr++;


            if(j == 0)
            {
                write(outputFd, "\nSemester I\n", strlen("\nSemester I\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 8)
            {
                write(outputFd, "\nSemester II\n", strlen("\nSemester II\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 17)
            {
                write(outputFd, "\nSemester III\n", strlen("\nSemester III\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 26)
            {
                write(outputFd, "\nSemester IV\n", strlen("\nSemester IV\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 35)
            {
                write(outputFd, "\nSemester V\n", strlen("\nSemester V\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 44)
            {
                write(outputFd, "\nSemester VI\n", strlen("\nSemester VI\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j  == 52)
            {
                write(outputFd, "\nSemester VII\n", strlen("\nSemester VII\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            if(j == 58)
            {
                write(outputFd, "\nSemester VIII\n", strlen("\nSemester VIII\n"));write_count++;
                write(outputFd, "Course Number | Course Name | C | Grade", strlen("Course Number | Course Name | C | Grade"));write_count++;
            }
            write(outputFd, "\n", 1);write_count++;
            write(outputFd, course, strlen(course));write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, courseName, strlen(courseName));write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, C, 2);write_count++;
            write(outputFd, " ", 1);write_count++;
            write(outputFd, grade, strlen(grade));write_count++;
            lseek(gradesFd, 157 * 20, SEEK_CUR);lseek_count++;

            credits[sem - '1'] = credits[sem - '1'] + (stoi(C));
            SPI[sem - '1'] = SPI[sem - '1'] + (double)(stoi(C)) * (gradePoint(grade)) / 10;

        }
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "Semester I II III IV V VI VII VIII", strlen("Semester I II III IV V VI VII VIII"));write_count++;
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "SPI  ", strlen("SPI  "));write_count++;
        for(int j = 0; j < 8; j++)
        {
            float x = (SPI[j] / credits[j]) * 10;
            char s[6];
            sprintf(s, "%.2f ", x);
            write(outputFd, s, strlen(s));write_count++;
        }
        CPI[0] = SPI[0];
        cr[0] = credits[0];
        for(int j = 1; j < 8; j++)
        {
            CPI[j] = CPI[j-1] + SPI[j];
            cr[j]  = cr[j-1] + credits[j];
        }
        write(outputFd, "\n", 1);write_count++;
        write(outputFd, "CPI  ", strlen("CPI  "));write_count++;

        
        for(int j = 0; j < 8; j++)
        {
            float x = (CPI[j] / cr[j]) * 10;
            char s[6];
            sprintf(s, "%.2f ", x);
            write(outputFd, s, strlen(s));write_count++;
        }
        free(gradesLine);
        close(gradesFd);
        close(outputFd);
    }
    fdatasync(outputFd);
    close(studentsFd);
    free(studentsData);
    free(coursesData);
    close(coursesFd);
    printf("%d %d %d\n", read_count, write_count, lseek_count);
}