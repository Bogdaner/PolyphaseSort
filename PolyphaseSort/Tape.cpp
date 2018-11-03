#include "Tape.h"

template Tape<Record>::Tape(int);
template Tape<Record>::Tape(std::string);
template Tape<Record>::~Tape(void);
template void Tape<Record>::clear(void);


template<typename T>
Tape<T>::Tape(int i) : FileOperations<T>(i), series{ 0 }, dummies{ 0 }, prev{ T().min() }
{

}

template<typename T>
Tape<T>::Tape(std::string s) : series{ 0 }, dummies{ 0 }, prev{ T().min() }
{
	this->file_name = s;
	this->file.open(s, std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
	if (!this->file.good())
		std::cout << "File creation failed\n";
}

template<typename T>
void Tape<T>::clear()
{
	prev.min();
	this->file.close();
	std::remove(this->file_name.c_str());
	this->file.open(this->file_name, std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
	if (!this->file.good())
		std::cout << "File creation failed\n";
}

template<typename T>
Tape<T>::~Tape()
{
	//this->file.close();
	//std::remove(this->file_name.c_str());
}