#include <iostream>
#include <ctime>
#include <string>
#include "Record.h"
#include "PolyphaseSort.h"


int main(int argc, char** argv)
{
	srand(time(NULL));
	bool print = false;
	long long unsigned int r = 0;
	std::string fname;
	for (int i = 1; i < argc; ++i)
	{
		std::string s{ argv[i] };
		if (s == "-p")
		{
			print = true;
			continue;
		}
		else if (s == "-g")
		{
			std::cout << "Enter number of records: \n";
			std::cin >> r;
			continue;
		}
		else
			fname = s;
	}
	if (r != 0)
	{
		PolyphaseSort<Record> sort{ r, print };
		sort.sort();
	}
	else if (fname != "")
	{
		PolyphaseSort<Record> sort{ fname, print };
		sort.sort();
	}
	else
	{
		std::cout << "Wrong parameters\n";
		return -1;
	}
	
	return 0;
}