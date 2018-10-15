#include "FileOperations.h"

FileOperations::FileOperations()
{
	generate_file();
}

Record FileOperations::get_next_record()
{
	return Record();
}

void FileOperations::save_record(Record r)
{

}

void FileOperations::save_buffer()
{
	if (buf.size() > 0)
	{
		file.write(reinterpret_cast<const char*>(&buf), buf.size()* R);
		buf.clear();
		//file.flush();
	}
}

void FileOperations::save_to_buffer(const Record r)
{
	if (buf.size()*R < B)
		buf.push_back(r);
	else
	{
		save_buffer();
		buf.push_back(r);
	}
}

void FileOperations::generate_file()
{
	using namespace std;

	file.open("gen.bin", fstream::binary | fstream::in | fstream::out | fstream::trunc);
	if (!file.good())
		cout << "File creation failed\n";

	int i;
	cout << "Enter the number of records\n";
	cin >> i;
	for (int j{ 0 }; j < i; j++)
	{
		Record r;
		r.random();
		save_to_buffer(r);
	}
	save_buffer();
}

const unsigned int FileOperations::t = 2;
const unsigned int FileOperations::R = sizeof(float) * 3;
const unsigned int FileOperations::B = R * 5;
