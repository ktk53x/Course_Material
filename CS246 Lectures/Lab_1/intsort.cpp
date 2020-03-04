#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;
int a[6120001], L[6120001], R[6120001];

void merge(int a[], int l, int m, int r)
{
    int n1 = m - l + 1, n2 = r - m;
    for(int i = 0; i < n1; i++)
        L[i] = a[l + i];
    for(int i = 0; i < n2; i++)
        R[i] = a[m + 1 + i];
    int i = 0, j = 0, k = l;
    while(i < n1 && j < n2)
    {
        if(L[i] <= R[j])
            a[k++] = L[i++];
        else 
            a[k++] = R[j++];
    }
    while(i < n1)
    {
        a[k++] = L[i++];
    }
    while(j < n2)
    {
        a[k++] = R[j++];
    }
}

void merge_sort(int a[], int l, int r)
{
    if(l < r)
    {
        int m = (l + r) / 2;
        merge_sort(a, l, m);
        merge_sort(a, m + 1, r);

        merge(a, l, m, r);
    }
}

void swap(int* a, int* b)  
{  
    int t = *a;  
    *a = *b;  
    *b = t;  
} 

int partition (int arr[], int low, int high)  
{  
    int pivot = arr[high];
    int i = (low - 1); 
    for (int j = low; j <= high - 1; j++)  
    {  
        if (arr[j] < pivot)  
        {  
            i++;  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  

void quickSort(int arr[], int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(arr, low, high); 
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}

int main()
{
    int x = 10;
    for(int i = 1; i <= 10; i++)
    {
        FILE * f_int;
        string file = "i" + to_string(x) + "k.txt";
        f_int = fopen(file.c_str(), "w");
        srand(time(0));
        for(int i = 0; i < 1000 * x; i++)
            fprintf(f_int, "%d\n", rand() % 1000000);
        fclose(f_int);
        x *= 2;
    }
    x = 10;
    for(int i = 1; i <= 10; i++)
    {
        FILE * fin, * fout;
        string in_file = "i" + to_string(x) + "k.txt";
        string out_file = "mi" + to_string(x) + "k.txt";
        fin = fopen(in_file.c_str(), "r");
        fout = fopen(out_file.c_str(), "w");
        for(int i = 0; i < 1000 * x; i++)
        {
            fscanf(fin, "%d", &a[i]);
        }
        time_t start_sorting = clock();
        merge_sort(a, 0, 1000 * x - 1);
        time_t end_sorting = clock();
        for(int i = 0; i < 1000 * x; i++)
            fprintf(fout, "%d\n", a[i]);
        fclose(fin);
        fclose(fout);
        time_t end_sorting_and_writing = clock();
        cout << "Integers " << x << "K " << (float)(end_sorting - start_sorting) / (CLOCKS_PER_SEC) << " " << (float)(end_sorting_and_writing - start_sorting) / (CLOCKS_PER_SEC) << '\n';
        x *= 2;
    }
    x = 10;
    for(int i = 1; i <= 10; i++)
    {
        FILE * fin, * fout;
        string in_file = "i" + to_string(x) + "k.txt";
        string out_file = "qi" + to_string(x) + "k.txt";
        fin = fopen(in_file.c_str(), "r");
        fout = fopen(out_file.c_str(), "w");
        for(int i = 0; i < 1000 * x; i++)
        {
            fscanf(fin, "%d", &a[i]);
        }
        time_t start_sorting = clock();
        quickSort(a, 0, 1000 * x - 1);
        time_t end_sorting = clock();
        for(int i = 0; i < 1000 * x; i++)
            fprintf(fout, "%d\n", a[i]);
        fclose(fin);
        fclose(fout);
        time_t end_sorting_and_writing = clock();
        cout << "Integers " << x << "K " << (float)(end_sorting - start_sorting) / (CLOCKS_PER_SEC) << " " << (float)(end_sorting_and_writing - start_sorting) / (CLOCKS_PER_SEC) << '\n';
        x *= 2;
    }
    return 0;
}