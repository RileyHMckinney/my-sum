#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <stdexcept>
#include "../include/validate.h"

using namespace std;


/* validates the input arguments
    - argc - the argument count
    - *argv[] - the array of arguments
    - &n - the number of values to be read
    - &m - the number of processes 
    - &input-file - the input file name
    - &output-file - the output file name
*/
int validate_arguments(int argc, char *argv[], int &n, int &m, string &input_file, string &output_file){
    //check that valid number of arguments entered 
    if(argc != 5){
        cerr << "Usage: " << argv[0] << " <n> <m> <input_filename.txt> <output_filename.txt>" <<endl;
        exit(EXIT_FAILURE);
    }

    //check that n and m are integers greater than 0
    try {
        n = stoi(argv[1]);
        m = stoi(argv[2]);

    } catch (exception &e) {
        cerr << "Error: n and m must be integer values" << endl;
        exit(EXIT_FAILURE);
    }
    
    //Check if n and m are valid
    m = validate_n_m(n, m);

    input_file = argv[3];
    output_file = argv[4];

    //check if input_file, output_file are valid
    validate_io_files(input_file, output_file);

    //check if file exists and has at least n numbers
    bool fileExists;
    if(!(exists_and_enough_numbers(n, input_file, fileExists))){
        if(!fileExists){
            cerr << "Error: Input file does not exist or cannot be opened" << endl;
        } else {
            cerr << "Error: Less than " << n << " numbers in input file" << endl;
        }
        exit(EXIT_FAILURE);
    }

    //return the value of m, in the event that m is changed by validate-n-m()
    return m;
}

/*checks if the filename exists and contains at least n numbers
    - n - the number of values read
    - &filename - the filename
    - &fileExists - the boolean for if a file exists 
*/
bool exists_and_enough_numbers(int n, const string &filename, bool &fileExists){
    //Establish filename as the input file
    ifstream infile(filename);
    if(!infile){
        fileExists = false;
        return false;
    }

    // Checks if filename contains at least n numbers
    fileExists = true;
    int val;
    for(int i = 0; i < n; i++){
        if(!(infile >> val)){
            infile.close();
            return false; //file ended before n numbers were found
        }
    }

    infile.close();
    return true; //n numbers were found in the file
}

int validate_n_m(int n, int m){
    //check if n and m are greater than 0
    if(n <= 0 || m <= 0){
        cerr << "Error: n and m must be greater than 0" << endl;
        exit(EXIT_FAILURE);
    }

    //check that n does not exceed MAX_N
    if(n > MAX_N){
        cerr << "Error: n is too large, and cannot exceed " << MAX_N << "." << endl;
        exit(EXIT_FAILURE);
    }

    //check that m does not exceed n
    if(m > n){
        cerr << "Warning: Requested processes (" << m 
            << ") is greater than number of elements in input array (" << n << ")." << endl;
        m = n;
    }

    //Check if m is greater than the number of available cores
    int available_cores = thread::hardware_concurrency(); //works on utd server
    if(m > available_cores){
        cerr << "Warning: Requested processes (" << m << ") is greater than available cores (" << available_cores << ")." << endl;
    }

    return m;
}

/*ensures that the input and output files are valid 
    - &input_file - the input file name
    - &output_file - the output file name
*/
void validate_io_files(const string &input_file,const string &output_file){
    //check that input_file and output_file are not empty or only whitespace
    if(input_file.empty() || output_file.empty() || isWhitespaceOnly(input_file) || isWhitespaceOnly(output_file)){
        cerr << "Error: Input and output file names cannot be empty or whitespace." << endl;
        exit(EXIT_FAILURE);
    }

    //check that input and output files are different files
    if(input_file == output_file){
        cerr << "Error: Input and output file must be different." << endl;
        exit(EXIT_FAILURE);
    }

    //check that output_file can be created or written to
    ofstream test_out(output_file);
    if(!test_out){
        cerr << "Error: output file cannot be created or written to." << endl;
        exit(EXIT_FAILURE);
    }
    test_out.close();
}

/*checks to see if a string consists only of whitespace
    - $s - a string
*/
bool isWhitespaceOnly(const string &s){
    for (char c : s) {
        if (!isspace(c)) return false;  //a non-whitespace character exists
    }
    return true;  // The string is all whitespace
}