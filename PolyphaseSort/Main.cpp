#include <iostream>
#include <ctime>
#include "Record.h"
#include "PolyphaseSort.h"


int main(void)
{
	srand(time(NULL));
	PolyphaseSort<Record> sort{10};
	sort.sort();
	return 0;
}