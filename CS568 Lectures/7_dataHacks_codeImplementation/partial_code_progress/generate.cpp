#include<fstream>
#include<iostream>
#include<stdlib.h>

using namespace std;

double rfloat()
{
	return float(rand()) / float(RAND_MAX);
}
int main()
{
	srand(time(NULL));
	fstream fout;
	fout.open("data.csv", ios::out);

	for(int i = 0; i < 100; i++)
	{
		double x = rfloat() * 10;
		double y = rfloat() * 10;
		double z = rfloat() * 10;
		fout << x << ' ' << y << ' ' << z << '\n';
	}

	for(int i = 0; i < 100; i++)
	{
		double x = rfloat() * 10 + 100;
		double y = rfloat() * 10 + 100;
		double z = rfloat() * 10 + 100;
		fout << x << ' ' << y << ' ' << z << '\n';
	}

	for(int i = 0; i < 100; i++)
	{
		double x = rfloat() * 10 + 100;
		double y = rfloat() * 10;
		double z = rfloat() * 10;
		fout << x << ' ' << y << ' ' << z << '\n';
	}

	for(int i = 0; i < 100; i++)
	{
		double x = rfloat() * 10 + 100;
		double y = rfloat() * 10;
		double z = rfloat() * 10 + 100;
		fout << x << ' ' << y << ' ' << z << '\n';
	}

	fout.close();
	return 0;
}
