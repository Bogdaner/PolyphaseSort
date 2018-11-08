#pragma once
#include "Tape.h"
#include <array>
#include <memory>
#include <vector>

template<typename T>
class PolyphaseSort
{
public:
	PolyphaseSort(long long unsigned int rec, bool p);
	PolyphaseSort(std::string fname, bool p);
	~PolyphaseSort();
	void sort();
private:
	static constexpr unsigned int t = 3; // number of tapes

	void init_tapes();
	void initial_distribution();
	void merge();
	void print_disk_op() const;
	void print_tapes();
	long long unsigned int get_number_of_series() const;
	unsigned int get_dummies_tape() const;
	unsigned int get_tape_for_saving() const;
	unsigned int get_shortest_tape() const;
	unsigned int get_min_rec(const std::vector<std::pair<T, int>>& v) const;
	void set_previous_to_min();

	const bool print;
	FileOperations<Record> file;
	std::array<std::unique_ptr<Tape<T>>, t> tapes;
protected:
};
