// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StringSorter.h"

int main()
{
	int threadCount;
	string inputFile, outputFile;
	cout << "Number of threads: ";
	cin >> threadCount;
	cout << "Input file: ";
	cin >> inputFile;
	cout << "Output file: ";
	cin >> outputFile;
	StringSorter sorter(threadCount);
	sorter.sort(inputFile, outputFile);
	printf("File was succesfully sorted\n");
	getchar();
	getchar();
	return 0;
}
