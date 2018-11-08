#pragma once
#include "FileOperations.h"
#include <chrono>
#include <thread>

template<typename T>
class Tape : public FileOperations<T>
{
public:
	Tape() = default;
	Tape(int i);
	Tape(std::string s);
	~Tape();
	void clear();
	unsigned int series;
	unsigned int dummies;
	T prev;
private:
protected:
};

