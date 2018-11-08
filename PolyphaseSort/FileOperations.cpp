#include "FileOperations.h"

template FileOperations<Record>::FileOperations(long long unsigned int);
template FileOperations<Record>::FileOperations(std::string);
template Record FileOperations<Record>::get_next_record();
template void FileOperations<Record>::save_record(Record r);
template void FileOperations<Record>::print_file(void);
template void FileOperations<Record>::return_to_beg(void);
template bool FileOperations<Record>::end_of_file();
template void FileOperations<Record>::fill_buffer(void);
template void FileOperations<Record>::save_buffer(void);
template void FileOperations<Record>::save_to_buffer(const Record r);

template<typename T>
FileOperations<T>::FileOperations(std::string s) : p{ 0 }, file_name{ s }, file(file_name, file.binary | file.out | file.in),
disk_operations{ 0 }
{
	if (!file.good())
		std::cout << "File creation failed\n";
}

template<typename T>
FileOperations<T>::FileOperations(long long unsigned int i) : p{ 0 }, disk_operations{ 0 }
{
	using namespace std;

	std::cout << "Enter the file name:\n";
	std::cin >> file_name;
	file.open(file_name, fstream::binary | fstream::in | fstream::out | fstream::trunc);
	if (!file.good())
		cout << "File creation failed\n";
	char c;
	cout << "Want to assign values to records manually?(y/n):\n";
	cin >> c;

	for (int j{ 0 }; j < i; j++)
	{
		T r;
		if (c == 'y' || c == 'Y')
			cin >> r;
		else
			r.random(); // record must have random() method
		save_to_buffer(r);
	}
	save_buffer();
	file.seekp(file.beg);
}

template<typename T>
T FileOperations<T>::get_next_record()
{
	if (file.gcount() == 0 && !end_of_file())
		fill_buffer();
	T to_return = buf[p];
	p++;
	if (p >= file.gcount() / R)
		fill_buffer();
	return to_return;
}

template<typename T>
void FileOperations<T>::save_record(const T r)
{
	save_to_buffer(r);
}

template<typename T>
void FileOperations<T>::print_file()
{
	if (end_of_file())
		return;
	// remember some stuff
	long long unsigned int disk = disk_operations;
	int pos = file.tellp();
	int count = file.gcount();
	std::array<T, n> tmp = buf;
	unsigned int tmp_p = p;

	file.clear();
	int i = 0;
	while (!end_of_file())
	{
		std::cout << get_next_record() << " ";
		i++;
		if (i % 10 == 0)
		{
			i = 0;
			std::cout << "\n";
		}
	}
	if (i != 0)
		std::cout << "\n";

	// restore
	file.clear();
	file.seekp(file.beg);
	file.read(reinterpret_cast<char *>(&buf), count); // restore gcount
	file.clear();

	disk_operations = disk;
	p = tmp_p;
	buf = tmp;
	file.seekp(pos);
	if (pos == -1)
		file.setstate(file.eofbit);
}

template<typename T>
void FileOperations<T>::return_to_beg()
{
	file.clear();
	file.seekp(file.beg);
}

template<typename T>
bool FileOperations<T>::end_of_file()
{
	if (file.eof() && file.gcount() == 0) // EOF reached and buff empty
		return true;
	else
		return false;
}

template<typename T>
void FileOperations<T>::fill_buffer()
{
	file.read(reinterpret_cast<char *>(&buf), B);
	p = 0;
	disk_operations++;
}

template<typename T>
void FileOperations<T>::save_buffer()
{
	file.write(reinterpret_cast<char *>(&buf), p * R);
	file.flush();
	p = 0;
	disk_operations++;
	buf = std::array<T, n>();
}

template<typename T>
void FileOperations<T>::save_to_buffer(const T r)
{
	if (p >= n)
		save_buffer();
	buf[p] = r;
	p++;
}