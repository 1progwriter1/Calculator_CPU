#ifndef CALCULATE_CALC
#define CALCULATE_CALC

#include <stdio.h>
#include "Stack/stack_values.h"
#include "calculator_values.h"

enum Result Calculate(Calc *calcdata, const char *file);
enum Result CalcCtor(Calc *calcdata);
enum Result CalcDtor(Calc *calcdata);
int CalcVerify(Calc *calcdata);
void DumpCalc(Calc *calcdata, unsigned int error, int correct_reg);

#endif
