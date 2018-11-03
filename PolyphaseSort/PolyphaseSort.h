#pragma once
#include "Tape.h"
#include <array>
#include <memory>
#include <vector>

template<typename T>
class PolyphaseSort
{
public:
	PolyphaseSort(int rec);
	void sort();
private:
	static constexpr unsigned int t = 3; // number of tapes

	void initial_distribution();
	void merge();

	void print_tapes();
	unsigned int get_number_of_series() const;
	unsigned int get_dummies_tape() const;
	unsigned int tape_for_saving() const;
	unsigned int shortest_tape() const;
	unsigned int get_min_rec(const std::vector<std::pair<T, int>>& v) const;
	void set_previous_to_min();
	FileOperations<Record> file;
	std::array<std::unique_ptr<Tape<T>>, t> tapes;
protected:
};
