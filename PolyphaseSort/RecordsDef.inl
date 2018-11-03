#pragma once
#include "Record.h"
#include "FileOperations.h"

template FileOperations<Record>::FileOperations(int);
template FileOperations<Record>::FileOperations(std::string);
//template FileOperations<Record>::~FileOperations(void);
template Record FileOperations<Record>::get_next_record();
template void FileOperations<Record>::save_record(Record r);
template void FileOperations<Record>::print_file(void);
template void FileOperations<Record>::return_to_beg(void);
template bool FileOperations<Record>::end_of_file();
template void FileOperations<Record>::fill_buffer(void);
template void FileOperations<Record>::save_buffer(void);
template void FileOperations<Record>::save_to_buffer(const Record r);

//template Tape<Record>::Tape(void);