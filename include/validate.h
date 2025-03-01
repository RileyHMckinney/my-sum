#ifndef VALIDATE_H
#define VALIDATE_H

#include <string>

const int MAX_N = 10000000; //sets the maximum 'n' to ten million

int validate_arguments(int argc, char *argv[], int &n, int &m, std::string &input_file, std::string &output_file);
bool exists_and_enough_numbers(int n, const std::string &filename, bool &fileExists);
int validate_n_m(int n, int m);
void validate_io_files(const std::string &input_file,const std::string &output_file);
bool isWhitespaceOnly(const std::string &s);

#endif // VALIDATE_H