#pragma once
#include <iostream>
#include <fstream>
#include <vector>

struct Record
{
	float p_a;
	float p_b;
	float p_ab;
	void random()
	{
		p_a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		p_b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		p_ab = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
};

class FileOperations
{
public:
	FileOperations();
	Record get_next_record();
	void save_record(Record r);
private:
	void save_buffer();
	void save_to_buffer(const Record r);
	void generate_file();
	std::fstream file;
	std::vector<Record> buf;
	std::vector<std::vector<Record>> tape_buf;
	static const unsigned int t; // number of tapes
	static const unsigned int B; // read/save unit from file (bytes)
	static const unsigned int R; // record size in bytes

protected:
};