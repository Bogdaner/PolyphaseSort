#pragma once
#include "Record.h"
#include <iostream>
#include <fstream>
#include <array>
#include <string>

template<typename T>
class FileOperations
{
public:
	FileOperations() = default;
	FileOperations(std::string s);
	FileOperations(int i); // generate file with i records
	virtual T get_next_record();
	virtual void save_record(const T r);
	void save_buffer();
	void print_file();
	void return_to_beg();
	bool end_of_file();
	std::string file_name;
private:
	static constexpr unsigned int R = sizeof(T); // record size in bytes
	static constexpr unsigned int n = 100; // number of records read/save
	static constexpr unsigned int B = R * n; // read/save unit from file (bytes)

	unsigned int saves;
	void fill_buffer();
	void save_to_buffer(const T r);
protected:
	std::fstream file;
	std::array<T, n> buf;
	unsigned int p;
};