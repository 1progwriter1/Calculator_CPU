#ifndef PROCESSOR_CONFIGURATION
#define PROCESSOR_CONFIGURATION

#define logs_file "logs.txt" // set file to print error messages during stack operation (default: stdout)
#define PROTECT // stack protection activation

typedef long long int Elem_t; // type
#define output_id "%lld" // format for printf/scanf

const int RAM_SIZE = 10000; // RAM size

const int PRECISION = 3; // precision

#endif